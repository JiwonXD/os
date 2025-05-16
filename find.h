#ifndef FIND_H
#define FIND_H

#include <stdio.h>
#include <pthread.h>
#include "system.h"  // dir, tree, file 구조체 포함

void find(tree* t, char* path);
void find_file(tree* t, char* path);

#endif

