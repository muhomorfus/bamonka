#!/bin/sh

gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror main.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror file_processing.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror numbers.c

gcc -o app.exe main.o file_processing.o numbers.o