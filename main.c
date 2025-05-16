#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "system.h"
#include "getpointer.h"
#include "chown.h"
#include "ls.h"

// 전역 파일 시스템 구조체
tree* structure;

void run(char* cmd);

int main() {
    // 파일 시스템 초기화
    dir* root_dir = new_root();
    structure = new_tree(root_dir);

    char command[300];

    file* testfile = new_file("testfile.txt");
    structure->current->file_inside = testfile;
    testfile->permission = 700;

    while (1) {
        printf("osmanager:%s$ ", structure->current->name);  // 프롬프트
        fgets(command, sizeof(command), stdin);
	run(command);
    }

    
    return 0;
}

void run(char* input)
{
	char* cmd = NULL;
	const char* opt = NULL;
	const char* temppath = NULL;

	dir* temp_dir = NULL;
	file* tempfile = NULL;

	size_t len = strlen(input);
	while(len > 0 && input[len - 1] == '\n') {
		input[--len] = '\0';
	}

	if(strcmp(input, "") == 0)
	{
		return;
	}

	cmd = strtok(input, " ");
	
	if(strcmp(cmd, "ls") == 0)
	{
		opt = strtok(NULL, " ");
		if (opt == NULL) {
			opt = "-";
		}
		lsCommand(structure->current, opt);
	}
	else if(strcmp(cmd, "chown") == 0)
	{
		opt = strtok(NULL, " ");
		temppath = strtok(NULL, " ");
		chownCommand(structure->current, temppath, opt);
	}		
	else if(strcmp(cmd, "exit") == 0)
	{
		exit(0);
	}
	else
	{
		printf("Unknown command: %s\n", cmd);
	}
}
