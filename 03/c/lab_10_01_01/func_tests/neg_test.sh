#!/bin/sh

if [ -n "$1" ]; then
  test=$1
else
  echo "Not enough arguments"
  exit 1
fi


test_args=$(printf "neg_%02d_args.txt" "${test}")
program_output=$(printf "neg_%02d_result.txt" "${test}")
args=$(cat "$test_args")

valgrind_code=123

valgrind --quiet --leak-check=full --errors-for-leak-kinds=all --error-exitcode=$valgrind_code --log-file="$(printf "../out/memcheck_neg_%02d.log" "${test}")" ../app.exe $args > "$program_output"

rc=$?

if [ $rc -eq $valgrind_code ]; then
  printf "NEG TEST %02d FAILED (MEMCHECK)\n" "${test}"
  rm -f "$program_output"
  exit 1
elif [ $rc -eq 0 ]; then
  printf "NEG TEST %02d FAILED (CODE)\n" "${test}"
  echo "Return code: ${rc}"
  rm -f "${program_output}"
  exit 1
fi

rm -f "${program_output}"
printf "NEG TEST %02d PASSED\n" "${test}"
