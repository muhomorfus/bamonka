#!/bin/sh

gcc -std=c99 -Wall -Wpedantic -Wextra -Wvla -Werror -c set.c -g3 --coverage
gcc -std=c99 -Wall -Wpedantic -Wextra -Wvla -Werror -c test_set.c -g3
gcc -std=c99 -Wall -Wpedantic -Wextra -Wvla -Werror -c unit_tests.c -g3

gcc -o app.exe set.o test_set.o unit_tests.o -g3 --coverage

./app.exe
rc=$?

if [ $rc -ne 0 ]; then
  echo "TESTING FAILED"
  echo "THERE ARE $rc FAILED TESTS"
else
  echo "TESTING PASSED"
fi

echo "COVERAGE"
gcov -f -r set.c