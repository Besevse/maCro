#ifndef KEYSTROKESX11_H
#define KEYSTROKESX11_H

#include "constants.h"
#include "functions.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <fcntl.h>
#include <linux/input-event-codes.h>
#include <linux/input.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void initializeFd(char *fileName);
int main(int argc, char *argv[]);

#endif /* KEYSTROKESX11_H */
