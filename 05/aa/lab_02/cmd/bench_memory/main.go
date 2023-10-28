package main

import (
	"fmt"
	"lab_02/internal/algs"
	"lab_02/internal/algs/usual"
	"lab_02/internal/algs/winograd"
	"lab_02/internal/algs/winograd_improved"
	"lab_02/internal/utils"
	"log"
	"os"
)

func main() {
	tests := []int{1, 2, 3, 4, 5, 6, 7, 8, 10, 13, 16, 20, 30, 40, 50, 60}
	algorithms := []algs.Multiplier{
		usual.New(),
		winograd.New(),
		winograd_improved.New(),
	}

	memoryFiles := make([]*os.File, len(algorithms))

	for i := range algorithms {
		memoryFile, err := os.Create(fmt.Sprintf("data/memory_%02d.txt", i))
		if err != nil {
			log.Fatalln(err)
		}
		memoryFiles[i] = memoryFile
	}

	for _, n := range tests {
		a := utils.GenMatrix(n)
		b := utils.GenMatrix(n)

		for i, alg := range algorithms {
			alg.Multiply(a, b)
			fmt.Fprintln(memoryFiles[i], n, alg.MemStat())
		}
	}
}
