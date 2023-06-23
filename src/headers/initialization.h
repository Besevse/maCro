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
#include <pthread.h>
#include "functions.h"



void parseInput(char *fileName, char timers[MAX][MAX_STRING_LENGTH],
                char macros[MAX][MAX_STRING_LENGTH], int *timerCounter,
                int *macroCounter);

void initializeMacros(char macros[MAX][MAX_STRING_LENGTH],
                      char macroChar[MAX][MAX_STRING_LENGTH],
                      const int *macroCounter);

void initializeThreads(pthread_t threads[], char timers[MAX][MAX_STRING_LENGTH], const int *timerCounter);

#endif
