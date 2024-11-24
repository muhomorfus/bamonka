package random

import (
	"math/rand"
	"time"

	"github.com/goulash/stat/dist"
)

type Exponential struct {
	intensity float64
}

func NewExponential(intencity float64) *Exponential {
	return &Exponential{
		intensity: intencity,
	}
}

func (h *Exponential) Rand() float64 {
	return dist.NewExponential(rand.NewSource(time.Now().UnixNano()), h.intensity).Float64()
}

func (h *Exponential) Intensity() float64 {
	return h.intensity
}
