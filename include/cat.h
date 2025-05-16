#pragma once

#include <stdio.h>      // printf, fgets
#include <stdlib.h>     // malloc, NULL
#include <string.h>     // strcmp, strcat, strncpy, strtok

#include "system.h"     // tree, dir, file 구조체 및 new_file() 정의





void cat(tree* structure, char* name, int option)
{
    dir* temp = structure->current;
    file* files = structure->current->file_inside;
    file* now = NULL;

    if (option == 0) // cat [filename]
    {
        if (files == NULL)
        {
            printf("cat: No such file or directory\n");
        }
        else
        {
            while (files != NULL)
            {
                if (strcmp(files->name, name) == 0)
                {
                    printf("%s", files->data);
                    return;
                }
                files = files->right;
            }
            printf("cat: No such file or directory\n");
        }
    }

    else if (option == 1) // cat > filename
    {
        file* newfile = new_file(name);
        newfile->data[0] = '\0'; // ⭐ strcat 안전용 초기화

        // 디렉토리에 연결
        if (temp->file_inside == NULL)
        {
            temp->file_inside = newfile;
        }
        else
        {
            file* cursor = temp->file_inside;
            while (cursor->right != NULL)
                cursor = cursor->right;
            cursor->right = newfile;
        }

        // 사용자 입력
        char t[1024];
        while (1)
        {
            char* te = fgets(t, 1024, stdin);
            if (strcmp(te, ":q\n") == 0)
                break;
            strcat(newfile->data, te);
        }
    }

    else if (option == 2) // cat -n filename
    {
        if (files == NULL)
        {
            printf("cat: No such file or directory\n");
        }
        else
        {
            int num = 1;
            while (files != NULL)
            {
                if (strcmp(files->name, name) == 0)
                {
                    char temp_data[1024];
                    strcpy(temp_data, files->data);
                    char* te = strtok(temp_data, "\n");
                    while (te != NULL)
                    {
                        printf("\t%d  %s\n", num++, te);
                        te = strtok(NULL, "\n");
                    }
                    return;
                }
                files = files->right;
            }
            printf("cat: No such file or directory\n");
        }
    }
}
void init_cat(tree* structure, char* name, char* content)
{
    file* temp = new_file(name);
    strncpy(temp->data, content, 1024);

    if (structure->current->file_inside == NULL)
    {
        structure->current->file_inside = temp;
    }
    else
    {
        file* now = structure->current->file_inside;
        while (now->right != NULL)
            now = now->right;
        now->right = temp;
    }
}

