package composition_model

import (
	"fmt"
	"lab/internal/driver"
	"lab/internal/int_input"
	"lab/internal/variant"
	"math"

	"github.com/charmbracelet/log"
	"golang.org/x/text/language"
	"golang.org/x/text/message"

	"fyne.io/fyne/v2"
)

type Model struct {
	RUSE intInput
	PROD driverComponent

	DisplayFormsSimple, DisplayFormsNormal, DisplayFormsHard intInput
	ReportsSimple, ReportsNormal, ReportsHard                intInput

	Salary intInput

	Modules intInput

	PREC, FLEX, RESL, TEAM, PMAT driverComponent
}

func New() *Model {
	return &Model{
		Salary:             int_input.New("Средняя зарплата", 230000),
		RUSE:               int_input.New("Повторное использование кода (RUSE)", 0),
		DisplayFormsSimple: int_input.New("Экранные формы, простые", 14),
		DisplayFormsNormal: int_input.New("Экранные формы, средние", 1),
		DisplayFormsHard:   int_input.New("Экранные формы, сложные", 0),
		ReportsSimple:      int_input.New("Отчеты, простые", 0),
		ReportsNormal:      int_input.New("Отчеты, средние", 2),
		ReportsHard:        int_input.New("Отчеты, сложные", 0),
		Modules:            int_input.New("Модулей на языках 3 поколения", 3),
		PROD: driver.New("Опытность команды (PROD)", map[string]float64{
			"Очень низкая":  4,
			"Низкая":        7,
			"Номинальная":   13,
			"Высокая":       25,
			"Очень высокая": 50,
		}, "Низкая"),
		PREC: driver.New("Новизна проекта (PREC)", map[string]float64{
			"Полное отсутствие прецедентов, полностью непредсказуемый проект":                 6.2,
			"Почти полное отсутствие прецедентов, в значительной мере непредсказуемый проект": 4.96,
			"Наличие некоторого количества прецедентов":                                       3.72,
			"Общее знакомство с проектом":                                                     2.48,
			"Значительное знакомство с проектом":                                              1.24,
			"Полное знакомство с проектом":                                                    0.0,
		}, "Почти полное отсутствие прецедентов, в значительной мере непредсказуемый проект"),
		FLEX: driver.New("Гибкость процесса разработки (FLEX)", map[string]float64{
			"Точный, строгий процесс разработки":   5.07,
			"Случайные послабления в процессе":     4.05,
			"Некоторые послабления в процессе":     3.04,
			"Большей частью согласованный процесс": 2.03,
			"Некоторое согласование процесса":      1.01,
			"Заказчик определил только общие цели": 0.0,
		}, "Большей частью согласованный процесс"),
		RESL: driver.New("Разрешение рисков в архитектуре системы (RESL)", map[string]float64{
			"Малое (20 %)":        7.0,
			"Некоторое (40 %)":    5.65,
			"Частое (60 %)":       4.24,
			"В целом (75 %)":      2.83,
			"Почти полное (90 %)": 1.41,
			"Полное (100%)":       0.0,
		}, "Некоторое (40 %)"),
		TEAM: driver.New("Сплоченность команды (TEAM)", map[string]float64{
			"Сильно затрудненное взаимодействие":    5.48,
			"Несколько затрудненное взаимодействие": 4.38,
			"Некоторая согласованность":             3.29,
			"Повышенная согласованность":            2.19,
			"Высокая согласованность":               1.1,
			"Взаимодействие как в едином целом":     0.0,
		}, "Некоторая согласованность"),
		PMAT: driver.New("Уровень зрелости процесса разработки (PMAT)", map[string]float64{
			"Уровень 1 СММ":  7.8,
			"Уровень 1+ СММ": 6.24,
			"Уровень 2 СММ":  4.68,
			"Уровень 3 СММ":  3.12,
			"Уровень 4 СММ":  1.56,
			"Уровень 5 СММ":  0.0,
		}, "Уровень 1+ СММ"),
	}
}

func (m *Model) Compute() (*Result, error) {
	salary, err := m.Salary.Get()
	if err != nil {
		return nil, fmt.Errorf("compute salary: %w", err)
	}
	ruse, err := m.RUSE.Get()
	if err != nil {
		return nil, fmt.Errorf("compute ruse: %w", err)
	}
	formsSimple, err := m.DisplayFormsSimple.Get()
	if err != nil {
		return nil, fmt.Errorf("compute simple forms: %w", err)
	}
	formsNormal, err := m.DisplayFormsNormal.Get()
	if err != nil {
		return nil, fmt.Errorf("compute normal forms: %w", err)
	}
	formsHard, err := m.DisplayFormsHard.Get()
	if err != nil {
		return nil, fmt.Errorf("compute hard forms: %w", err)
	}
	reportsSimple, err := m.ReportsSimple.Get()
	if err != nil {
		return nil, fmt.Errorf("compute simple reports: %w", err)
	}
	reportsNormal, err := m.ReportsNormal.Get()
	if err != nil {
		return nil, fmt.Errorf("compute normal reports: %w", err)
	}
	reportsHard, err := m.ReportsHard.Get()
	if err != nil {
		return nil, fmt.Errorf("compute hard reports: %w", err)
	}
	modules, err := m.Modules.Get()
	if err != nil {
		return nil, fmt.Errorf("compute modules: %w", err)
	}
	prod := m.PROD.Get()

	objectPoints := 1*formsSimple + 2*formsNormal + 3*formsHard +
		2*reportsSimple + 5*reportsNormal + 8*reportsHard +
		10*modules

	nop := float64(objectPoints) * ((100.0 - float64(ruse)) / 100.0)
	log.Info("got nop", "nop", nop)
	laborCost := nop / prod

	p := (m.PREC.Get()+m.FLEX.Get()+m.RESL.Get()+m.TEAM.Get()+m.PMAT.Get())/100.0 + 1.01
	p = 1.1

	time := 3 * math.Pow(laborCost, 0.33+0.2*(p-1.01))

	return &Result{
		LaborCost: fmt.Sprintf("%.2f человеко-месяцев", laborCost),
		Time:      fmt.Sprintf("%.2f месяцев", time),
		Cost:      message.NewPrinter(language.Russian).Sprintf("%d рублей", int(float64(salary)*laborCost)),
	}, nil
}

func (m *Model) Widgets() []fyne.CanvasObject {
	return []fyne.CanvasObject{
		m.PREC.Widget(),
		m.FLEX.Widget(),
		m.RESL.Widget(),
		m.TEAM.Widget(),
		m.PMAT.Widget(),
		m.Salary.Widget(),
		m.RUSE.Widget(),
		m.DisplayFormsSimple.Widget(),
		m.DisplayFormsNormal.Widget(),
		m.DisplayFormsHard.Widget(),
		m.ReportsSimple.Widget(),
		m.ReportsNormal.Widget(),
		m.ReportsHard.Widget(),
		m.Modules.Widget(),
		m.PROD.Widget(),
	}
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
