#!/bin/sh

gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror --coverage -g3 main.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror --coverage -g3 process.c

gcc -o app.exe --coverage -g3 main.o process.o