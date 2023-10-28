#!/bin/sh

if [ -n "$1" ]; then
  test=$1
else
  echo "Not enough arguments"
  exit 1
fi

test_input=$(printf "neg_%02d_in.txt" "${test}")
program_output=$(printf "neg_%02d_out_my.txt" "${test}")

../app.exe <"${test_input}" >"${program_output}"
rc=$?
if [ $rc -eq 0 ]; then
  printf "NEG TEST %02d FAILED\n" "${test}"
  echo "Return code: ${rc}"
  rm "${program_output}"
  exit 1
fi

rm "${program_output}"
printf "NEG TEST %02d PASSED\n" "${test}"
