package main

import "math"

type function func(float64, float64, float64) float64

func f1(x, y, z float64) float64 {
	return 1/(x+y) - z
}

func f2(x, y, z float64) float64 {
	return math.Exp(2*x-y) + z*z
}

func f3(x, y, z float64) float64 {
	return (x*x + y*y) / z
}
