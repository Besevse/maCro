#include "../headers/parseInput.h"

void parseInput(char *fileName, char timers[MAX][MAX_STRING_LENGTH],
                char macros[MAX][MAX_STRING_LENGTH],
                char macroChar[MAX][MAX_STRING_LENGTH], int *timerCounter,
                int *macroCounter) {
    int fdInput;

    fdInput = open(fileName, O_RDONLY);
    if (fdInput == -1) {
        perror("Error opening input file");
        exit(1);
    }

    // read fdInput line by line
    char line[MAX_STRING_LENGTH];
    int bytes_read;
    int sizeRead = 0;

    // TODO this is very rough and pretty unsafe, but it works for now
    while ((bytes_read = read(fdInput, line, MAX_STRING_LENGTH)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (line[i] == '\n' && sizeRead < i) {
                if (line[sizeRead] == 't') {
                    if (i - sizeRead - 1 > MAX_STRING_LENGTH) {
                        printf("Timer too long, max is %d\n",
                               MAX_STRING_LENGTH);
                        exit(1);
                    }
                    strncpy((char *)&timers[*timerCounter],
                            (char *)&line[sizeRead + 2], i - sizeRead - 1);
                    timers[*timerCounter][i - sizeRead - 1] = '\0';
                    sizeRead = i + 1;
                    *timerCounter = *timerCounter + 1;
                } else if (line[sizeRead] == 'm') {
                    if (i - sizeRead - 1 > MAX_STRING_LENGTH) {
                        printf("Macro too long, max is %d\n",
                               MAX_STRING_LENGTH);
                        exit(1);
                    }
                    strncpy((char *)&macros[*macroCounter],
                            (char *)&line[sizeRead + 2], i - sizeRead - 1);
                    macros[*macroCounter][i - sizeRead - 1] = '\0';
                    sizeRead = i + 1;
                    *macroCounter = *macroCounter + 1;
                }
                if (*macroCounter == MAX || *timerCounter == MAX) {
                    printf("Too many macros or timers, max is %d\n", MAX);
                    exit(1);
                }
            }
        }
    }

    // initialize all macros
    for (int i = 0; i < *macroCounter; i++) {
        printf("Initializing macro %s\n", macros[i]);
        int counter = 0;
        while (macros[i][counter] != ' ') {
            macroChar[i][counter] = macros[i][counter];
            counter++;
        }
        macroChar[i][counter + 1] = '\0';
    }
}
