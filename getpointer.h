#pragma once

#ifndef GETPOINTER_H
#define GETPOINTER_H

#include "system.h"  // dir 구조체를 사용하므로 필요

// 경로 문자열을 따라 디렉토리 포인터 반환
dir* get_pointer(dir* root, dir* cur_pointer, char path[]);

#endif

