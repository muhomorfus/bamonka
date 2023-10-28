#!/bin/sh

gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror --coverage main.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror --coverage my_string_functions.c
gcc -c -Wall -Wpedantic -Wextra -Wvla -Werror --coverage test_my_string_functions.c
gcc -o app.exe --coverage main.o my_string_functions.o test_my_string_functions.o

./app.exe
rc=$?
echo "Return code: ${rc}"
echo

echo "GCov info: "
gcov -f -r my_string_functions.c