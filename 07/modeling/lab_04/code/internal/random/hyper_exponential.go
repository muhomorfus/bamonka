package random

import (
	"errors"
	"fmt"
	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/widget"
	"github.com/goulash/stat/dist"
	"github.com/samber/lo"
	"lab_04/pkg/sdk"
	"lab_04/pkg/sdk/advanced_widgets"
	"math"
	"math/rand"
	"sort"
	"strconv"
	"time"
)

type HyperExponential struct {
	n int

	probs   []*advanced_widgets.NumericalEntry
	lambdas []*advanced_widgets.NumericalEntry
}

func NewHyperExponential() *HyperExponential {
	return &HyperExponential{
		n: 3,
		probs: []*advanced_widgets.NumericalEntry{
			advanced_widgets.NewFloatEntry(0.3),
			advanced_widgets.NewFloatEntry(0.5),
			advanced_widgets.NewFloatEntry(0.2),
		},
		lambdas: []*advanced_widgets.NumericalEntry{
			advanced_widgets.NewFloatEntry(0.3),
			advanced_widgets.NewFloatEntry(0.6),
			advanced_widgets.NewFloatEntry(0.1),
		},
	}
}

func (h *HyperExponential) Widgets() []fyne.CanvasObject {
	plusButton := widget.NewButton("+", func() {
		h.probs = append(h.probs, advanced_widgets.NewFloatEntry(0.0))
		h.lambdas = append(h.lambdas, advanced_widgets.NewFloatEntry(0.0))
		h.n++
		sdk.Reload()
	})

	minusButton := widget.NewButton("-", func() {
		if h.n <= 1 {
			return
		}

		h.probs = h.probs[:len(h.probs)-1]
		h.lambdas = h.lambdas[:len(h.lambdas)-1]
		h.n--
		sdk.Reload()
	})

	l := widget.NewLabel(strconv.Itoa(h.n))
	l.Alignment = fyne.TextAlignCenter

	row := container.NewGridWithColumns(3, minusButton, l, plusButton)
	box := container.NewVBox(row)
	grid := container.NewGridWithColumns(4)

	for i := 0; i < h.n; i++ {
		c := widget.NewCard(
			"",
			"",
			container.NewVBox(
				sdk.Describe("p"+strconv.Itoa(i+1), h.probs[i]),
				sdk.Describe("λ"+strconv.Itoa(i+1), h.lambdas[i]),
			),
		)
		grid.Add(c)
	}
	box.Add(grid)

	return []fyne.CanvasObject{
		widget.NewCard(
			"",
			"Параметры гиперэкспоненциального распределения",
			box,
		),
	}
}

type coefficient struct {
	prob, lambda float64
}

func (h *HyperExponential) Rand() (float64, error) {
	coefficients := make([]coefficient, h.n)
	var err error

	for i := 0; i < h.n; i++ {
		coefficients[i].prob, err = h.probs[i].GetFloat()
		if err != nil {
			return 0.0, &sdk.Error{
				Human: fmt.Sprintf("Неверный параметр: p%d.", i+1),
				Raw:   err,
			}
		}

		coefficients[i].lambda, err = h.lambdas[i].GetFloat()
		if err != nil {
			return 0.0, &sdk.Error{
				Human: fmt.Sprintf("Неверный параметр: λ%d.", i+1),
				Raw:   err,
			}
		}
	}

	sort.Slice(coefficients, func(i, j int) bool {
		return coefficients[i].prob < coefficients[j].prob
	})

	probs := lo.Map(coefficients, func(c coefficient, _ int) float64 {
		return c.prob
	})

	lambdas := lo.Map(coefficients, func(c coefficient, _ int) float64 {
		return c.lambda
	})

	if math.Abs(lo.Sum(probs)-1.0) > 1e-6 {
		return 0.0, &sdk.Error{
			Human: "Сумма вероятностей должна быть равна 1.",
			Raw:   errors.New("sum probs must be 1.0"),
		}
	}

	return dist.NewHyperExponential(rand.NewSource(time.Now().UnixNano()), probs, lambdas).Float64(), nil
}
