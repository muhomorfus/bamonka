#!/bin/sh

gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror --coverage -g3 main.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror --coverage -g3 file_processing.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror --coverage -g3 numbers.c

gcc -o app.exe --coverage -g3 main.o file_processing.o numbers.o