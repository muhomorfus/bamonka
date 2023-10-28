package main

import (
	"flag"
	"fmt"
	"lab_03/internal/algs"
	"lab_03/internal/algs/bucket"
	count "lab_03/internal/algs/count"
	"lab_03/internal/algs/heap"
	"lab_03/internal/utils"
)

func main() {
	filename := flag.String("filename", "", "имя файла с массивом")
	flag.Usage = func() {
		fmt.Println("Сортировка массива тремя алгоритмами сортировки: блочная, пирамидальная, подсчетом. Использование:")
		flag.PrintDefaults()
	}
	flag.Parse()

	a, err := utils.InputArrayFromFile(*filename)
	if err != nil {
		utils.ExitErr(err)
	}

	algorithms := []struct {
		m    algs.Sorter
		name string
	}{
		{bucket.New(3), "Блочная сортировка"},
		{heap.New(), "Пирамидальная сортировка"},
		{count.New(), "Сортировка подсчетом"},
	}

	for _, alg := range algorithms {
		res := alg.m.Sort(a)

		fmt.Println(alg.name)
		fmt.Println(res)
		fmt.Println()
	}
}
