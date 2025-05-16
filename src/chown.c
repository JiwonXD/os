#include <stdio.h>
#include <string.h>
#include "../include/system.h"  // Assumes file and dir structs include user field
#include "../include/chown.h"
#include "../include/getpointer.h"
extern tree* structure;
// Change ownership of a file or directory by name
void chownCommand(dir* current, const char* path, const char* new_owner) {
    dir* parent = current;
    char* last = strrchr(path,'/'); // 마지막 '/' 위치 찾기
    char target_name[100];

    if (last != NULL) {
        strncpy(target_name, last + 1, sizeof(target_name)); // 파일/디렉토리 이름 추출
        char parent_path[100];
        strncpy(parent_path, path, last - path);             // 상위 디렉토리 경로 추출
        parent_path[last - path] = '\0';
        parent = get_pointer(structure->root, current, parent_path);    // 상위 디렉토리 탐색
    } else {
        strncpy(target_name, path, sizeof(target_name));     // 이름만 있는 경우
    }

    if (!parent) {
        printf("chown '%s' No such directory\n", path);
        return;
    }

    file* f = parent->file_inside;
    while (f) {
        if (strcmp(f->name, target_name) == 0) {
            strcpy(f->user, new_owner); // ← 구조체에 user 필드가 있어야 함
            printf("Changed owner of file '%s' to '%s'\n", f->name, new_owner);
            return;
        }
        f = f->right;
    }

    dir* d = parent->left;
    while (d) {
        if (strcmp(d->name, target_name) == 0) {
            strcpy(d->user, new_owner); // ← 구조체에 user 필드가 있어야 함
            printf("Changed owner of directory '%s' to '%s'\n", d->name, new_owner);
            return;
        }
        d = d->right;
    }

    printf("'%s': No such file or directory\n", path);
}

