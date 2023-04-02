#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/input-event-codes.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include "functions.h"

Display *display;

void initializeDisplay(){
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        perror("Cannot open display\n");
        exit(1);
    }
}

void pressAndRelease(int button)
{
    printf("Pressing and releasing %d\n", button);


    int major_opcode, first_event, first_error;

    if (!XQueryExtension(display, "XTEST", &major_opcode, &first_event, &first_error)) {
        perror("XTEST extension is not available\n");
        exit(1);
    }

    XTestFakeKeyEvent(display, button, True, 0);
    XTestFakeKeyEvent(display, button, False, 0);

    XFlush(display);
}


int main(int argc, char *argv[]){
    struct input_event out;
    char timers[MAX_SIZE][MAX_SIZE];
    char macros[MAX_SIZE][MAX_SIZE];
    char macroChar[MAX_SIZE][MAX_SIZE];
    int timerCounter = 0;
    int macroCounter = 0;


    parseInput(argv[1], timers, macros, macroChar, &timerCounter, &macroCounter);
    initializeDisplay();

    printf("%s", timers[0]);

    sleep(0.1);


    pthread_t threads[timerCounter];
    // start all timers
    for(int i = 0; i < timerCounter; i++){
        printf("Starting timer %s\n", timers[i]);
        pthread_t thread;
        pthread_create(&thread, NULL, timer, timers[i]);
        threads[i] = thread;
    }

    Window root = DefaultRootWindow(display);
    Window focused;
    KeySym ks;
    XComposeStatus comp;
    int revert;

    XGetInputFocus (display, &focused, &revert);
    XSelectInput(display, focused, KeyPressMask|KeyReleaseMask|FocusChangeMask);

    while (1){
        XEvent ev;
        XNextEvent(display, &ev);
        switch (ev.type){
            case FocusOut:
                printf("switching focus\n");
                if (focused != root)
                    XSelectInput(display, focused, 0);
                XGetInputFocus (display, &focused, &revert);
                if (focused == PointerRoot)
                    focused = root;
                XSelectInput(display, focused, KeyPressMask|KeyReleaseMask|FocusChangeMask);
                break;

            case KeyPress:
                printf("Keypress detected: %d\n", ev.xkey.keycode);
                for(int i = 0; i < macroCounter; i++){
                    printf("Comparing %d to %d\n", ev.xkey.keycode, atoi(macroChar[i]));
                    if(ev.xkey.keycode == atoi(macroChar[i])){
                        execute(&macros[i][strlen(macroChar[i])+1]);
                    }
                }
                fflush(stdout);
                break;
        }
    }
}