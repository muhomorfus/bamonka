#!/bin/sh

if [ -n "$1" ]; then
  test=$1
else
  echo "Not enough arguments"
  exit 1
fi

test_args=$(printf "pos_%02d_args.txt" "${test}")
args=$(cat "$test_args")
mode=$(echo "$args" | cut -d' ' -f1)

if [ "$mode" = "sb" ]; then
  test_output_bin=$(printf "shared/pos_%02d_out.bin" "${test}")
  program_output_bin=$(echo "$args" | cut -d' ' -f3)

  ../app.exe $args
  rc=$?
  if [ $rc -ne 0 ]; then
    printf "POS TEST %02d FAILED\n" "${test}"
    echo "Return code: ${rc}"
    rm "$program_output" 2> /dev/null
    exit 1
  else
    diffs=$(cmp "${test_output_bin}" "${program_output_bin}")
    rc=$?
    if [ $rc -ne 0 ]; then
      printf "POS TEST %02d FAILED\n" "${test}"
      echo "${diffs}"
      exit 1
    fi
  fi
elif [ "$mode" = "ab" ]; then
  test_input=$(printf "pos_%02d_in.txt" "${test}")
  test_output_bin=$(printf "shared/pos_%02d_out.bin" "${test}")
  program_output_bin=$(echo "$args" | cut -d' ' -f2)

  ../app.exe $args <"$test_input"
  rc=$?
  if [ $rc -ne 0 ]; then
    printf "POS TEST %02d FAILED\n" "${test}"
    echo "Return code: ${rc}"
    rm "$program_output" 2> /dev/null
    exit 1
  else
    diffs=$(cmp "${test_output_bin}" "${program_output_bin}")
    rc=$?
    if [ $rc -ne 0 ]; then
      printf "POS TEST %02d FAILED\n" "${test}"
      echo "${diffs}"
      exit 1
    fi
  fi
else
  test_output=$(printf "pos_%02d_out.txt" "${test}")
  program_output=$(printf "pos_%02d_out_my.txt" "${test}")
  comparator_path="./string_comparator.py"

  ../app.exe $args >"${program_output}"
  rc=$?
  if [ $rc -ne 0 ]; then
    printf "POS TEST %02d FAILED\n" "${test}"
    echo "Return code: ${rc}"
    rm "${program_output}" 2> /dev/null
    exit 1
  else
    diffs=$(python "${comparator_path}" "${test_output}" "${program_output}")
    rc=$?
    rm "${program_output}" 2> /dev/null
    if [ $rc -ne 0 ]; then
      printf "POS TEST %02d FAILED\n" "${test}"
      echo "${diffs}"
      exit 1
    fi
  fi
fi

printf "POS TEST %02d PASSED\n" "${test}"
