#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "system.h"

// 루트 디렉토리 생성
dir* new_root() {
    dir* rootnode = (dir*)malloc(sizeof(dir));
    strcpy(rootnode->name, "root");
    rootnode->permission = 755;
    rootnode->file_inside = NULL;

    struct tm* timetemp = (struct tm*)malloc(sizeof(struct tm));
    time_t timenow = time(NULL);
    memcpy(timetemp, localtime(&timenow), sizeof(struct tm));
    rootnode->time = timetemp;

    rootnode->parent = NULL;
    rootnode->left = NULL;
    rootnode->right = NULL;

    return rootnode;
}

// 파일 시스템 트리 생성
tree* new_tree(dir* rootnode) {
    tree* treenode = (tree*)malloc(sizeof(tree));
    treenode->root = rootnode;
    treenode->current = rootnode;
    return treenode;
}

// 새 파일 생성
file* new_file(char* name) {
    file* filenode = (file*)malloc(sizeof(file));
    strcpy(filenode->name, name);
    strcpy(filenode->user,"user");
    filenode->permission = 755;

    struct tm* timetemp = (struct tm*)malloc(sizeof(struct tm));
    time_t timenow = time(NULL);
    memcpy(timetemp, localtime(&timenow), sizeof(struct tm));
    filenode->time = timetemp;

    filenode->right = NULL;
    return filenode;
}

// 새 디렉토리 생성
dir* new_dir(char* name) {
    dir* dirnode = (dir*)malloc(sizeof(dir));
    strcpy(dirnode->name, name);
    strcpy(dirnode->user,"user");
    dirnode->permission = 755;
    dirnode->file_inside = NULL;

    struct tm* timetemp = (struct tm*)malloc(sizeof(struct tm));
    time_t timenow = time(NULL);
    memcpy(timetemp, localtime(&timenow), sizeof(struct tm));
    dirnode->time = timetemp;

    dirnode->parent = NULL;
    dirnode->left = NULL;
    dirnode->right = NULL;

    return dirnode;
}

