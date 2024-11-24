package int_input

import (
	"lab/pkg/sdk"
	"lab/pkg/sdk/advanced_widgets"

	"fyne.io/fyne/v2"
)

type Input struct {
	name string

	entry *advanced_widgets.NumericalEntry
}

func New(name string, defaultValue int) *Input {
	return &Input{
		name:  name,
		entry: advanced_widgets.NewIntEntry(defaultValue),
	}
}

func (h *Input) Widget() fyne.CanvasObject {
	return sdk.Describe(h.name, h.entry)
}

func (h *Input) Get() (int, error) {
	v, err := h.entry.GetInt()
	if err != nil {
		return 0, &sdk.Error{
			Human: "Неверное значение поля.",
			Raw:   err,
		}
	}

	return v, nil
}
