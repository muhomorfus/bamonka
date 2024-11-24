package functional_points

import (
	"fmt"
	"lab/internal/driver"
	"lab/internal/int_input"
	"lab/internal/variant"

	"fyne.io/fyne/v2"
)

type Model struct {
	EILow, EINormal, EIHigh intInput
	EOLow, EONormal, EOHigh intInput
	EQLow, EQNormal, EQHigh intInput
    ILFLow, ILFNormal, ILFHigh intInput
    EIFLow, EIFNormal, EIFHigh intInput

    Params []driverComponent

    Languages map[string]intInput

	FPToLOC map[string]float64
}

func New() *Model {
	paramsMap := map[string]float64{
		"Нет влияния (0)": 0.0,
		"Случайное (1)": 1.0,
		"Небольшое (2)": 2.0,
		"Среднее (3)": 3.0,
		"Важное (4)": 4.0,
		"Основное (5)": 5.0,
	}

	return &Model{
		EILow: int_input.New("EI, Низкие", 4),
		EINormal: int_input.New("EI, Средние", 0),
		EIHigh: int_input.New("EI, Высокие", 0),
		EOLow: int_input.New("EO, Низкие", 2),
		EONormal: int_input.New("EO, Средние", 0),
		EOHigh: int_input.New("EO, Высокие", 0),
		EQLow: int_input.New("EQ, Низкие", 1),
		EQNormal: int_input.New("EQ, Средние", 0),
		EQHigh: int_input.New("EQ, Высокие", 0),
		ILFLow: int_input.New("ILF, Низкие", 3),
		ILFNormal: int_input.New("ILF, Средние", 0),
		ILFHigh: int_input.New("ILF, Высокие", 0),
		EIFLow: int_input.New("EIF, Низкие", 1),
		EIFNormal: int_input.New("EIF, Средние", 0),
		EIFHigh: int_input.New("EIF, Высокие", 0),

		Params: []driverComponent{
			driver.New("Передача данных", paramsMap, "Основное (5)"),
			driver.New("Распределенная обработка данных", paramsMap, "Основное (5)"),
			driver.New("Производительность", paramsMap, "Среднее (3)"),
			driver.New("Эксплуатационные ограничения", paramsMap, "Небольшое (2)"),
			driver.New("Частота транзакций", paramsMap, "Среднее (3)"),
			driver.New("Оперативный ввод данных", paramsMap, "Важное (4)"),
			driver.New("Эффективность работы конечных пользователей", paramsMap, "Случайное (1)"),
			driver.New("Оперативное обновление", paramsMap, "Важное (4)"),
			driver.New("Сложность обработки", paramsMap, "Важное (4)"),
			driver.New("Повторная используемость", paramsMap, "Нет влияния (0)"),
			driver.New("Легкость инсталляции", paramsMap, "Случайное (1)"),
			driver.New("Легкость эксплуатации", paramsMap, "Небольшое (2)"),
			driver.New("Количество возможных установок на различных платформах", paramsMap, "Небольшое (2)"),
			driver.New("Простота изменений (гибкость)", paramsMap, "Небольшое (2)"),
		},

		Languages: map[string]intInput{
			"ASM": int_input.New("ASM", 0),
			"C": int_input.New("C", 0),
			"C++": int_input.New("C++", 0),
			"C#": int_input.New("C#", 60),
			"Java": int_input.New("Java", 25),
			"SQL": int_input.New("SQL", 15),
		},

		FPToLOC: map[string]float64{
			"ASM": 320.0,
			"C": 128.0,
			"C++": 64.0,
			"C#": 53.0,
			"Java": 53.0,
			"SQL": 13.0,
		},
	}
}

