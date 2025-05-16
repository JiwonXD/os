#ifndef GREP_H
#define GREP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "system.h"  // file, tree 구조체 포함

file* exitfile(tree* t, char* name);
void grep(tree* t, char* pattern, char* filename);

#endif

