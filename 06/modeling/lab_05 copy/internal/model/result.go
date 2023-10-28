package model

import (
	"gonum.org/v1/plot/plotter"
)

type Result struct {
	X []float64
	Y []float64
	Z []float64

	err error
}

func (r *Result) Plot(name string) *Result {
	return nil
}

func (r *Result) Error() error {
	return r.err
}

func toXYs(x, y []float64) plotter.XYs {
	res := make(plotter.XYs, len(x))
	for i := range res {
		res[i].X = x[i]
		res[i].Y = y[i]
	}

	return res
}
