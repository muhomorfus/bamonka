#!/bin/sh

if [ -n $1 ]; then
  test=$1
else
  echo "Not enough arguments"
  exit 1
fi

test_input=$(printf "pos_%02d_in.txt" "${test}")
test_output=$(printf "pos_%02d_out.txt" "${test}")
program_output=$(printf "pos_%02d_out_my.txt" "${test}")
comparator_path="./after_result_comparator.py"

../app.exe <"${test_input}" >"${program_output}"
rc=$?
if [ $rc -ne 0 ]; then
  printf "POS TEST %02d FAILED\n" "${test}"
  echo "Return code: ${rc}"
  rm "$program_output"
  exit 1
else
  diffs=$(python "${comparator_path}" "${test_output}" "${program_output}")
  rc=$?
  rm "${program_output}"
  if [ $rc -ne 0 ]; then
    printf "POS TEST %02d FAILED\n" "${test}"
    echo "${diffs}"
    exit 1
  fi
fi

printf "POS TEST %02d PASSED\n" "${test}"
