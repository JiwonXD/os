#include <string.h>
#include <stdlib.h>
#include "getpointer.h"

// 경로 문자열을 따라 디렉토리 포인터 반환
dir* get_pointer(dir* root, dir* cur_pointer, char path[]) {
    dir* result;

    if (strcmp(path, "/") == 0) {
        result = root;
    } else {
        char* temp = strdup(path);
        char* ptr = strtok(temp, "/");

        if (path[0] == '~') {
            result = get_pointer(root, cur_pointer, "/home/ubuntu");
            free(temp);
            temp = strdup(path);
            strtok(temp, "/"); // skip '~'
            ptr = strtok(NULL, "/");
        } else if (path[0] == '/') {
            result = root;
        } else {
            result = cur_pointer;
        }

        while (ptr != NULL && result != NULL) {
            if (strcmp(ptr, ".") == 0) {
                // current directory, do nothing
            } else if (strcmp(ptr, "..") == 0) {
                result = result->parent ? result->parent : NULL;
            } else {
                result = result->left;
                while (result && strcmp(result->name, ptr) != 0) {
                    result = result->right;
                }
            }
            ptr = strtok(NULL, "/");
        }
        free(temp);
    }
    return result;
}

