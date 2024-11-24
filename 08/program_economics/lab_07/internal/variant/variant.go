package variant

import (
	"lab/pkg/sdk"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/widget"
)

const (
	Default  = "Обычный вариант"
	Middle   = "Промежуточный вариант"
	Embedded = "Встроенный вариант"
)

type Variant struct {
	selection    *widget.Select
	coefficients map[string]Coefficients
}

func New() *Variant {
	keys := []string{Default, Middle, Embedded}
	w := widget.NewSelect(keys, func(string) {})
	w.SetSelectedIndex(0)

	return &Variant{
		selection: w,
		coefficients: map[string]Coefficients{
			Default: {
				C1: 3.2,
				P1: 1.05,
				C2: 2.5,
				P2: 0.38,
			},
			Middle: {
				C1: 3.0,
				P1: 1.12,
				C2: 2.5,
				P2: 0.35,
			},
			Embedded: {
				C1: 2.8,
				P1: 1.2,
				C2: 2.5,
				P2: 0.32,
			},
		},
	}
}

func (d *Variant) Get() Coefficients {
	return d.coefficients[d.selection.Selected]
}

func (d *Variant) Widget() fyne.CanvasObject {
	return sdk.Describe("Режим модели", d.selection)
}

type Coefficients struct {
	C1, P1 float64
	C2, P2 float64
}
