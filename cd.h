#pragma once

#include "getpointer.h"
#include <stdio.h>

dir *change_directory(dir *root_dir, dir *cur_pointer, char path[])
{
    dir *result;
    if (strcmp(path, "..") == 0) // 상위 디렉토리로 이동
    {
        if (cur_pointer == root_dir) // 이미 최상위 루트 디렉토리라면 루트 디렉토리 반환
            result = root_dir;
        else
            result = cur_pointer->parent;
    }
    else if (strcmp(path, "~") == 0)
    {
        result = get_pointer(root_dir, cur_pointer, "/home/ubuntu");
    }
    else if (strcmp(path, "--help") == 0)
    {
        printf("cd: cd [dir] [..] [~]\n");
        printf("\tChange the shell working directory.\n");
        printf("\tChange the current directory to DIR.  The default DIR is the value of the HOME shell variable.\n");
        return cur_pointer;
    }
    else
    {
        dir *result = get_pointer(root_dir, cur_pointer, path);
        if (result == NULL) // NULL포인터를 반환하면 존재하지 않는 디렉토리
            printf("no such file or directory: %s\n", path);
        //else if (result->type != 'd')
        //{
        //    printf("cd: not a directory: %s\n", path);
        //}
        else 
            cur_pointer = result;
        return cur_pointer;
    }
    return result;
}

void run_cd(char* path) {
	dir* new_dir = change_directory(structure->root, structure->current, path);
	if(new_dir != NULL) {
		structure->current = new_dir;
	}
}
