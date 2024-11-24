package early_arch

import (
	"fmt"
	"lab/internal/driver"
	"lab/internal/int_input"
	"lab/internal/variant"
	"math"

	"golang.org/x/text/language"
	"golang.org/x/text/message"

	"fyne.io/fyne/v2"
)

type Model struct {
	Salary intInput
	SLOC   intInput

	RCPX, RUSE, PDIF, PERS, PREX, FCIL, SCED driverComponent
	PREC, FLEX, RESL, TEAM, PMAT             driverComponent
}

func New() *Model {
	return &Model{
		Salary: int_input.New("Средняя зарплата", 230000),
		SLOC:   int_input.New("SLOC", 2372),
		PERS: driver.New("Опытность персонала (PERS)", map[string]float64{
			"Сверхнизкий":   2.12,
			"Очень низкий":  1.62,
			"Низкий":        1.26,
			"Номинальный":   1.0,
			"Высокий":       0.83,
			"Очень высокий": 0.63,
			"Сверхвысокий":  0.5,
		}, "Номинальный"),
		RCPX: driver.New("Надежность и сложность продукта (RCPX)", map[string]float64{
			"Сверхнизкий":   0.49,
			"Очень низкий":  0.60,
			"Низкий":        0.83,
			"Номинальный":   1.0,
			"Высокий":       1.33,
			"Очень высокий": 1.91,
			"Сверхвысокий":  2.72,
		}, "Очень высокий"),
		RUSE: driver.New("Повторное использование компонентов (RUSE)", map[string]float64{
			"Низкий":        0.95,
			"Номинальный":   1.0,
			"Высокий":       1.07,
			"Очень высокий": 1.15,
			"Сверхвысокий":  1.24,
		}, "Низкий"),
		PDIF: driver.New("Сложность платформы (PDIF)", map[string]float64{
			"Низкий":        0.87,
			"Номинальный":   1.0,
			"Высокий":       1.29,
			"Очень высокий": 1.81,
			"Сверхвысокий":  2.61,
		}, "Высокий"),
		PREX: driver.New("Способности персонала (PREX)", map[string]float64{
			"Сверхнизкий":   1.59,
			"Очень низкий":  1.33,
			"Низкий":        1.22,
			"Номинальный":   1.0,
			"Высокий":       0.87,
			"Очень высокий": 0.73,
			"Сверхвысокий":  0.62,
		}, "Низкий"),
		FCIL: driver.New("Возможности среды (FCIL)", map[string]float64{
			"Сверхнизкий":   1.43,
			"Очень низкий":  1.30,
			"Низкий":        1.10,
			"Номинальный":   1.0,
			"Высокий":       0.87,
			"Очень высокий": 0.73,
			"Сверхвысокий":  0.62,
		}, "Очень высокий"),
		SCED: driver.New("Сроки (SCED)", map[string]float64{
			"Очень низкий":  1.43,
			"Низкий":        1.14,
			"Номинальный":   1.0,
			"Высокий":       1.0,
			"Очень высокий": 1.0,
		}, "Высокий"),
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
	sloc, err := m.SLOC.Get()
	if err != nil {
		return nil, fmt.Errorf("compute sloc: %w", err)
	}

	ksloc := float64(sloc) / 1000.0

	earch := m.PERS.Get() * m.RCPX.Get() * m.RUSE.Get() * m.PDIF.Get() * m.PREX.Get() * m.FCIL.Get() * m.SCED.Get()

	p := (m.PREC.Get()+m.FLEX.Get()+m.RESL.Get()+m.TEAM.Get()+m.PMAT.Get())/100.0 + 1.01
	p = 1.1

	laborCost := 2.45 * earch * math.Pow(ksloc, p)
	time := 3 * math.Pow(laborCost, 0.33+0.2*(p-1.01))

	return &Result{
		LaborCost: fmt.Sprintf("%.2f человеко-месяцев", laborCost),
		Time:      fmt.Sprintf("%.2f месяцев", time),
		Cost:      message.NewPrinter(language.Russian).Sprintf("%d рублей", int(float64(salary)*laborCost)),
	}, nil
}

func (m *Model) Widgets() []fyne.CanvasObject {
	return []fyne.CanvasObject{
		m.Salary.Widget(),
		m.SLOC.Widget(),

		m.PREC.Widget(),
		m.FLEX.Widget(),
		m.RESL.Widget(),
		m.TEAM.Widget(),
		m.PMAT.Widget(),

		m.PERS.Widget(),
		m.RCPX.Widget(),
		m.RUSE.Widget(),
		m.PDIF.Widget(),
		m.PREX.Widget(),
		m.FCIL.Widget(),
		m.SCED.Widget(),
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
