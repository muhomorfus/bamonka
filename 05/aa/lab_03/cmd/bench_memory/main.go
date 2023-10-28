package main

import (
	"fmt"
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

	memoryFiles := make([]*os.File, len(algorithms))

	for i := range algorithms {
		memoryFile, err := os.Create(fmt.Sprintf("data/memory_%02d.txt", i))
		if err != nil {
			log.Fatalln(err)
		}
		memoryFiles[i] = memoryFile
	}

	latexFile, err := os.Create("data/latex_memory.txt")
	if err != nil {
		log.Fatalln(err)
	}

	for _, n := range tests {
		a := utils.GenerateRandomArray(n, 1000)

		row := []string{fmt.Sprint(n)}

		for i, alg := range algorithms {
			alg.Sort(a)
			fmt.Fprintln(memoryFiles[i], n, alg.MemStat())
			row = append(row, fmt.Sprint(alg.MemStat()))
		}

		fmt.Fprintf(latexFile, "%s \\\\ \\hline \n", strings.Join(row, " & "))
	}
}
