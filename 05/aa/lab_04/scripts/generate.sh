n=50

first=$(($n * $1))
last=$(($first + $n - 1))

for i in $(seq $first $last); do
./cg-course -i $i -o $i.png -t 192
done
