package main

import (
	"fmt"
	"os"
)

var f function = f3

func Run(filename, mode string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	var matrix [][][]float64
	var x, y, z []float64

	var xVal, yVal, zVal float64
	var nX, nY, nZ int

	if mode == "gen" {
		var xStart, xEnd float64
		var xN int

		var yStart, yEnd float64
		var yN int

		var zStart, zEnd float64
		var zN int

		fmt.Fscan(file, &xStart, &xEnd, &xN)
		fmt.Fscan(file, &yStart, &yEnd, &yN)
		fmt.Fscan(file, &zStart, &zEnd, &zN)

		x = GetRange(xStart, xEnd, xN)
		y = GetRange(yStart, yEnd, yN)
		z = GetRange(zStart, zEnd, zN)

		matrix = GetMatrix(f, x, y, z)
		PrintMatrix(matrix, x, y, z)
	} else if mode == "in" {
		x = InputArray(file)
		y = InputArray(file)
		z = InputArray(file)
		matrix = Input3DMatrix(file, len(x), len(y), len(z))
	}

	fmt.Fscan(file, &nX, &nY, &nZ)
	fmt.Fscan(file, &xVal, &yVal, &zVal)

	res := MultiInterpolationXYZ(matrix, x, y, z, xVal, yVal, zVal, nX, nY, nZ)
	fmt.Printf("Interpolation (XYZ) = %.3f\n", res)

	res = MultiInterpolationZYX(matrix, x, y, z, xVal, yVal, zVal, nX, nY, nZ)
	fmt.Printf("Interpolation (ZYX) = %.3f\n", res)

	if mode == "gen" {
		fmt.Printf("F(x, y, z) = %.3f\n", f(xVal, yVal, zVal))
	}

	return nil
}
