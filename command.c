#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "system.h"
#include "getpointer.h"
#include "chown.h"
#include "ls.h"
#include "grep.h"
#include "find.h"
#include "pwd.h"
#include "cat.h"
#include "cd.h"

// 전역 파일 시스템 구조체
extern tree* structure;
extern stack* pwdstack;

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
		run_cat(cmd, filename);
	}
	else if(strcmp(cmd, "cd") == 0)
	{
		cmd = strtok(NULL, " ");
		run_cd(cmd);
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
