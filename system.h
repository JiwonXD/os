#pragma once

#ifndef SYSTEM_H
#define SYSTEM_H

#include <time.h>

typedef struct file file;
typedef struct dir dir;
typedef struct tree tree;

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

dir* new_root();
tree* new_tree(dir* rootnode);
file* new_file(char* name);
dir* new_dir(char* name);

#endif

