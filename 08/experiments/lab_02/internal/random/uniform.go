package random

import (
	"math"
	"math/rand"
)

type Uniform struct {
	intensity float64
	delta     float64
}

func NewUniform(intensity, delta float64) *Uniform {
	return &Uniform{
		intensity: intensity,
		delta:     delta,
	}
}

func (u *Uniform) Rand() float64 {
	a := 1/u.intensity - u.delta
	b := 1/u.intensity + u.delta

	return math.Abs(rand.Float64()*(b-a) + a)
}

func (u *Uniform) Intensity() float64 {
	return u.intensity
}
