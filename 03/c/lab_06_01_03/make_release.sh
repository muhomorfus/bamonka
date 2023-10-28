#!/bin/sh

gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror main.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror short_string.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror error.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror product.c
gcc -o app.exe main.o short_string.o error.o product.o