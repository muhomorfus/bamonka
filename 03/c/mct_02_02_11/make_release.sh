#!/bin/sh

gcc -std=c99 -Wall -Wextra -Wpedantic -Wall -Wvla -c main.c
gcc -std=c99 -Wall -Wextra -Wpedantic -Wall -Wvla -c product.c

gcc -o app.exe main.o product.o