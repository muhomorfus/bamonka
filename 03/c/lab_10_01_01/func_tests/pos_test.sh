#!/bin/sh

if [ -n "$1" ]; then
  test=$1
else
  echo "Not enough arguments"
  exit 1
fi

test_args=$(printf "pos_%02d_args.txt" "${test}")
test_output=$(printf "pos_%02d_out.txt" "${test}")
program_output=$(printf "pos_%02d_result.txt" "${test}")
comparator_path="./string_comparator.py"
args=$(cat "$test_args")

valgrind_code=123

valgrind --quiet --leak-check=full --errors-for-leak-kinds=all --error-exitcode=$valgrind_code --log-file="$(printf "../out/memcheck_pos_%02d.log" "${test}")" ../app.exe $args > "$program_output"
rc=$?

if [ $rc -eq $valgrind_code ]; then
  printf "POS TEST %02d FAILED (MEMCHECK)\n" "${test}"
  rm "$program_output"
  exit 1
elif [ $rc -ne 0 ]; then
  printf "POS TEST %02d FAILED (CODE)\n" "${test}"
  echo "Return code: ${rc}"
  rm -f "$program_output"
  exit 1
else
  diffs=$(python3 "${comparator_path}" "${test_output}" "${program_output}")
  rc=$?
  rm -f "${program_output}"
  if [ $rc -ne 0 ]; then
    printf "POS TEST %02d FAILED (RESULT)\n" "${test}"
    echo "${diffs}"
    exit 1
  fi
fi

printf "POS TEST %02d PASSED\n" "${test}"
