#!/bin/sh

if [ -n "$1" ]; then
  test=$1
else
  echo "Not enough arguments"
  exit 1
fi

test_args=$(printf "pos_%02d_args.txt" "${test}")
args=$(cat "$test_args")

case "${args}" in
s*)
  test_output_bin=$(printf "shared/pos_%02d_out.bin" "${test}")
  program_output_bin=$(echo "${args}" | sed 's/s //g')

  ../app.exe $args
  rc=$?
  if [ $rc -ne 0 ]; then
    printf "POS TEST %02d FAILED\n" "${test}"
    echo "Return code: ${rc}"
    rm "$program_output"
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
  ;;
*)
  test_output=$(printf "pos_%02d_out.txt" "${test}")
  program_output=$(printf "pos_%02d_out_my.txt" "${test}")
  comparator_path="./numbers_comparator.py"

  ../app.exe $args >"${program_output}"
  rc=$?
  if [ $rc -ne 0 ]; then
    printf "POS TEST %02d FAILED\n" "${test}"
    echo "Return code: ${rc}"
    rm "${program_output}"
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
  ;;
esac

printf "POS TEST %02d PASSED\n" "${test}"
