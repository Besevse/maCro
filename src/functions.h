#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include "parseInput.h"
#include "constants.h"

extern void pressAndRelease(int key);

void executeBash(char *arg){
    system(arg);
}

void executeKeystrokes(char* arg){
    int counter = 0;
    int counterSpace = 0;   
    char temp[MAX_STRING_LENGTH];
    while(1){
        if(arg[counter] == ' '){
            temp[counterSpace+1] = '\0';
            pressAndRelease(atoi(temp));
            counterSpace = 0;
        }
        else {
            temp[counterSpace] = arg[counter];
            counterSpace++;
        }
        if(arg[counter] == '\0'){
            temp[counterSpace+1] = '\0';
            pressAndRelease(atoi(temp));
            break;
        }
        counter++;
    }
}

void execute(char *arg){
    if(arg[0] == 'b'){
        pthread_t thread;
        pthread_create(&thread, NULL, executeBash, (char*)&arg[2]);
    }
    if(arg[0] == 'k'){
        executeKeystrokes(&arg[2]);
    }
    printf("Executing %s\n", arg);
}


float randomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

void sleep_for_float(float seconds) {
    struct timespec req;
    req.tv_sec = (time_t)seconds;  // Get the integer part of the seconds
    req.tv_nsec = (long)((seconds - req.tv_sec) * 1e9);  // Get the fractional part in nanoseconds
    nanosleep(&req, NULL);  // Sleep for the specified time
}

void *timer(char *arg) {
    char sleepTimeChar[MAX_STRING_LENGTH];
    float sleepTime;

    int counter = 0;
    while(arg[counter] != ' '){
        sleepTimeChar[counter] = arg[counter];
        counter++;
    }
    sleepTimeChar[counter+1] = '\0';
    sleepTime = atof(sleepTimeChar);

    printf("sleeptime: %f\n", sleepTime);
    while(1){
        float sleepyTime = sleepTime * randomFloat(1.0, 2.0);
        printf("Sleeping for %f seconds", sleepyTime);
        sleep_for_float(sleepyTime);
        execute(&arg[counter+1]);
    }
}


#endif