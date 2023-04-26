CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99
X11FLAGS=-lX11 -lXtst

buildall: build build_x11 build_functions build_parse_input

build: src/headers/keystrokes.h src/impl/keystrokes.c
	mkdir -p build
	$(CC) $(CFLAGS) -c -o build/keystrokes.o src/impl/keystrokes.c

build_x11: src/headers/keystrokesx11.h src/impl/keystrokesx11.c
	mkdir -p build
	$(CC) $(CFLAGS) -c -o build/keystrokesx11.o src/impl/keystrokesx11.c

build_functions: src/headers/functions.h src/impl/functions.c
	mkdir -p build
	$(CC) $(CFLAGS) -c -o build/functions.o src/impl/functions.c

build_parse_input: src/headers/parseInput.h src/impl/parseInput.c
	mkdir -p build
	$(CC) $(CFLAGS) -c -o build/parseInput.o src/impl/parseInput.c
	
linkx11:
	mkdir -p build
	$(CC) build/keystrokesx11.o build/functions.o build/parseInput.o -o build/main $(CFLAGS) $(X11FLAGS)
	
link:
	mkdir -p build
	$(CC) build/keystrokes.o build/functions.o build/parseInput.o -o build/main $(CFLAGS)

run:
	sudo ./build/main input.txt

clean:
	rm -rf build/*
