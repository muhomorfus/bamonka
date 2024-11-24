package driver

import (
	"lab/pkg/sdk"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/widget"
)

const (
	VeryLow  = "Очень низкий"
	Low      = "Низкий"
	Nominal  = "Номинальный"
	High     = "Высокий"
	VeryHigh = "Очень высокий"
)

type Driver struct {
	name         string
	selection    *widget.Select
	coefficients map[string]float64
}

func New(name string, coefficients map[string]float64) *Driver {
	idx := 0
	keys := make([]string, 0, len(coefficients))
	for key := range coefficients {
		if key == Nominal {
			idx = len(keys)
		}

		keys = append(keys, key)
	}

	w := widget.NewSelect(keys, func(string) {})
	w.SetSelectedIndex(idx)

	return &Driver{
		name:         name,
		selection:    w,
		coefficients: coefficients,
	}
}

func (d *Driver) Get() float64 {
	return d.coefficients[d.selection.Selected]
}

func (d *Driver) Widget() fyne.CanvasObject {
	return sdk.Describe(d.name, d.selection)
}
