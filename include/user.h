#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct userInfo {
    char name[20];   // 사용자 이름
    char group[20];  // 그룹 이름
} userInfo;

typedef struct userList {
    userInfo* curUser;  // 현재 사용자
} userList;

// 사용자 리스트 초기화
static inline userList* initUserList() {
    userList* list = (userList*)malloc(sizeof(userList));
    list->curUser = (userInfo*)malloc(sizeof(userInfo));

    // 예시 사용자 정보 (수정 가능)
    strcpy(list->curUser->name, "osmanager");
    strcpy(list->curUser->group, "admin");

    return list;
}

