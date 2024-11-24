package model

import (
	"errors"
	"fmt"
	"lab_01/internal/random"
	"lab_01/pkg/sdk"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/widget"
)

type BaseModel struct {
	generatorTimers []rand
	processorTimer  rand

	generatorsNum *widget.Label
	plusButton    *widget.Button
	minusButton   *widget.Button
	box           *fyne.Container
}

func NewBaseModel() *BaseModel {
	generatorTimers := []rand{
		//random.NewExponential("Генератор 1"),
		//random.NewExponential("Генератор 2"),
		random.NewExponential("Генератор 1"),
	}

	processorTimer := random.NewUniform("Обработчик")

	m := &BaseModel{
		generatorTimers: generatorTimers,
		processorTimer:  processorTimer,
	}

	m.generatorsNum = widget.NewLabel("1")
	m.generatorsNum.Alignment = fyne.TextAlignCenter
	m.plusButton = widget.NewButton("+", func() {
		// r := random.NewExponential(fmt.Sprintf("Генератор %d", len(m.generatorTimers)+1))
		r := random.NewUniform(fmt.Sprintf("Генератор %d", len(m.generatorTimers)+1))
		m.generatorsNum.SetText(fmt.Sprint(len(m.generatorTimers) + 1))
		m.generatorTimers = append(m.generatorTimers, r)
		m.updateBox()
		sdk.Reload()
	})
	m.minusButton = widget.NewButton("-", func() {
		if len(m.generatorTimers) == 1 {
			sdk.ShowError(&sdk.Error{
				Human: "Количество генераторов не может быть меньше 1",
				Raw:   errors.New("cant decrease generators"),
			})
			return
		}
		m.generatorsNum.SetText(fmt.Sprint(len(m.generatorTimers) - 1))
		m.generatorTimers = m.generatorTimers[:len(m.generatorTimers)-1]
		m.updateBox()
		sdk.Reload()
	})
	m.updateBox()

	return m
}

func (m *BaseModel) updateBox() {
	m.box = container.NewGridWithColumns(2)
	for _, g := range m.generatorTimers {
		for _, obj := range g.Widgets() {
			m.box.Add(obj)
		}
	}
}

func (m *BaseModel) Widgets() []fyne.CanvasObject {
	grid := container.NewGridWithColumns(3, m.minusButton, m.generatorsNum, m.plusButton)
	result := []fyne.CanvasObject{
		m.box,
		grid,
	}
	return append(result, m.processorTimer.Widgets()...)
}

func (m *BaseModel) Load() float64 {
	sum := 0.0
	for _, g := range m.generatorTimers {
		sum += g.Intensity()
	}

	return sum / m.processorTimer.Intensity()
}

func (m *BaseModel) GenerateIntensity() float64 {
	sum := 0.0
	for _, g := range m.generatorTimers {
		sum += g.Intensity()
	}

	return sum
}

func (m *BaseModel) ProcessIntensity() float64 {
	return m.processorTimer.Intensity()
}

type rand interface {
	Rand() (float64, error)
	Widgets() []fyne.CanvasObject
	Intensity() float64
}
