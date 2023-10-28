package main

import (
	"fmt"
	"git.parasha.space/go/libs/benchmark"
	"lab_07/internal/graph"
	"lab_07/internal/path/ant"
	"lab_07/internal/path/bruteforce"
	"log"
	"os"
	"strings"
)

func main() {
	tests := []int{2, 3, 4, 5, 6, 7, 8, 9, 10}

	timeFile0, err := os.Create("data/time_00.txt")
	if err != nil {
		log.Fatalln(err)
	}

	timeFile1, err := os.Create("data/time_01.txt")
	if err != nil {
		log.Fatalln(err)
	}

	mb := bruteforce.New()
	ma := ant.New(10, 0.7, 0.3, 0.2)

	for _, n := range tests {
		g, err := graph.FromFile(fmt.Sprintf("data/%d.txt", n))
		if err != nil {
			fmt.Println(err)
			os.Exit(1)
		}

		row := []string{fmt.Sprint(n)}

		resbf, _ := benchmark.CheckTime(func() {
			mb.MinPath(g)
		}, 100)

		resa, _ := benchmark.CheckTime(func() {
			ma.MinPath(g)
		}, 100)

		fmt.Fprintln(timeFile0, n, resbf)
		fmt.Fprintln(timeFile1, n, resa)

		row = append(row, fmt.Sprint(resbf))
		row = append(row, fmt.Sprint(resa))

		fmt.Printf("%s \\\\ \\hline \n", strings.Join(row, " & "))
	}
}
