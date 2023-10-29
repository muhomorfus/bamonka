package main

import (
	"flag"
	"fmt"
	"log"
	"os"

	"lab_01/compute"
	"lab_01/func_info"
)

func main() {
	name := flag.String("filename", "", "Имя файла с данными о функции.")
	x := flag.Float64("x", 0, "Точка, для которой производится интерполяция.")
	n := flag.Int("n", 0, "Количество точек, забираемое в интерполяции.")
	flag.Parse()

	if *n <= 1 {
		log.Fatalln("n must be bigger than one")
	}

	file, err := os.Open(*name)
	if err != nil {
		log.Fatalln(err)
	}
	defer file.Close()

	points, err := func_info.FromReaderMany(file)
	if err != nil {
		log.Fatalln(err)
	}

	selected, err := compute.GetConfiguration(points, *x, *n+1)
	if err != nil {
		log.Fatalln(err)
	}
	fmt.Printf("\nВыбранные точки: ")
	for i, point := range selected {
		fmt.Printf("(%.4f, %.4f)", point.X, point.Y)
		if i != len(selected)-1 {
			fmt.Print(", ")
		}
	}
	fmt.Print(".\n")

	//fmt.Println()
	//err = compute.PrintNewtonDiffsTable(points, *x, *n)
	//if err != nil {
	//	log.Fatalln(err)
	//}

	newtonRes, err := compute.Polynomial(points, *x, *n+1, compute.NewtonDiffs)
	if err != nil {
		log.Fatalln(err)
	}
	fmt.Printf("Результат методом Ньютона для x = %.4f, n = %d: %.4f.\n", *x, *n, newtonRes)

	hermitRes, err := compute.Polynomial(points, *x, *n+1, compute.HermitDiffs)
	if err != nil {
		log.Fatalln(err)
	}
	fmt.Printf("Результат методом Эрмита для x = %.4f, n = %d: %.4f.\n", *x, *n, hermitRes)

	newtonRoot, err := compute.FindRoot(points, *x, *n+1, compute.NewtonDiffs)
	if err != nil {
		log.Fatalln(err)
	}
	fmt.Printf("Корень методов Ньютона для n = %d: %.4f.\n", *n, newtonRoot)

	hermitRoot, err := compute.FindRoot(points, *x, *n+1, compute.HermitDiffs)
	if err != nil {
		log.Fatalln(err)
	}
	fmt.Printf("Корень методов Эрмита для n = %d: %.4f.\n", *n, hermitRoot)

	err = compute.PrintInterpolationTable(points, *x, *n+1)
	if err != nil {
		log.Fatalln(err)
	}

	err = compute.PrintRootTable(points, *x, *n+1)
	if err != nil {
		log.Fatalln(err)
	}
}
