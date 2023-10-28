#!/bin/sh

gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror --coverage -g3 main.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror --coverage -g3 short_string.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror --coverage -g3 date.c
gcc -o app.exe --coverage -g3 main.o short_string.o date.o