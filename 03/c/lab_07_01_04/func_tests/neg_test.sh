#!/bin/sh

if [ -n "$1" ]; then
  test=$1
else
  echo "Not enough arguments"
  exit 1
fi

test_args=$(printf "func_tests/neg_%02d_args.txt" "${test}")
args=$(cat "$test_args")
program_output=$(echo "$args" | cut -d' ' -f2)

./app.exe $args
rc=$?
if [ $rc -eq 0 ]; then
  printf "NEG TEST %02d FAILED\n" "${test}"
  echo "Return code: ${rc}"
  rm -f "${program_output}"
  exit 1
fi

rm -f "${program_output}"
printf "NEG TEST %02d PASSED\n" "${test}"
