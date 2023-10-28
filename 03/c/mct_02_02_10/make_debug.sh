#!/bin/sh

gcc -std=c99 -Wall -Wextra -Wpedantic -Wall -Wvla -g3 --coverage -c main.c
gcc -std=c99 -Wall -Wextra -Wpedantic -Wall -Wvla -g3 --coverage -c student.c

gcc -o app.exe -g3 --coverage main.o student.o