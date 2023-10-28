#!/bin/sh

gcc -std=c99 -Wall -Wextra -Wpedantic -Wall -Wvla -c main.c
gcc -std=c99 -Wall -Wextra -Wpedantic -Wall -Wvla -c my_string.c

gcc -o app.exe main.o my_string.o