#!/bin/sh

cd func_tests || exit 1
./all_test.sh
rc=$?
cd ..
if [ $rc -eq 0 ]
then
  echo "TESTING PASSED"
else
  echo "TESTING FAILED"
  echo "There are ${rc} failed tests"
fi

echo
echo "COVERAGE:"
gcov -f -r main.c
gcov -f -r file_processing.c
gcov -f -r numbers.c
exit $rc

