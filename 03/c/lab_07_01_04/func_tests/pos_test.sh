#!/bin/sh

if [ -n "$1" ]; then
  test=$1
else
  echo "Not enough arguments"
  exit 1
fi

test_output=$(printf "func_tests/pos_%02d_out.txt" "${test}")
test_args=$(printf "func_tests/pos_%02d_args.txt" "${test}")
comparator_path="./func_tests/numbers_comparator.py"
args=$(cat "$test_args")
program_output=$(echo "$args" | cut -d' ' -f2)

./app.exe $args
rc=$?
if [ $rc -ne 0 ]; then
  printf "POS TEST %02d FAILED\n" "${test}"
  echo "Return code: ${rc}"
  rm -f "$program_output"
  exit 1
else
  diffs=$(python3 "${comparator_path}" "${test_output}" "${program_output}")
  rc=$?
  rm -f "${program_output}"
  if [ $rc -ne 0 ]; then
    printf "POS TEST %02d FAILED\n" "${test}"
    echo "${diffs}"
    exit 1
  fi
fi

printf "POS TEST %02d PASSED\n" "${test}"
