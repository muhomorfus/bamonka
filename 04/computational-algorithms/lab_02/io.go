package main

import (
	"fmt"
	"github.com/jedib0t/go-pretty/v6/table"
	"io"
)

func PrintMatrix(matrix [][][]float64, x, y, z []float64) {
	for i := range z {
		t := table.NewWriter()
		t.SetStyle(table.StyleDouble)

		header := make(table.Row, 0)

		header = append(header, "y / x")
		for j := range x {
			header = append(header, fmt.Sprintf("%.3f", x[j]))
		}
		t.AppendHeader(header)

		for j := range y {
			row := make(table.Row, 0)

			row = append(row, fmt.Sprintf("%.3f ", y[j]))
			for k := range x {
				row = append(row, fmt.Sprintf("%.3f", matrix[i][j][k]))
			}

			t.AppendRow(row)
		}

		fmt.Printf("z = %f\n", z[i])
		fmt.Println(t.Render())
		fmt.Println()
	}
}

func InputArray(r io.Reader) []float64 {
	var n int
	fmt.Fscan(r, &n)

	arr := make([]float64, n)
	for i := range arr {
		fmt.Fscan(r, &arr[i])
	}

	return arr
}

func Input3DMatrix(r io.Reader, nX, nY, nZ int) [][][]float64 {
	matrix := make([][][]float64, nZ)

	for i := range matrix {
		matrix[i] = make([][]float64, nY)

		for j := range matrix[i] {
			matrix[i][j] = make([]float64, nX)

			for k := range matrix[i][j] {
				fmt.Fscan(r, &matrix[i][j][k])
			}
		}
	}

	return matrix
}
