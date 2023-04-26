#include "../headers/keystrokesx11.h"

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

int main(int argc, char *argv[]) {
    char timers[MAX][MAX_STRING_LENGTH];
    char macros[MAX][MAX_STRING_LENGTH];
    char macroChar[MAX][MAX_STRING_LENGTH];
    int timerCounter = 0;
    int macroCounter = 0;

    initializeDisplay();
    parseInput(argv[1], timers, macros, macroChar, &timerCounter,
               &macroCounter);

    sleep(0.1);

    pthread_t threads[timerCounter];
    // start all timers
    for (int i = 0; i < timerCounter; i++) {
        printf("Starting timer %s\n", timers[i]);
        pthread_t thread;
        pthread_create(&thread, NULL, timer, &timers[i]);
        threads[i] = thread;
    }

    Window root = DefaultRootWindow(display);
    Window focused;
    int revert;

    XGetInputFocus(display, &focused, &revert);
    XSelectInput(display, focused,
                 KeyPressMask | KeyReleaseMask | FocusChangeMask);

    while (1) {
        XEvent ev;
        XNextEvent(display, &ev);
        switch (ev.type) {
        case FocusOut:
            printf("switching focus\n");
            if (focused != root)
                XSelectInput(display, focused, 0);
            XGetInputFocus(display, &focused, &revert);
            if (focused == PointerRoot)
                focused = root;
            XSelectInput(display, focused,
                         KeyPressMask | KeyReleaseMask | FocusChangeMask);
            break;

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
    XCloseDisplay(display);
    return 0;
}