package model

import (
	"lab/internal/random"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/widget"
)

type BaseModel struct {
	generatorTimer rand
	processorTimer rand

	generatorsNum *widget.Label
	plusButton    *widget.Button
	minusButton   *widget.Button
	box           *fyne.Container
}

func NewBaseModel(generatorIntencity, generatorDelta, processorIntencity float64) *BaseModel {
	generatorTimer := random.NewUniform(generatorIntencity, generatorDelta)
	processorTimer := random.NewExponential(processorIntencity)

	m := &BaseModel{
		generatorTimer: generatorTimer,
		processorTimer: processorTimer,
	}

	return m
}

func (m *BaseModel) Load() float64 {
	return m.generatorTimer.Intensity() / m.processorTimer.Intensity()
}

func (m *BaseModel) GenerateIntensity() float64 {
	return m.generatorTimer.Intensity()
}

func (m *BaseModel) ProcessIntensity() float64 {
	return m.processorTimer.Intensity()
}

type rand interface {
	Rand() float64
	Intensity() float64
}
