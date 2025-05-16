
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAX_NAME_LEN 256        // 디렉토리 이름 최대 길이
#define MAX_PATH_LEN 1024       // 경로 최대 길이
#define MAX_NODES 1024          // 최대 디렉토리 수 (정적 배열 기반)

// 디렉토리 노드 구조체 (자료구조 기반 디렉토리 표현)
typedef struct Directory {
    char name[MAX_NAME_LEN];   // 디렉토리 이름
    mode_t permission;         // 권한 정보 (예: 0775)
    int parent_index;          // 부모 디렉토리의 인덱스
    time_t created_time;       // 생성 시간 (로그 출력용 등 활용)
} Directory;

// 스레드에서 받을 인자 구조체
typedef struct {
    char path[MAX_PATH_LEN];   // 생성할 디렉토리 경로
    mode_t mode;               // 권한 모드
    int use_p;                 // -p 옵션 여부 (상위 디렉토리 생성 허용 여부)
} mkdir_args;

Directory directory_pool[MAX_NODES];  // 디렉토리들을 저장할 배열
int directory_count = 0;              // 현재 디렉토리 수

// 로그 기록 함수 (mkdir_log.txt에 결과 기록)
void log_result(const char *message, const char *path) {
    FILE *fp = fopen("mkdir_log.txt", "a");
    if (!fp) return;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(fp, "[%04d-%02d-%02d %02d:%02d:%02d] %s: %s\n",
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec,
            message, path);
    fclose(fp);
}

// 루트 디렉토리 초기화 함수
void init_root() {
    strncpy(directory_pool[0].name, "/", MAX_NAME_LEN);
    directory_pool[0].permission = 0775;
    directory_pool[0].parent_index = -1;           // 루트는 부모 없음
    directory_pool[0].created_time = time(NULL);
    directory_count = 1;
}

// 특정 부모 아래에서 이름이 같은 디렉토리 탐색
int find_child(int parent_index, const char *name) {
    for (int i = 0; i < directory_count; i++) {
        if (directory_pool[i].parent_index == parent_index && strcmp(directory_pool[i].name, name) == 0) {
            return i; // 일치하는 디렉토리 인덱스 반환
        }
    }
    return -1; // 없음
}

// 새 디렉토리 추가 함수
int add_directory(const char *name, mode_t permission, int parent_index) {
    if (directory_count >= MAX_NODES) return -1; // 한계 초과 시 실패
    strncpy(directory_pool[directory_count].name, name, MAX_NAME_LEN - 1);
    directory_pool[directory_count].name[MAX_NAME_LEN - 1] = '\0';
    directory_pool[directory_count].permission = permission;
    directory_pool[directory_count].parent_index = parent_index;
    directory_pool[directory_count].created_time = time(NULL);
    return directory_count++; // 새 디렉토리 인덱스 반환
}

// 자료구조 기반 mkdir 구현 (path 분할 및 경로 탐색)
int mkdir_in_structure(const char *path, mode_t mode, int use_p) {
    char temp[MAX_PATH_LEN];
    strncpy(temp, path, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

    char *token = strtok(temp, "/");
    int current_index = 0; // 루트에서 시작

    while (token) {
        int found = find_child(current_index, token);
        if (found == -1) {
            int new_index = add_directory(token, mode, current_index);
            if (new_index == -1) return -1; // 실패
            current_index = new_index;
        } else {
            current_index = found; // 기존 디렉토리로 이동
        }
        token = strtok(NULL, "/");
    }
    return 0; // 성공
}

// 스레드에서 실행되는 mkdir 함수
void *mkdir_thread(void *arg) {
    mkdir_args *args = (mkdir_args *)arg;
    int result = mkdir_in_structure(args->path, args->mode, args->use_p);

    if (result == 0) {
        log_result("Created", args->path);
    } else {
        log_result("Failed", args->path);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int opt;
    int use_p = 0;           // -p 옵션 여부
    mode_t mode = 0775;      // 기본 권한

    // 옵션 파싱
    while ((opt = getopt(argc, argv, "pm:")) != -1) {
        switch (opt) {
            case 'p':
                use_p = 1;
                break;
            case 'm':
                mode = strtol(optarg, NULL, 8);
                break;
            default:
                printf("Usage: %s [-p] [-m mode] dir1 dir2 ...\n", argv[0]);
                return 1;
        }
    }

    if (optind >= argc) {
        printf("Error: No directory name provided.\n");
        return 1;
    }

    init_root(); // 루트 디렉토리 생성

    int count = argc - optind;
    pthread_t threads[count];     // 스레드 배열
    mkdir_args args[count];       // 각 스레드 인자 배열

    // 디렉토리별 스레드 생성
    for (int i = 0; i < count; i++) {
        strncpy(args[i].path, argv[optind + i], MAX_PATH_LEN);
        args[i].path[MAX_PATH_LEN - 1] = '\0';
        args[i].mode = mode;
        args[i].use_p = use_p;
        pthread_create(&threads[i], NULL, mkdir_thread, &args[i]);
    }

    // 스레드 종료 대기
    for (int i = 0; i < count; i++) {
        pthread_join(threads[i], NULL);
    }

    // 결과 출력
    printf("\n[ Directory structure created (in memory) ]\n");
    for (int i = 0; i < directory_count; i++) {
        printf("%s (parent: %d)\n", directory_pool[i].name, directory_pool[i].parent_index);
    }

    return 0;
}
