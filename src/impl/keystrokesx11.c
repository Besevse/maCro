#include "../headers/keystrokesx11.h"
#include <time.h>

Display *display;

void initializeDisplay() {
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        perror("Cannot open display\n");
        exit(1);
    }
}

void pressAndRelease(int key) {
    printf("Pressing and releasing %d\n", key);

    int major_opcode, first_event, first_error;

    if (!XQueryExtension(display, "XTEST", &major_opcode, &first_event,
                         &first_error)) {
        perror("XTEST extension is not available\n");
        exit(1);
    }

    XTestFakeKeyEvent(display, key, True, 0);
    XTestFakeKeyEvent(display, key, False, 0);

    XFlush(display);
}

int xerrorhandler(Display *display, XErrorEvent *error) {
    printf("Caught an X error: %d\n", error->error_code);
}

int main(int argc, char *argv[]) {
    XInitThreads();

    char timers[MAX][MAX_STRING_LENGTH];
    char macros[MAX][MAX_STRING_LENGTH];
    char macroChar[MAX][MAX_STRING_LENGTH];
    int timerCounter = 0;
    int macroCounter = 0;

    initializeDisplay();
    XSetErrorHandler(xerrorhandler);

    parseInput(argv[1], timers, macros, &timerCounter,
               &macroCounter);

    sleep(1);

    pthread_t threads[timerCounter];
    initializeMacros(macros, macroChar, &macroCounter);
    initializeThreads(threads, timers, &timerCounter);

    Window root = DefaultRootWindow(display);
    Window focused;
    int revert;

    while (1) {
        XGetInputFocus(display, &focused, &revert);
        XSelectInput(display, focused,
                     KeyPressMask | KeyReleaseMask | FocusChangeMask);
        XEvent ev;
        XNextEvent(display, &ev);
        switch (ev.type) {
            case KeyPress:
                printf("Keypress detected: %d\n", ev.xkey.keycode);
                for (int i = 0; i < macroCounter; i++) {
                    printf("Comparing %d to %d\n", ev.xkey.keycode,
                           atoi(macroChar[i]));
                    if (ev.xkey.keycode == (unsigned int)atoi(macroChar[i])) {
                        execute(&macros[i][strlen(macroChar[i]) + 1]);
                    } else if (ev.xkey.keycode == 34)
                        goto finish;
                }
                fflush(stdout);
                break;
            }
    }
    finish:
    for(int i = 0; i < sizeof(threads) / sizeof(pthread_t); i++) {
        pthread_cancel(threads[i]);
    }
    XCloseDisplay(display);
    return 0;
}
