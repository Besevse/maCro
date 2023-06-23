#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "constants.h"
#include "initialization.h"
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

extern void pressAndRelease(int key);
float randomFloat(float a, float b);
void sleep_for_float(float seconds);
void executeBash(char *arg);
void executeKeystrokes(char *arg);
void execute(char *arg);
void *timer(char *arg);
#endif
