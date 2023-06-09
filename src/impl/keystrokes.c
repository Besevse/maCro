#include "../headers/keystrokes.h"

int fd = -1;

void initializeFd(char *fileName) {
    fd = open(fileName, O_RDWR | O_NONBLOCK);
    if (fd == -1) {
        perror("Error opening file");
        exit(1);
    }
}

void pressAndRelease(int key) {
    printf("Pressing and releasing %d\n", key);
    struct input_event in;
    in.type = EV_KEY;
    in.value = 1;

    in.code = key;
    write(fd, &in, sizeof(in));
    in.value = 0;
    sleep(0.1);
    write(fd, &in, sizeof(in));
}

int main(int argc, char *argv[]) {
    // TODO: MAKE THIS VARIABLE
    char *deviceName = "/dev/input/event2";
    struct input_event out;
    char timers[MAX][MAX_STRING_LENGTH];
    char macros[MAX][MAX_STRING_LENGTH];
    char macroChar[MAX][MAX_STRING_LENGTH];
    int timerCounter = 0;
    int macroCounter = 0;


    initializeFd(deviceName);
    parseInput(argv[1], timers, macros, &timerCounter,
               &macroCounter);

    sleep(1);

    pthread_t threads[timerCounter];

    initializeMacros(macros, macroChar, &macroCounter);
    initializeThreads(threads, timers, &timerCounter);

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    // main loop
    while (1) {
        int ret = select(fd + 1, &rfds, NULL, NULL, NULL);
        if (ret == -1) {
            perror("Error in select");
            exit(1);
        } else if (ret > 0 && FD_ISSET(fd, &rfds)) {
            fsync(fd);
            if (read(fd, &out, sizeof(struct input_event)) == -1) {
                perror("Error reading file");
                exit(1);
            }

            if (out.type == EV_KEY && out.value == 1) {
                printf("Keypress detected: %d\n", out.code);
                for (int i = 0; i < macroCounter; i++) {
                    printf("Comparing %d to %d\n", out.code,
                           atoi(macroChar[i]));
                    if (out.code == atoi(macroChar[i])) {
                        execute(&macros[i][strlen(macroChar[i]) + 1]);
                    }
                }
                fflush(stdout);
            }
        }
    }
    // TODO EXIT THIS
    for(int i = 0; i < sizeof(threads) / sizeof(pthread_t); i++) {
        pthread_cancel(threads[i]);
    }
    close(fd);
    return 0;
}