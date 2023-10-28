#!/bin/bash

alphas="0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0"
ks="0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0"
times="2 3 4"
n=$1

cd ..

echo -e "0, 0, 0, 0, 0" > data/param_$n.txt
echo -e "0, 0, 0, 0, 0" > data/param_best_$n.txt

for alpha in $alphas; do
  for k in $ks; do
    for time in $times; do
      go run ./cmd/lab_07 -filename=data/$n.txt -alpha=$alpha -rho=$k -t=$time -filename_csv=data/param_$n.txt -filename_csv_best=data/param_best_$n.txt -out=false
    done
  done
done