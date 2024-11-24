package random

import (
	"math"
	"math/rand"
)

type Normal struct {
	intensity float64
	sigma     float64
}

func NewNormal(intensity, sigma float64) *Normal {
	return &Normal{
		intensity: intensity,
		sigma:     sigma,
	}
}

func (n *Normal) Rand() float64 {
	m := 1 / n.intensity

	return math.Abs(rand.NormFloat64()*n.sigma + m)
}

func (n *Normal) Intensity() float64 {
	return n.intensity
}
