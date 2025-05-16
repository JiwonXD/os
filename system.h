#pragma once

#ifndef SYSTEM_H
#define SYSTEM_H

#include <time.h>

typedef struct file file;
typedef struct dir dir;
typedef struct tree tree;
typedef struct stack stack;

struct file {
    char name[20];
    char data[1024];
    int permission;
    struct tm* time;
    struct file* right;
    char user[10];
};

struct dir {
    char name[20];
    int permission;
    file* file_inside;
    struct tm* time;
    struct dir* parent;
    struct dir* left;
    struct dir* right;
    char user[10];
};

struct tree {
    dir* root;
    dir* current;
};

struct stack {
	int top;
	char* datas[1024];
};


dir* new_root();
tree* new_tree(dir* rootnode);
file* new_file(char* name);
dir* new_dir(char* name);
stack* new_stack();

int check_stack_empty(stack* newstack);
void push(stack* newstack, char* input);
void pop(stack* newstack);

#endif

