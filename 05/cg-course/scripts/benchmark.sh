for i in 0 1 2 5 10 20 30 40 60 80 100 120 140 160 190 230 260 290 320 350 400; do
echo Benchmark at $i threads...
echo -n $i" " >> result.txt
./cg-course -i 50 -o $i.png -t $i -b >> result.txt
done
