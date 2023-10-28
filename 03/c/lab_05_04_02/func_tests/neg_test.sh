#!/bin/sh

if [ -n "$1" ]; then
  test=$1
else
  echo "Not enough arguments"
  exit 1
fi

test_args=$(printf "neg_%02d_args.txt" "${test}")
test_input=$(printf "neg_%02d_in.txt" "${test}")

args=$(cat "$test_args")
../app.exe $args <"$test_input" >/dev/null
rc=$?
if [ $rc -eq 0 ]; then
  printf "NEG TEST %02d FAILED\n" "${test}"
  echo "Return code: ${rc}"
  exit 1
fi

printf "NEG TEST %02d PASSED\n" "${test}"
