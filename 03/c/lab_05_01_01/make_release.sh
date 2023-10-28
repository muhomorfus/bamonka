#!/bin/sh

gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror main.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror process.c

gcc -o app.exe main.o process.o