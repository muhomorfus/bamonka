package random

import (
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/widget"
	"lab_01/pkg/sdk"
	"lab_01/pkg/sdk/advanced_widgets"
	"math"
	"math/rand"
)

type Normal struct {
	name string

	intensity *advanced_widgets.NumericalEntry
	sigma     *advanced_widgets.NumericalEntry
}

func NewNormal(name string) *Normal {
	return &Normal{
		name:      name,
		intensity: advanced_widgets.NewFloatEntry(10),
		sigma:     advanced_widgets.NewFloatEntry(0.1),
	}
}

func (n *Normal) Widgets() []fyne.CanvasObject {
	return []fyne.CanvasObject{
		widget.NewCard(
			"",
			n.name,
			container.NewVBox(
				sdk.Describe("Интенсивность", n.intensity),
				sdk.Describe("Параметр σ", n.sigma),
			),
		),
	}
}

func (n *Normal) Rand() (float64, error) {
	intensity, err := n.intensity.GetFloat()
	if err != nil {
		return 0.0, &sdk.Error{
			Human: "Неверное значение интенсивности.",
			Raw:   err,
		}
	}

	sigma, err := n.sigma.GetFloat()
	if err != nil {
		return 0.0, &sdk.Error{
			Human: "Неверное значение параметра σ.",
			Raw:   err,
		}
	}

	m := 1 / intensity

	return math.Abs(rand.NormFloat64()*sigma + m), nil
}

func (n *Normal) Intensity() float64 {
	v, _ := n.intensity.GetFloat()
	return v
}
