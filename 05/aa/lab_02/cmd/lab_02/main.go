package main

import (
	"flag"
	"fmt"
	"lab_02/internal/algs"
	"lab_02/internal/algs/usual"
	"lab_02/internal/algs/winograd"
	"lab_02/internal/algs/winograd_improved"
	"lab_02/internal/utils"
)

func main() {
	filenameA := flag.String("filenameA", "", "имя файла с матрицей A")
	filenameB := flag.String("filenameB", "", "имя файла с матрицей B")
	flag.Usage = func() {
		fmt.Println("Умножатель матриц AxB. использование:")
		flag.PrintDefaults()
	}
	flag.Parse()

	a, err := utils.InputMatrixFromFile(*filenameA)
	if err != nil {
		utils.ExitErr(err)
	}

	b, err := utils.InputMatrixFromFile(*filenameB)
	if err != nil {
		utils.ExitErr(err)
	}

	algorithms := []struct {
		m    algs.Multiplier
		name string
	}{
		{usual.New(), "Обычный алгоритм умножения матриц"},
		{winograd.New(), "Виноград"},
		{winograd_improved.New(), "Улучшенный Виноград"},
	}

	for _, alg := range algorithms {
		res, err := alg.m.Multiply(a, b)
		if err != nil {
			utils.ExitErr(err)
		}

		fmt.Println(alg.name)
		utils.PrintMatrix(res)
		fmt.Println()
	}
}
