#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAX_NAME_LEN 256        // 파일/디렉토리 이름 최대 길이
#define MAX_PATH_LEN 1024       // 경로 최대 길이
#define MAX_NODES 1024          // 최대 파일/디렉토리 수 (정적 자료구조로 제한)

// ---------- 자료구조 정의 ----------

// 파일/디렉토리 노드를 표현하는 구조체
typedef struct FileNode {
    char name[MAX_NAME_LEN];  // 파일명 또는 디렉토리명
    int is_dir;               // 1이면 디렉토리, 0이면 일반 파일
    int parent_index;         // 부모 노드의 인덱스 (트리 형태 구현)
    time_t created_time;      // 생성 시간 (로그에 사용 가능)
    int deleted;              // 삭제 여부 플래그 (1이면 삭제된 상태)
} FileNode;

// 스레드 인자로 사용할 구조체
typedef struct {
    char path[MAX_PATH_LEN];  // 삭제할 경로
    int recursive;            // -r 옵션 (하위 포함 삭제 여부)
    int force;                // -f 옵션 (에러 무시 여부)
} rm_args;

// ---------- 전역 변수 ----------

FileNode file_pool[MAX_NODES];  // 메모리상 "파일 시스템" 저장소
int file_count = 0;             // 현재 저장된 파일/디렉토리 수

// ---------- 유틸리티 함수 ----------

// 로그 출력 함수: rm_log.txt에 성공/실패 기록
void log_result(const char *message, const char *path) {
    FILE *fp = fopen("rm_log.txt", "a");
    if (!fp) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(fp, "[%04d-%02d-%02d %02d:%02d:%02d] %s: %s\n",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec,
            message, path);
    fclose(fp);
}

// 루트 디렉토리 생성 함수
void init_root() {
    strcpy(file_pool[0].name, "/");
    file_pool[0].is_dir = 1;
    file_pool[0].parent_index = -1;  // 루트는 부모 없음
    file_pool[0].created_time = time(NULL);
    file_pool[0].deleted = 0;
    file_count = 1;
}

// 특정 부모 디렉토리 하위에서 특정 이름의 노드를 찾는다
int find_node(int parent_index, const char *name) {
    for (int i = 0; i < file_count; i++) {
        if (!file_pool[i].deleted &&
            file_pool[i].parent_index == parent_index &&
            strcmp(file_pool[i].name, name) == 0) {
            return i;
        }
    }
    return -1;  // 없음
}

// 새로운 노드 추가
int add_node(const char *name, int is_dir, int parent_index) {
    if (file_count >= MAX_NODES) return -1;
    strncpy(file_pool[file_count].name, name, MAX_NAME_LEN - 1);
    file_pool[file_count].name[MAX_NAME_LEN - 1] = '\0';
    file_pool[file_count].is_dir = is_dir;
    file_pool[file_count].parent_index = parent_index;
    file_pool[file_count].created_time = time(NULL);
    file_pool[file_count].deleted = 0;
    return file_count++;
}

// 경로를 따라가며 해당 노드의 인덱스를 찾는 함수
int find_path(const char *path) {
    char temp[MAX_PATH_LEN];
    strncpy(temp, path, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';

    char *token = strtok(temp, "/");
    int current_index = 0;  // 루트부터 시작

    while (token) {
        int found = find_node(current_index, token);
        if (found == -1) return -1;  // 경로 중간에 없음
        current_index = found;
        token = strtok(NULL, "/");
    }
    return current_index;
}

// 노드 삭제 (재귀 포함 가능)
void delete_node(int index, int recursive, int force) {
    if (file_pool[index].deleted) return;

    // 디렉토리인데 -r 옵션이 없으면 거부
    if (file_pool[index].is_dir && !recursive) {
        if (!force)
            fprintf(stderr, "Cannot delete directory without -r: %s\n", file_pool[index].name);
        log_result("Failed", file_pool[index].name);
        return;
    }

    // 디렉토리라면 자식들도 삭제 (재귀)
    if (file_pool[index].is_dir) {
        for (int i = 0; i < file_count; i++) {
            if (file_pool[i].parent_index == index && !file_pool[i].deleted) {
                delete_node(i, recursive, force);
            }
        }
    }

    // 최종 삭제
    file_pool[index].deleted = 1;
    log_result("Deleted", file_pool[index].name);
}

// 스레드에서 실행될 삭제 함수
void *rm_thread(void *arg) {
    rm_args *args = (rm_args *)arg;
    int idx = find_path(args->path);

    if (idx == -1) {
        if (!args->force)
            fprintf(stderr, "Not found: %s\n", args->path);
        log_result("Failed", args->path);
    } else {
        delete_node(idx, args->recursive, args->force);
    }

    return NULL;
}

// ---------- 메인 함수 ----------

int main(int argc, char *argv[]) {
    int opt;
    int recursive = 0;  // -r
    int force = 0;      // -f

    // 옵션 파싱
    while ((opt = getopt(argc, argv, "rf")) != -1) {
        if (opt == 'r') recursive = 1;
        if (opt == 'f') force = 1;
    }

    if (optind >= argc) {
        fprintf(stderr, "Usage: %s [-r] [-f] path1 path2 ...\n", argv[0]);
        return 1;
    }

    // 테스트용 디렉토리 구조 초기화
    init_root();
    int d1 = add_node("home", 1, 0);
    int d2 = add_node("user", 1, d1);
    add_node("file.txt", 0, d2);
    add_node("docs", 1, d2);
    add_node("log.txt", 0, d2);

    int count = argc - optind;
    pthread_t threads[count];
    rm_args args[count];

    // 스레드 생성 (동시 삭제 처리)
    for (int i = 0; i < count; i++) {
        strncpy(args[i].path, argv[optind + i], MAX_PATH_LEN);
        args[i].path[MAX_PATH_LEN - 1] = '\0';
        args[i].recursive = recursive;
        args[i].force = force;
        pthread_create(&threads[i], NULL, rm_thread, &args[i]);
    }

    // 스레드 완료 대기
    for (int i = 0; i < count; i++) {
        pthread_join(threads[i], NULL);
    }

    // 삭제 이후 남은 파일 시스템 출력
    printf("\n[ In-memory file system status after deletion ]\n");
    for (int i = 0; i < file_count; i++) {
        if (!file_pool[i].deleted)
            printf("%s (parent: %d)\n", file_pool[i].name, file_pool[i].parent_index);
    }

    return 0;
}
