#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "system.h"
#include "command.h"
// 전역 파일 시스템 구조체
tree* structure;
stack* pwdstack;

int main() {
    // 파일 시스템 초기화
    dir* root_dir = new_root();
    structure = new_tree(root_dir);

    char command[300];

    //테스트용 파일,디렉토리 생성
    file* testfile = new_file("testfile.txt");
    structure->current->file_inside = testfile;
    testfile->permission = 700;
    strcpy(testfile->data,"abcabcdabcde");
    dir* testdir = new_dir("testdir");
    root_dir->left = testdir;
    testdir->parent = root_dir;
    file* testfile2 = new_file("testfile2.txt");
    testdir->file_inside = testfile;
    testfile->permission = 700;

    while (1) {
        printf("osmanager:%s$ ", structure->current->name);  // 프롬프트
        fgets(command, sizeof(command), stdin);
	run(command);
    }

    
    return 0;
}

/*
void run(char* input)
{
	char* cmd = NULL;
	const char* opt = NULL;
	const char* temppath = NULL;
	char* pattern = NULL;
	char* filename = NULL;
	int option;

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
	else if(strcmp(cmd, "grep") == 0)
	{
		pattern = strtok(NULL, " ");
		filename = strtok(NULL, " ");
		if (pattern && filename) {
			grep(structure, pattern, filename);
		}
	}
	else if(strcmp(cmd, "find") == 0)
	{
		cmd = strtok(NULL, " ");
		if (cmd == NULL) {
			cmd = "-";
		}
		find(structure, cmd);
	}
	else if(strcmp(cmd, "pwd") == 0)
	{
		pwd(structure, pwdstack);
	}
	else if(strcmp(cmd, "cat") == 0)
	{
		cmd = strtok(NULL, " ");
		filename = strtok(NULL, " ");
		if(strcmp(cmd, ">") == 0)
		{
			option = 1;
			cmd = strtok(NULL, " ");
			cat(structure, cmd, option);
		}
		else if(strcmp(cmd, "-n") == 0)
		{
			option = 2;
			cmd = strtok(NULL, " ");
			cat(structure, cmd, option);
		}
		else
		{
			option = 0;
			cat(structure, cmd, option);
		}
	}
	else if(strcmp(cmd, "cd") == 0)
	{
		cmd = strtok(NULL, " ");
		temp_dir = change_directory(structure->root, structure->current, cmd);
		if (temp_dir != NULL) {
			structure->current = new_dir;
		}
	}
	else if(strcmp(cmd, "exit") == 0)
	{
		exit(0);
	}
	else
	{
		printf("Unknown command: %s\n", cmd);
	}
}*/
