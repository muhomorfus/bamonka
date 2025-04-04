package main

import (
	"fmt"
	"git.parasha.space/go/libs/benchmark"
	"lab_02/internal/algs"
	"lab_02/internal/algs/usual"
	"lab_02/internal/algs/winograd"
	"lab_02/internal/algs/winograd_improved"
	"lab_02/internal/utils"
	"log"
	"os"
)

func main() {
	tests := []int{1, 3, 5, 7, 9, 13, 15, 19, 29, 39, 49, 59}
	algorithms := []algs.Multiplier{
		usual.New(),
		winograd.New(),
		winograd_improved.New(),
	}

	timeFiles := make([]*os.File, len(algorithms))

	for i := range algorithms {
		timeFile, err := os.Create(fmt.Sprintf("data/time_worst_%02d.txt", i))
		if err != nil {
			log.Fatalln(err)
		}
		timeFiles[i] = timeFile
	}

	for _, n := range tests {
		a := utils.GenMatrix(n)
		b := utils.GenMatrix(n)

		for i, alg := range algorithms {
			res, _ := benchmark.CheckTime(func() {
				alg.Multiply(a, b)
			}, 10000)

			fmt.Fprintln(timeFiles[i], n, res)
		}
	}
}
