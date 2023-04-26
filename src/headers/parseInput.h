#ifndef PARSEINPUT_H
#define PARSEINPUT_H

#include "constants.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void parseInput(char *fileName, char timers[MAX][MAX_STRING_LENGTH],
                char macros[MAX][MAX_STRING_LENGTH],
                char macroChar[MAX][MAX_STRING_LENGTH], int *timerCounter,
                int *macroCounter);

#endif