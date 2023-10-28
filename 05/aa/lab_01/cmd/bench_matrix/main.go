package main

import (
	"fmt"
	"git.parasha.space/go/libs/benchmark"
	"github.com/icrowley/fake"
	"lab_01/internal/algs"
	dl "lab_01/internal/algs/damerau_levenshtein"
	l "lab_01/internal/algs/levenshtein"
	"log"
	"os"
)

func main() {
	tests := []int{75, 80, 85, 90, 95, 100}
	algorithms := []algs.StringsDiffer{
		l.New(),
		dl.New(),
	}

	timeFiles := make([]*os.File, len(algorithms))

	for i := range algorithms {
		timeFile, err := os.Create(fmt.Sprintf("data/time_matrix_%02d.txt", i))
		if err != nil {
			log.Fatalln(err)
		}
		timeFiles[i] = timeFile
	}

	for _, n := range tests {
		s1 := fake.CharactersN(n)
		s2 := fake.CharactersN(n)

		for i, a := range algorithms {
			res, _ := benchmark.CheckTime(func() {
				a.Diff(s1, s2)
			}, 1000)

			fmt.Fprintln(timeFiles[i], n, res)
		}
	}
}
