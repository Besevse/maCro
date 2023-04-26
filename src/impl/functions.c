#include "../headers/functions.h"

extern void pressAndRelease(int key);

float randomFloat(float a, float b) {
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

void sleep_for_float(float seconds) {
    struct timespec time;
    time.tv_sec = (time_t)seconds;
    time.tv_nsec = (long)((seconds - time.tv_sec) * 1e9);
    nanosleep(&time, NULL);
}

void executeBash(char *arg) { system(arg); }

void executeKeystrokes(char *arg) {
    int counter = 0;
    int counterSpace = 0;
    char temp[MAX_STRING_LENGTH];
    while (1) {
        // TODO: MAKE THIS VARIABLE
        float rf = randomFloat(0.02, 0.07);
        sleep_for_float(rf);

        if (arg[counter] == ' ') {
            temp[counterSpace + 1] = '\0';
            pressAndRelease(atoi(temp));
            counterSpace = 0;
        } else {
            temp[counterSpace] = arg[counter];
            counterSpace++;
        }
        if (arg[counter] == '\0') {
            temp[counterSpace + 1] = '\0';
            pressAndRelease(atoi(temp));
            break;
        }
        counter++;
    }
}

void execute(char *arg) {
    if (arg[0] == 'b') {
        pthread_t thread;
        pthread_create(&thread, NULL, executeBash, (char *)&arg[2]);
    }
    if (arg[0] == 'k') {
        pthread_t thread;
        pthread_create(&thread, NULL, executeKeystrokes, (char *)&arg[2]);
    }
    printf("Executing %s\n", arg);
}

void *timer(char *arg) {
    char sleepTimeChar[MAX_STRING_LENGTH];
    float sleepTime;

    int counter = 0;
    while (arg[counter] != ' ') {
        sleepTimeChar[counter] = arg[counter];
        counter++;
    }
    sleepTimeChar[counter + 1] = '\0';
    sleepTime = atof(sleepTimeChar);

    while (1) {
        // TODO: MAKE THIS VARIABLE
        sleepTime *= randomFloat(1.0, 1.3);
        sleep_for_float(sleepTime);
        execute(&arg[counter + 1]);
    }
}
