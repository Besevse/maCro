This is a program that automates keyboard inputs and bash commands on linux, there's two version - with and without x11 library
It supports both macros(happens when keypressed) and timers(happens every x amount of time).

Find ur input device using evtest tool.

Inside input.txt there's an example of two macros and 1 timer but in general you define:

1 2 3 4

1 = "m" for macro or "t" for timer

2 = keypress which to listen to for macro or x amount of seconds to wait for timer

3 = "b" for bash command or "k" for keystrokes

4 = bash command or keystrokes seperated by ' ' you want to execute


How to use:
```
$ make buildall
$ make link
$ make run
```
for the x11 version do

```
$ make buildall
$ make linkx11
$ make run
```

THIS IS A WIP!
