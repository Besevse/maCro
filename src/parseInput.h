#ifndef PARSEINPUT_H
#define PARSEINPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_SIZE 1024

void parseInput(char* fileName, char timers[MAX_SIZE][MAX_SIZE], char macros[MAX_SIZE][MAX_SIZE], char macroChar[MAX_SIZE][MAX_SIZE], int* timerCounter, int* macroCounter){
    int fdInput;

    fdInput = open(fileName, O_RDONLY);
    if (fdInput == -1) {
        perror("Error opening input file");
        exit(1);
    }

    // read fdInput line by line
    char line[MAX_SIZE];
    int bytes_read;
    int sizeRead = 0;

    // TODO this is very rough and pretty unsafe but it works for now
    while ((bytes_read = read(fdInput, line, MAX_SIZE)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (line[i] == '\n' && sizeRead < i) {
                if(line[sizeRead] == 't' ){
                    strncpy(&timers[*timerCounter], &line[sizeRead + 2],  i-sizeRead-1);
                    timers[*timerCounter][i-sizeRead-1] = '\0';
                    sizeRead = i + 1;
                    *timerCounter = *timerCounter + 1;
                }
                else if(line[sizeRead] == 'm'){
                    strncpy(&macros[*macroCounter], &line[sizeRead + 2],  i-sizeRead-1);
                    macros[*macroCounter][i-sizeRead-1] = '\0';
                    sizeRead = i + 1;
                    *macroCounter = *macroCounter + 1;
                }
            }
        }
    }

    // initialize all macros
    for(int i = 0; i < *macroCounter; i++){
        printf("Initializing macro %s\n", macros[i]);
        int counter = 0;
        while(macros[i][counter] != ' '){
            macroChar[i][counter] = macros[i][counter];
            counter++;
        }
        macroChar[i][counter+1] = '\0';
    }
}

#endif