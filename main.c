#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "system.h"
#include "getpointer.h"
#include "chown.h"
#include "ls.h"

// 전역 파일 시스템 구조체
tree* structure;

void run(char* cmd, int cmdopt);

int main() {
    // 파일 시스템 초기화
    dir* root_dir = new_root();
    structure = new_tree(root_dir);

    char cmd[100];
    char* temp;
    int length;

    while (1) {
        printf("osmanager:%s$ ", structure->current->name);  // 프롬프트
        gets(cmd);
	run(cmd);
    }

    return 0;
}

void run(char* input)
{
	char* cmd = NULL;
	char temp[300];
	char opt;
	dir temp_dir = NULL;
	file* tempfile = NULL;

	strncpy(temp, cmd, 300);

	if(strcmp(input, "") == 0)
	{
		return 0;
	}

	cmd = strtok(input, " ");
	
	else if(strcmp(cmd, "ls") == 0)
	{
		if (strchr(temp, '-') == NULL)
		{
			opt = '-';
			lsCommand(structure, opt);
		}
		else
		{
			cmd = strtok(NULL, " ");
			lsCommand(structure, cmd);
		}
	}

	else if(strcmp(cmd, "exit") == 0)
	{
		return 0;
	}
	else
	{
		printf("Unknown command: %s\n", cmd);
	}
}
