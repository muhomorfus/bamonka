package check

import (
	"github.com/samber/lo"
	"gonum.org/v1/gonum/stat"
	"math"
)

func IsRandom(values []int) float64 {
	return checkCorrelation(values)
}

func checkCorrelation(values []int) float64 {
	size := 5
	correlations := make([]float64, 0, len(values)/size)

	data := lo.Map(values, func(v int, _ int) float64 {
		return float64(v)
	})

	for i := size; i+size <= len(values); i += size {
		correlations = append(correlations, stat.Correlation(data[i-size:i], data[i:i+size], nil))
	}

	result := math.Abs(stat.Mean(correlations, nil))
	if math.IsNaN(result) || math.IsInf(result, 1) || math.IsInf(result, -1) {
		return 0.0
	}

	return 1.0 - result
}
