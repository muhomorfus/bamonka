#!/bin/sh

echo

failed_tests=0
pos_test_num=1
while [ -f "$(printf 'pos_%02d_in.txt' ${pos_test_num})" ]; do
  cd shared || exit 1
  for bin_file in data_*.bin; do
    if ! cmp "$bin_file" "copy_$bin_file" -s; then
      cp "$bin_file" "copy_$bin_file"
    fi
  done
  cd ..
  ./pos_test.sh "${pos_test_num}"
  failed_tests=$((failed_tests + $?))
  pos_test_num=$((pos_test_num + 1))
done

neg_test_num=1
while [ -f "$(printf 'neg_%02d_in.txt' ${neg_test_num})" ]; do
  cd shared || exit 1
  for bin_file in data_*.bin; do
    if ! cmp "$bin_file" "copy_$bin_file" -s; then
      cp "$bin_file" "copy_$bin_file"
    fi
  done
  cd ..
  ./neg_test.sh "${neg_test_num}"
  failed_tests=$((failed_tests + $?))
  neg_test_num=$((neg_test_num + 1))
done

cd shared || exit 1
rm copy_*.bin
cd ..

exit $failed_tests
