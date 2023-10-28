#!/bin/sh

gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror main.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror file_processing.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror product.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror string_functions.c

gcc -o app.exe main.o file_processing.o product.o string_functions.o