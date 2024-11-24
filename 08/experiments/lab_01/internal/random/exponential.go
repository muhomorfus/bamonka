package random

import (
	"lab_01/pkg/sdk"
	"lab_01/pkg/sdk/advanced_widgets"
	"math/rand"
	"strconv"
	"time"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/widget"
	"github.com/goulash/stat/dist"
)

type Exponential struct {
	name string

	intensity *advanced_widgets.NumericalEntry
}

func NewExponential(name string) *Exponential {
	return &Exponential{
		name:      name,
		intensity: advanced_widgets.NewFloatEntry(2),
	}
}

func (h *Exponential) Widgets() []fyne.CanvasObject {
	l := widget.NewLabel(strconv.Itoa(1))
	l.Alignment = fyne.TextAlignCenter

	return []fyne.CanvasObject{
		widget.NewCard(
			"",
			h.name,
			container.NewVBox(
				sdk.Describe("Интенсивность", h.intensity),
			),
		),
	}
}

func (h *Exponential) Rand() (float64, error) {
	l, err := h.intensity.GetFloat()
	if err != nil {
		return 0.0, &sdk.Error{
			Human: "Неверное значение интенсивности.",
			Raw:   err,
		}
	}

	ex := dist.NewExponential(rand.NewSource(time.Now().UnixNano()), l)

	return ex.Float64(), nil
}

func (h *Exponential) Intensity() float64 {
	v, _ := h.intensity.GetFloat()
	return v
}
