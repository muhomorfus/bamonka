package main

import (
	"fmt"
	"git.parasha.space/go/libs/benchmark"
	"lab_03/internal/algs"
	"lab_03/internal/algs/bucket"
	"lab_03/internal/algs/count"
	"lab_03/internal/algs/heap"
	"lab_03/internal/utils"
	"log"
	"os"
	"strings"
)

func main() {
	tests := []int{1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000}
	algorithms := []algs.Sorter{
		bucket.New(10),
		heap.New(),
		count.New(),
	}

	timeFiles := make([]*os.File, len(algorithms))

	for i := range algorithms {
		timeFile, err := os.Create(fmt.Sprintf("data/time_best_%02d.txt", i))
		if err != nil {
			log.Fatalln(err)
		}
		timeFiles[i] = timeFile
	}

	latexFile, err := os.Create("data/latex_time_best.txt")
	if err != nil {
		log.Fatalln(err)
	}

	for _, n := range tests {
		arrs := [][]int{
			utils.GenerateSortedArray(n),
			utils.GenerateEqualsArray(n, 0),
			utils.GenerateEqualsArray(n, 0),
		}

		row := []string{fmt.Sprint(n)}

		for i, alg := range algorithms {
			res, _ := benchmark.CheckTime(func() {
				alg.Sort(arrs[i])
			}, 100)

			fmt.Fprintln(timeFiles[i], n, res)
			row = append(row, fmt.Sprint(res))
		}

		fmt.Fprintf(latexFile, "%s \\\\ \\hline \n", strings.Join(row, " & "))
	}
}
