package cocomo

import (
	"fmt"
	"lab/internal/driver"
	"lab/internal/int_input"
	"lab/internal/variant"
	"math"

	"github.com/samber/lo"
	"golang.org/x/text/language"
	"golang.org/x/text/message"

	"fyne.io/fyne/v2"
)

type Model struct {
	EmployeeCost                 intInput
	LOC                          intInput
	Variant                      variantComponent
	RELY, DATA, CPLX             driverComponent
	TIME, STOR, VIRT, TURN       driverComponent
	ACAP, AEXP, PCAP, VEXP, LEXP driverComponent
	MODP, TOOL, SCED             driverComponent
	Stages                       []stage
	Categories                   []category
}

func New() *Model {
	return &Model{
		EmployeeCost: int_input.New("Средняя зарплата", 250000),
		LOC:          int_input.New("Количество строк кода", 10000),
		Variant:      variant.New(),
		RELY: driver.New("Требуемая надежность", map[string]float64{
			driver.VeryLow:  0.75,
			driver.Low:      0.86,
			driver.Nominal:  1.0,
			driver.High:     1.15,
			driver.VeryHigh: 1.4,
		}),
		DATA: driver.New("Размер базы данных", map[string]float64{
			driver.Low:      0.94,
			driver.Nominal:  1.0,
			driver.High:     1.08,
			driver.VeryHigh: 1.16,
		}),
		CPLX: driver.New("Сложность продукта", map[string]float64{
			driver.VeryLow:  0.7,
			driver.Low:      0.85,
			driver.Nominal:  1.0,
			driver.High:     1.15,
			driver.VeryHigh: 1.3,
		}),
		TIME: driver.New("Ограничение времени выполнения", map[string]float64{
			driver.Nominal:  1.0,
			driver.High:     1.11,
			driver.VeryHigh: 1.5,
		}),
		STOR: driver.New("Ограничение объема основной памяти", map[string]float64{
			driver.Nominal:  1.0,
			driver.High:     1.06,
			driver.VeryHigh: 1.21,
		}),
		VIRT: driver.New("Изменчивость виртуальной машины", map[string]float64{
			driver.Low:      0.87,
			driver.Nominal:  1.0,
			driver.High:     1.15,
			driver.VeryHigh: 1.3,
		}),
		TURN: driver.New("Время реакции компьютера", map[string]float64{
			driver.Low:      0.87,
			driver.Nominal:  1.0,
			driver.High:     1.07,
			driver.VeryHigh: 1.15,
		}),
		ACAP: driver.New("Способности аналитика", map[string]float64{
			driver.VeryLow:  1.46,
			driver.Low:      1.19,
			driver.Nominal:  1.0,
			driver.High:     0.86,
			driver.VeryHigh: 0.71,
		}),
		AEXP: driver.New("Знание приложений", map[string]float64{
			driver.VeryLow:  1.29,
			driver.Low:      1.15,
			driver.Nominal:  1.0,
			driver.High:     0.91,
			driver.VeryHigh: 0.82,
		}),
		PCAP: driver.New("Способности программиста", map[string]float64{
			driver.VeryLow:  1.42,
			driver.Low:      1.17,
			driver.Nominal:  1.0,
			driver.High:     0.86,
			driver.VeryHigh: 0.7,
		}),
		VEXP: driver.New("Знание виртуальной машины", map[string]float64{
			driver.VeryLow: 1.21,
			driver.Low:     1.1,
			driver.Nominal: 1.0,
			driver.High:    0.9,
		}),
		LEXP: driver.New("Знание языка программирования", map[string]float64{
			driver.VeryLow: 1.14,
			driver.Low:     1.07,
			driver.Nominal: 1.0,
			driver.High:    0.95,
		}),
		MODP: driver.New("Использование современных методов", map[string]float64{
			driver.VeryLow:  1.24,
			driver.Low:      1.1,
			driver.Nominal:  1.0,
			driver.High:     0.91,
			driver.VeryHigh: 0.82,
		}),
		TOOL: driver.New("Использование программных инструментов", map[string]float64{
			driver.VeryLow:  1.24,
			driver.Low:      1.1,
			driver.Nominal:  1.0,
			driver.High:     0.91,
			driver.VeryHigh: 0.82,
		}),
		SCED: driver.New("Требуемые сроки разработки", map[string]float64{
			driver.VeryLow:  1.23,
			driver.Low:      1.08,
			driver.Nominal:  1.0,
			driver.High:     0.04,
			driver.VeryHigh: 1.1,
		}),
		Stages: []stage{
			{
				name:      "Планирование и определение\nтребований",
				laborCost: 0.08,
				time:      0.36,
			},
			{
				name:      "Проектирование продукта",
				laborCost: 0.18,
				time:      0.36,
			},
			{
				name:      "Детальное проектирование",
				laborCost: 0.25,
				time:      0.18,
			},
			{
				name:      "Кодирование и тестирование\nотдельных модулей",
				laborCost: 0.26,
				time:      0.18,
			},
			{
				name:      "Интеграция и тестирование",
				laborCost: 0.31,
				time:      0.28,
			},
		},
		Categories: []category{
			{
				name:      "Анализ требований",
				laborCost: 0.04,
			},
			{
				name:      "Проектирование продукта",
				laborCost: 0.12,
			},
			{
				name:      "Программирование",
				laborCost: 0.44,
			},
			{
				name:      "Тестирование",
				laborCost: 0.06,
			},
			{
				name:      "Верификация и аттестация",
				laborCost: 0.14,
			},
			{
				name:      "Канцелярия проекта",
				laborCost: 0.07,
			},
			{
				name:      "Управление конфигурацией\nи обеспечение качества",
				laborCost: 0.07,
			},
			{
				name:      "Создание руководств",
				laborCost: 0.06,
			},
		},
	}
}

