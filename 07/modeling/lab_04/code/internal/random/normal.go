package random

import (
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/widget"
	"lab_04/pkg/sdk"
	"lab_04/pkg/sdk/advanced_widgets"
	"math/rand"
)

type Normal struct {
	m     *advanced_widgets.NumericalEntry
	sigma *advanced_widgets.NumericalEntry
}

func NewNormal() *Normal {
	return &Normal{
		m:     advanced_widgets.NewFloatEntry(1),
		sigma: advanced_widgets.NewFloatEntry(10),
	}
}

func (n *Normal) Widgets() []fyne.CanvasObject {
	return []fyne.CanvasObject{
		widget.NewCard(
			"",
			"Параметры нормального распределения",
			container.NewVBox(
				sdk.Describe("m", n.m),
				sdk.Describe("sigma", n.sigma),
			),
		),
	}
}

func (n *Normal) Rand() (float64, error) {
	m, err := n.m.GetFloat()
	if err != nil {
		return 0.0, &sdk.Error{
			Human: "Неверный параметр: m.",
			Raw:   err,
		}
	}

	sigma, err := n.sigma.GetFloat()
	if err != nil {
		return 0.0, &sdk.Error{
			Human: "Неверный параметр: sigma.",
			Raw:   err,
		}
	}

	return rand.NormFloat64()*sigma + m, nil
}
