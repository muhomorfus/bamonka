package random

import (
	"lab_01/pkg/sdk"
	"lab_01/pkg/sdk/advanced_widgets"
	"math"
	"math/rand"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/widget"
)

type Uniform struct {
	name      string
	intensity *advanced_widgets.NumericalEntry
	delta     *advanced_widgets.NumericalEntry
}

func NewUniform(name string) *Uniform {
	return &Uniform{
		name:      name,
		intensity: advanced_widgets.NewFloatEntry(5),
		delta:     advanced_widgets.NewFloatEntry(0.2),
	}
}

func (u *Uniform) Widgets() []fyne.CanvasObject {
	return []fyne.CanvasObject{
		widget.NewCard(
			"",
			u.name,
			container.NewVBox(
				sdk.Describe("Интенсивность", u.intensity),
				sdk.Describe("Отклонение", u.delta),
			),
		),
	}
}

func (u *Uniform) Rand() (float64, error) {
	intensity, err := u.intensity.GetFloat()
	if err != nil {
		return 0.0, &sdk.Error{
			Human: "Неверное значение интенсивности.",
			Raw:   err,
		}
	}

	delta, err := u.delta.GetFloat()
	if err != nil {
		return 0.0, &sdk.Error{
			Human: "Неверное значение отклонения.",
			Raw:   err,
		}
	}

	a := 1/intensity - delta
	b := 1/intensity + delta

	return math.Abs(rand.Float64()*(b-a) + a), nil
}

func (u *Uniform) Intensity() float64 {
	v, _ := u.intensity.GetFloat()
	return v
}
