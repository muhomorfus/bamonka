package main

import (
	"fmt"
	"git.parasha.space/go/libs/benchmark"
	"github.com/icrowley/fake"
	"lab_01/internal/algs"
	dl "lab_01/internal/algs/damerau_levenshtein"
	dlr "lab_01/internal/algs/damerau_levenshtein_recursive"
	dlrc "lab_01/internal/algs/damerau_levenshtein_recursive_cached"
	l "lab_01/internal/algs/levenshtein"
	"log"
	"os"
)

func main() {
	tests := []int{1, 2, 3, 4, 5, 6, 7, 8, 10}
	algorithms := []algs.StringsDiffer{
		l.New(),
		dl.New(),
		dlr.New(),
		dlrc.New(),
	}

	timeFiles := make([]*os.File, len(algorithms))
	memoryFiles := make([]*os.File, len(algorithms))

	for i := range algorithms {
		timeFile, err := os.Create(fmt.Sprintf("data/time_%02d.txt", i))
		if err != nil {
			log.Fatalln(err)
		}
		timeFiles[i] = timeFile

		memoryFile, err := os.Create(fmt.Sprintf("data/memory_%02d.txt", i))
		if err != nil {
			log.Fatalln(err)
		}
		memoryFiles[i] = memoryFile
	}

	for _, n := range tests {
		s1 := fake.CharactersN(n)
		s2 := fake.CharactersN(n)

		for i, a := range algorithms {
			res, _ := benchmark.CheckTime(func() {
				a.Diff(s1, s2)
			}, 100)

			fmt.Fprintln(timeFiles[i], n, res)
			fmt.Fprintln(memoryFiles[i], n, a.MemStat())
		}
	}
}
