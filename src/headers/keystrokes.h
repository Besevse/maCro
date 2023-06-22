#ifndef KEYSTROKES_H
#define KEYSTROKES_H

#include "constants.h"
#include "functions.h"
#include <fcntl.h>
#include <linux/input.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void initializeFd(char *fileName);
int main(int argc, char *argv[]);

#endif /* KEYSTROKES_H */
