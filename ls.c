#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "system.h"
#include "ls.h"
#include "getpointer.h"

// 권한을 rwx 문자열로 변환하는 함수
void formatPermission(int perm, char* buf){
    int digits[3];
    digits[0] = perm / 100;          // owner permissions
    digits[1] = (perm / 10) % 10;    // group permissions
    digits[2] = perm % 10;           // other permissions

    int pos = 0;
    for(int i = 0; i < 3; i++){
        int val = digits[i];
        buf[pos++] = (val & 4) ? 'r' : '-';
        buf[pos++] = (val & 2) ? 'w' : '-';
        buf[pos++] = (val & 1) ? 'x' : '-';
    }
    buf[9] = '\0';
}

// 파일 정보 출력
void printFileInfo(file *f, int opt_l){
    if(opt_l){
        char time_buf[64];
        char perm_buf[10];
        formatPermission(f->permission, perm_buf);
        strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", f->time);
        printf("-%s 1 %s group %6d %s %s\n",
               perm_buf,f->user, 1024, time_buf, f->name);
    } else {
        printf("%s  ", f->name);
    }
}

// 디렉토리 정보 출력
void printDirInfo(dir* d, int opt_l){
    if(opt_l){
        char time_buf[64];
        char perm_buf[10];
        formatPermission(d->permission, perm_buf);
        strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", d->time);
        printf("d%s 2 %s group %6d %s %s/\n",
               perm_buf,d->user, 4096, time_buf, d->name);
    } else {
        printf("%s/  ", d->name);
    }
}

// 디렉토리 내 파일과 서브디렉토리 출력
void listDirectory(dir* target, int opt_a, int opt_l){
    int totalsize = 0;

    // 파일과 디렉토리를 한 번의 반복으로 크기 계산 및 출력
    file* f = target->file_inside;
    dir* d = target->left;

    // 먼저 총 블록 수 계산
    file* tf = f;
    dir* td = d;
    while (tf != NULL || td != NULL) {
        if (tf != NULL) {
            if(opt_a || tf->name[0] != '.') {
                totalsize += (1024 + 511) / 512;
            }
            tf = tf->right;
        }
        if (td != NULL) {
            if(opt_a || td->name[0] != '.') {
                totalsize += (4096 + 511) / 512;
            }
            td = td->right;
        }
    }

    if(opt_l) printf("total %d\n", totalsize); // ls -l 출력 시 total은 항상 맨 위에 표시됨

    // 출력 루프
    while (f != NULL || d != NULL) {
        if (f != NULL) {
            // opt_a가 1이고 이름이 '.'으로 시작하는 경우: 숨김파일 출력
            if(opt_a || f->name[0] != '.') {
                printFileInfo(f, opt_l);
            }
            f = f->right;
        }
        if (d != NULL) {
            // opt_a가 1이고 이름이 '.'으로 시작하는 경우: 숨김디렉토리 출력
            if(opt_a || d->name[0] != '.') {
                printDirInfo(d, opt_l);
            }
            d = d->right;
        }
    }

    if(!opt_l) printf("\n");
}

// ls 명령어 실행
void lsCommand(dir* current, const char* option){
    int opt_a = 0, opt_l = 0;
    char path[100] = "";

    // 옵션 또는 경로 분리
    if(option && strlen(option) > 0){
        if(option[0] == '-'){
            for(int i = 1; option[i] != '\0'; i++){
                if(option[i] == 'a') opt_a = 1;
                if(option[i] == 'l') opt_l = 1;
            }
        } else {
            strcpy(path, option);  // 경로로 인식
        }
    }

    dir* target = current;
    if(strlen(path) > 0){
       	extern tree* structure;  // 전역 트리 선언 (필요시)
	dir* resolved = get_pointer(structure->root, current, path);
        if(resolved != NULL){
            target = resolved;
        } else {
            printf("ls: cannot access '%s': No such directory\n", path);
            return;
        }
    }

    listDirectory(target, opt_a, opt_l);
}
