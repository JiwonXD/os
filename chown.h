#pragma once

#ifndef CHOWN_H
#define CHOWN_H

#include "system.h"

// chown 명령어 실행 함수
void chownCommand(dir* current, const char* path, const char* new_owner);

#endif
