#!/bin/sh

failed_tests=0
pos_test_num=1

while [ -f "$(printf 'func_tests/pos_%02d_in.txt' ${pos_test_num})" ]; do
  ./func_tests/pos_test.sh "${pos_test_num}"
  failed_tests=$((failed_tests + $?))
  pos_test_num=$((pos_test_num + 1))
done

neg_test_num=1
while [ -f "$(printf 'func_tests/neg_%02d_in.txt' ${neg_test_num})" ]; do
  ./func_tests/neg_test.sh "${neg_test_num}"
  failed_tests=$((failed_tests + $?))
  neg_test_num=$((neg_test_num + 1))
done

if [ $failed_tests -eq 0 ]
then
  echo "TESTING PASSED"
else
  echo "TESTING FAILED"
  echo "There are ${failed_tests} failed tests"
fi
