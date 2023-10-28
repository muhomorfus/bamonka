#!/bin/sh

gcc -std=c99 -Wall -Wextra -Wpedantic -Wall -Wvla -c main.c
gcc -std=c99 -Wall -Wextra -Wpedantic -Wall -Wvla -c student.c

gcc -o app.exe main.o student.o