func (m *Model) Compute() (*Result, error) {
	cost, err := m.EmployeeCost.Get()
	if err != nil {
		return nil, fmt.Errorf("compute cost: %w", err)
	}
	loc, err := m.LOC.Get()
	if err != nil {
		return nil, fmt.Errorf("compute kloc: %w", err)
	}
	kloc := float64(loc) / 1000.0
	coefficients := m.Variant.Get()
	eaf := m.eaf()

	laborCost := coefficients.C1 * eaf * math.Pow(float64(kloc), coefficients.P1)
	time := coefficients.C2 * math.Pow(laborCost, coefficients.P2)

	// Составление табличек.
	byStage := lo.Map(m.Stages, func(s stage, _ int) []string {
		return []string{
			s.name,
			fmt.Sprintf("%.2f мес", s.laborCost*laborCost),
			fmt.Sprintf("%.2f мес", s.time*time),
		}
	})
	byStage = append([][]string{{"Стадия", "Трудозатраты", "Время"}}, byStage...)

	byCategory := lo.Map(m.Categories, func(c category, _ int) []string {
		return []string{
			c.name,
			fmt.Sprintf("%.2f мес", c.laborCost*laborCost),
		}
	})
	byCategory = append([][]string{{"Вид", "Трудозатраты"}}, byCategory...)

	// Учет времени на анализ.
	laborCost += laborCost * m.Stages[0].laborCost
	time += time * m.Stages[0].time

	return &Result{
		LaborCost:  fmt.Sprintf("%.3f человеко-месяцев", laborCost),
		Time:       fmt.Sprintf("%.3f месяцев", time),
		Cost:       message.NewPrinter(language.Russian).Sprintf("%d рублей", int(float64(cost)*laborCost)),
		ByStage:    byStage,
		ByCategory: byCategory,
	}, nil
}

func (m *Model) eaf() float64 {
	return m.RELY.Get() * m.DATA.Get() * m.CPLX.Get() *
		m.TIME.Get() * m.STOR.Get() * m.VIRT.Get() * m.TURN.Get() *
		m.ACAP.Get() * m.AEXP.Get() * m.PCAP.Get() * m.VEXP.Get() * m.LEXP.Get() *
		m.MODP.Get() * m.TOOL.Get() * m.SCED.Get()
}

func (m *Model) Widgets() []fyne.CanvasObject {
	return []fyne.CanvasObject{
		m.Variant.Widget(),
		m.EmployeeCost.Widget(),
		m.LOC.Widget(),
		m.RELY.Widget(),
		m.DATA.Widget(),
		m.CPLX.Widget(),
		m.TIME.Widget(),
		m.STOR.Widget(),
		m.VIRT.Widget(),
		m.TURN.Widget(),
		m.ACAP.Widget(),
		m.AEXP.Widget(),
		m.PCAP.Widget(),
		m.VEXP.Widget(),
		m.LEXP.Widget(),
		m.MODP.Widget(),
		m.TOOL.Widget(),
		m.SCED.Widget(),
	}
}

type stage struct {
	name      string
	laborCost float64
	time      float64
}

type category struct {
	name      string
	laborCost float64
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
