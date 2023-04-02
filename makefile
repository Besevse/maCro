CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -lX11 -lXtst

build: src/keystrokes.c src/keystrokesx11.c src/parseInput.h
	mkdir -p build
	$(CC) $(CFLAGS) -o build/keystrokesx11 src/keystrokesx11.c
	$(CC) $(CFLAGS) -o build/keystrokes src/keystrokes.c

run: build/keystrokes build/keystrokesx11
	sudo ./build/keystrokes input.txt

runx11: build/keystrokesx11
	sudo ./build/keystrokesx11 input.txt

clean:
	rm -rf build/*