func (m *Model) Compute() (*Result, error) {
	eiLow, err := m.EILow.Get()
	if err != nil {
		return nil, fmt.Errorf("invalid eiLow: %w", err)
	}
	eiNormal, err := m.EINormal.Get()
	if err != nil {
		return nil, fmt.Errorf("invalid eiNormal: %w", err)
	}
	eiHigh, err := m.EIHigh.Get()
	if err != nil {
		return nil, fmt.Errorf("invalid eiHigh: %w", err)
	}
	eoLow, err := m.EOLow.Get()
	if err != nil {
		return nil, fmt.Errorf("invalid eoLow: %w", err)
	}
	eoNormal, err := m.EONormal.Get()
	if err != nil {
		return nil, fmt.Errorf("invalid eoNormal: %w", err)
	}
	eoHigh, err := m.EOHigh.Get()
	if err != nil {
		return nil, fmt.Errorf("invalid eoHigh: %w", err)
	}
	eqLow, err := m.EQLow.Get()
	if err != nil {
		return nil, fmt.Errorf("invalid eqLow: %w", err)
	}
	eqNormal, err := m.EQNormal.Get()
	if err != nil {
		return nil, fmt.Errorf("invalid eqNormal: %w", err)
	}
	eqHigh, err := m.EQHigh.Get()
	if err != nil {
		return nil, fmt.Errorf("invalid eqHigh: %w", err)
	}
	ilfLow, err := m.ILFLow.Get()
	if err != nil {
		return nil, fmt.Errorf("invalid ilfLow: %w", err)
	}
	ilfNormal, err := m.ILFNormal.Get()
	if err != nil {
		return nil, fmt.Errorf("invalid ilfNormal: %w", err)
	}
	ilfHigh, err := m.ILFHigh.Get()
	if err != nil {
		return nil, fmt.Errorf("invalid ilfHigh: %w", err)
	}
	eifLow, err := m.EIFLow.Get()
	if err != nil {
		return nil, fmt.Errorf("invalid eifLow: %w", err)
	}
	eifNormal, err := m.EIFNormal.Get()
	if err != nil {
		return nil, fmt.Errorf("invalid eifNormal: %w", err)
	}
	eifHigh, err := m.EIFHigh.Get()
	if err != nil {
		return nil, fmt.Errorf("invalid eifHigh: %w", err)
	}

	ei := 3 * eiLow + 4 * eiNormal + 6 * eiHigh
	eo := 4 * eoLow + 5 * eoNormal + 7 * eoHigh
	eq := 3 * eqLow + 4 * eqNormal + 6 * eqHigh
	ilf := 7 * ilfLow + 10 * ilfNormal + 15 * ilfHigh
	eif := 5 * eifLow + 7 * eifNormal + 10 * eifHigh

	fp := ei + eo + eq + ilf + eif

	sumParams := 0.0
	for i := range m.Params {
		sumParams += m.Params[i].Get()
	}

	fpCorrected := float64(fp) * (0.65 + 0.01 * sumParams)

	sloc := 0.0
	for language, widget := range m.Languages {
		percent, err := widget.Get()
		if err != nil {
			return nil, fmt.Errorf("invalid %s: %w", language, err)
		}

		sloc += fpCorrected * (float64(percent) / 100.0) * m.FPToLOC[language]
	}

	return &Result{
		Points:  fmt.Sprintf("%d функциональных точек", fp),
		PointsCorrected:  fmt.Sprintf("%.1f функциональных точек", fpCorrected),
		SLOC: fmt.Sprintf("%.0f", sloc),
	}, nil
}

func (m *Model) Widgets() []fyne.CanvasObject {
	objects := []fyne.CanvasObject{
		m.EILow.Widget(),
		m.EINormal.Widget(),
		m.EIHigh.Widget(),
		m.EOLow.Widget(),
		m.EONormal.Widget(),
		m.EOHigh.Widget(),
		m.EQLow.Widget(),
		m.EQNormal.Widget(),
		m.EQHigh.Widget(),
		m.ILFLow.Widget(),
		m.ILFNormal.Widget(),
		m.ILFHigh.Widget(),
		m.EIFLow.Widget(),
		m.EIFNormal.Widget(),
		m.EIFHigh.Widget(),
	}

	for i := range m.Params {
		objects = append(objects, m.Params[i].Widget())
	}

	for key := range m.Languages {
		objects = append(objects, m.Languages[key].Widget())
	}

	return objects
}

type driverComponent interface {
	Get() float64
	Widget() fyne.CanvasObject
}

type variantComponent interface {
	Get() variant.Coefficients
	Widget() fyne.CanvasObject
}

type intInput interface {
	Get() (int, error)
	Widget() fyne.CanvasObject
}
