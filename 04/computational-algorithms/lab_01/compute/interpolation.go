package compute

import (
	"lab_01/func_info"
)

type DiffFunc func([]func_info.FuncInfo, int) ([]float64, []float64)

func NewtonDiffs(fis []func_info.FuncInfo, _ int) ([]float64, []float64) {
	x := make([]float64, len(fis))
	y := make([]float64, len(fis))

	for i, fi := range fis {
		x[i] = fi.X
		y[i] = fi.Y
	}

	diffs := make([]float64, 1)
	diffs[0] = y[0]

	for i := 1; i < len(y); i++ {
		for j := 0; j < len(y)-i; j++ {
			y[j] = (y[j] - y[j+1]) / (x[j] - x[j+i])
		}
		diffs = append(diffs, y[0])
	}

	return diffs, x
}

func HermitDiffs(fis []func_info.FuncInfo, n int) ([]float64, []float64) {
	x := make([]float64, n)
	y := make([]float64, n)

	for i := 0; i < n; i++ {
		x[i] = fis[i/2].X
		y[i] = fis[i/2].Y
	}

	diffs := make([]float64, 1)
	diffs[0] = y[0]

	for i := 0; i < n-1; i++ {
		if i%2 == 0 {
			y[i] = fis[i/2].Derivative
		} else {
			y[i] = (y[i] - y[i+1]) / (x[i] - x[i+1])
		}
	}
	diffs = append(diffs, y[0])

	for i := 2; i < n; i++ {
		for j := 0; j < n-i; j++ {
			y[j] = (y[j] - y[j+1]) / (x[j] - x[j+i])
		}
		diffs = append(diffs, y[0])
	}

	return diffs, x
}

func Polynomial(fis []func_info.FuncInfo, x float64, n int, diffsFunc DiffFunc) (float64, error) {
	points, err := GetConfiguration(fis, x, n)
	if err != nil {
		return 0, err
	}

	diffs, xs := diffsFunc(points, n)
	var result float64
	var m float64 = 1

	for i, diff := range diffs {
		result += diff * m
		m *= x - xs[i]
	}

	return result, nil
}
