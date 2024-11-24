package planner

import (
	"fmt"
	"math"
	"strconv"
	"strings"

	"github.com/samber/lo"
	"gonum.org/v1/gonum/stat/combin"
)

type Planner struct {
	Factors           []factor
	Combinations      [][]int
	ExperimentsNumber int

	Plan           [][]float64
	NormalizedPlan [][]float64
	PlanTable      PlanTable

	NormalizedCoefficients []float64
	LinearCoefficients     []float64
	NonLinearCoefficients  []float64

	Results []float64
}

func New(factors ...factor) *Planner {
	p := &Planner{
		Factors:           factors,
		ExperimentsNumber: int(math.Pow(2, float64(len(factors)))),
	}

	p.Results = make([]float64, p.ExperimentsNumber)

	combinations := make([][]int, len(p.Factors))
	for i := range p.Factors {
		combinations[i] = []int{i}
	}

	for k := 2; k <= len(p.Factors); k++ {
		combinations = append(combinations, combin.Combinations(len(p.Factors), k)...)
	}

	p.Combinations = combinations
	p.fillNormalizedPlan()
	p.fillPlan()

	return p
}

func (p *Planner) GetPlan(i int) []float64 {
	return p.Plan[i]
}

func (p *Planner) SaveResult(i int, result float64) {
	p.Results[i] = result
}

func (p *Planner) ComputeCoefficients() {
	p.computeNormalizedCoefficients()
	p.computeLinearCoefficients()
	p.computeNonLinearCoefficients()
}

func (p *Planner) computeNormalizedCoefficients() {
	p.NormalizedCoefficients = make([]float64, len(p.NormalizedPlan[0]))
	for j := 0; j < len(p.NormalizedPlan[0]); j++ {
		for i := 0; i < p.ExperimentsNumber; i++ {
			p.NormalizedCoefficients[j] += p.NormalizedPlan[i][j] * p.Results[i]
		}

		p.NormalizedCoefficients[j] /= float64(p.ExperimentsNumber)
	}
}

func (p *Planner) computeLinearCoefficients() {
	p.LinearCoefficients = make([]float64, len(p.Factors)+1)

	p.LinearCoefficients[0] = p.NormalizedCoefficients[0]
	for i := 1; i <= len(p.Factors); i++ {
		p.LinearCoefficients[0] -= p.NormalizedCoefficients[i] * (p.Factors[i-1].Max() + p.Factors[i-1].Min()) / (p.Factors[i-1].Max() - p.Factors[i-1].Min())
	}

	for i := 1; i <= len(p.Factors); i++ {
		p.LinearCoefficients[i] = 2 * p.NormalizedCoefficients[i] / (p.Factors[i-1].Max() - p.Factors[i-1].Min())
	}
}

func (p *Planner) computeNonLinearCoefficients() {
	p.NonLinearCoefficients = make([]float64, 1+len(p.Combinations))

	centers := append([]float64{0.0}, lo.Map(
		p.Factors,
		func(f factor, _ int) float64 {
			return (f.Min() + f.Max()) / 2
		},
	)...)

	deltas := append([]float64{0.0}, lo.Map(
		p.Factors,
		func(f factor, _ int) float64 {
			return (f.Max() - f.Min()) / 2
		},
	)...)

	ratios := lo.Map(
		centers,
		func(_ float64, i int) float64 {
			if i == 0 {
				return 0.0
			}

			return centers[i] / deltas[i]
		},
	)

	coeffs := p.NormalizedCoefficients

	p.NonLinearCoefficients[0] = makeSign(coeffs[0] - lo.Sum(lo.Map(centers, func(_ float64, i int) float64 {
		return centers[i] * ratios[i]
	})) + coeffs[4]*ratios[1]*ratios[2] + coeffs[5]*ratios[1]*ratios[3] +
		coeffs[6]*ratios[2]*ratios[3] - coeffs[7]*ratios[1]*ratios[2]*ratios[3], coeffs[0])

	p.NonLinearCoefficients[1] = makeSign((coeffs[1] - coeffs[4]*ratios[2] - coeffs[5]*ratios[3] + coeffs[7]*ratios[2]*ratios[3]) / deltas[1], coeffs[1])
	p.NonLinearCoefficients[2] = makeSign((coeffs[2] - coeffs[4]*ratios[1] - coeffs[6]*ratios[3] + coeffs[7]*ratios[1]*ratios[3]) / deltas[2], coeffs[2])
	p.NonLinearCoefficients[3] = makeSign((coeffs[3] - coeffs[5]*ratios[1] - coeffs[6]*ratios[2] + coeffs[7]*ratios[1]*ratios[2]) / deltas[3], coeffs[3])
	p.NonLinearCoefficients[4] = makeSign((coeffs[4] - coeffs[7]*centers[3]/deltas[3]) / deltas[1] / deltas[2], coeffs[4])
	p.NonLinearCoefficients[5] = makeSign((coeffs[5] - coeffs[7]*centers[2]/deltas[2]) / deltas[1] / deltas[3], coeffs[5])
	p.NonLinearCoefficients[6] = makeSign((coeffs[6] - coeffs[7]*centers[1]/deltas[1]) / deltas[2] / deltas[3], coeffs[6])
	p.NonLinearCoefficients[7] = makeSign(coeffs[7] / deltas[1] / deltas[2] / deltas[3], coeffs[7])
}

func makeSign(value float64, sign float64) float64 {
	if sign < 0 {
		return -math.Abs(value)
	}

	return math.Abs(value)
}

func (p *Planner) fillNormalizedPlan() {
	rows := p.ExperimentsNumber
	cols := 1 + len(p.Combinations)
	m := make([][]float64, rows)

	// Инициализация и заполнение первого столбца x0 (все  единицы).
	for i := range m {
		m[i] = make([]float64, cols)
		m[i][0] = 1.0
	}

	// Заполнение значений, соответствующих переменным x1...xn.
	// Если идти по столбцам, то в первом столбце чередуем -1 и 1 через один,
	// то есть -1, 1, -1, 1, и так далее.
	// Во втором столбце через 2, то есть -1, -1, 1, 1.
	limit := 1
	for j := 1; j <= len(p.Factors); j++ {
		value := -1.0
		left := limit

		for i := range m {
			if left == 0 {
				left = limit
				value *= -1.0
			}

			m[i][j] = value
			left--
		}

		limit *= 2
	}

	// Заполнение значений-комбинаций исходных переменных.
	// Это значения вида x12 = x1 * x2, x23, и так далее.
	combinationNumber := len(p.Factors)
	for j := len(p.Factors) + 1; j < cols; j++ {
		for i := range m {
			value := 1.0
			// Идем по всей комбинации.
			for _, index := range p.Combinations[combinationNumber] {
				value *= m[i][index+1]
			}

			m[i][j] = value
		}

		combinationNumber++
	}

	p.NormalizedPlan = m
}

func (p *Planner) fillPlan() {
	rows := p.ExperimentsNumber
	cols := len(p.Factors)
	m := make([][]float64, rows)

	// Инициализация и заполнение первого столбца x0 (все  единицы).
	for i := range m {
		m[i] = make([]float64, cols)
	}

	for i := range m {
		for j := range m[i] {
			m[i][j] = p.Factors[j].Decode(p.NormalizedPlan[i][j+1])
		}
	}

	p.Plan = m
}

func (p *Planner) fillPlanTable() {
	rows := p.ExperimentsNumber
	cols := 1 + len(p.Combinations)
	m := make([][]float64, rows)

	// Инициализация и заполнение первого столбца x0 (все  единицы).
	for i := range m {
		m[i] = make([]float64, cols)
		m[i][0] = 1.0
	}

	// Заполнение значений, соответствующих переменным x1...xn.
	// Если идти по столбцам, то в первом столбце чередуем -1 и 1 через один,
	// то есть -1, 1, -1, 1, и так далее.
	// Во втором столбце через 2, то есть -1, -1, 1, 1.
	limit := 1
	for j := 1; j <= len(p.Factors); j++ {
		value := -1.0
		left := limit

		for i := range m {
			if left == 0 {
				left = limit
				value *= -1.0
			}

			m[i][j] = value
			left--
		}

		limit *= 2
	}

	// Заполнение значений-комбинаций исходных переменных.
	// Это значения вида x12 = x1 * x2, x23, и так далее.
	combinationNumber := len(p.Factors)
	for j := len(p.Factors) + 1; j < cols; j++ {
		for i := range m {
			value := 1.0
			// Идем по всей комбинации.
			for _, index := range p.Combinations[combinationNumber] {
				value *= m[i][index+1]
			}

			m[i][j] = value
		}

		combinationNumber++
	}

	p.PlanTable = PlanTable{
		Header: p.makeHeaderFromCombinations(p.Combinations),
		Matrix: m,
	}
}

func (p *Planner) CreateTable() [][]string {
	table := [][]string{p.makeHeaderFromCombinations(p.Combinations)}
	for i := range p.ExperimentsNumber {
		row := make([]string, len(p.NormalizedPlan))
		for j := range p.NormalizedPlan[i] {
			row[j] = fmt.Sprintf("%.1f", p.NormalizedPlan[i][j])
		}

		row = append(row, fmt.Sprintf("%.3f", p.Results[i]))

		yLin := 0.0
		for j := range p.LinearCoefficients {
			yLin += p.NormalizedPlan[i][j] * p.NormalizedCoefficients[j]
		}

		row = append(row, fmt.Sprintf("%.3f", yLin))
		row = append(row, fmt.Sprintf("%.3f", math.Abs(yLin-p.Results[i])))

		yNonLin := 0.0
		for j := range p.NormalizedCoefficients {
			yNonLin += p.NormalizedPlan[i][j] * p.NormalizedCoefficients[j]
		}

		row = append(row, fmt.Sprintf("%.3f", yNonLin))
		row = append(row, fmt.Sprintf("%.3f", math.Abs(yNonLin-p.Results[i])))

		table = append(table, row)
	}

	return table
}

func (p *Planner) MakeLinearEquation() string {
	variables := []string{""}
	for _, combination := range p.Combinations {
		name := new(strings.Builder)
		name.WriteString("x")

		for _, index := range combination {
			name.WriteString(strconv.Itoa(index + 1))
		}

		variables = append(variables, name.String())
	}

	result := new(strings.Builder)
	for i := range p.LinearCoefficients {
		result.WriteString(fmt.Sprintf("(%.3f)%s", p.LinearCoefficients[i], variables[i]))
		if i == len(p.LinearCoefficients)-1 {
			continue
		}

		result.WriteString(" + ")
	}

	return result.String()
}

func (p *Planner) MakeLinearNormalizedEquation() string {
	variables := []string{""}
	for _, combination := range p.Combinations {
		name := new(strings.Builder)
		name.WriteString("x")

		for _, index := range combination {
			name.WriteString(strconv.Itoa(index + 1))
		}

		variables = append(variables, name.String())
	}

	result := new(strings.Builder)
	for i := range p.LinearCoefficients {
		result.WriteString(fmt.Sprintf("(%.3f)%s", p.NormalizedCoefficients[i], variables[i]))
		if i == len(p.LinearCoefficients)-1 {
			continue
		}

		result.WriteString(" + ")
	}

	return result.String()
}

func (p *Planner) MakeNonLinearNormalizedEquation() string {
	variables := []string{""}
	for _, combination := range p.Combinations {
		name := new(strings.Builder)
		name.WriteString("x")

		for _, index := range combination {
			name.WriteString(strconv.Itoa(index + 1))
		}

		variables = append(variables, name.String())
	}

	result := new(strings.Builder)
	for i := range p.NormalizedCoefficients {
		result.WriteString(fmt.Sprintf("(%.3f)%s", p.NormalizedCoefficients[i], variables[i]))
		if i == len(p.NormalizedCoefficients)-1 {
			continue
		}

		result.WriteString(" + ")
		if (i+1)%10 == 0 {
			result.WriteRune('\n')
		}
	}

	return result.String()
}

func (p *Planner) MakeNonLinearEquation() string {
	variables := []string{""}
	for _, combination := range p.Combinations {
		name := new(strings.Builder)
		name.WriteString("x")

		for _, index := range combination {
			name.WriteString(strconv.Itoa(index + 1))
		}

		variables = append(variables, name.String())
	}

	result := new(strings.Builder)
	for i := range p.NonLinearCoefficients {
		result.WriteString(fmt.Sprintf("(%.3f)%s", p.NonLinearCoefficients[i], variables[i]))
		if i == len(p.NonLinearCoefficients)-1 {
			continue
		}

		result.WriteString(" + ")
		if (i+1)%10 == 0 {
			result.WriteRune('\n')
		}
	}

	return result.String()
}

func (p *Planner) AddResultRow(result float64, params ...float64) []string {
	normalized := []float64{1.0}
	for i, param := range params {
		normalized = append(normalized, p.Factors[i].Encode(param))
	}

	combinationNumber := len(p.Factors)
	for j := len(p.Factors); j < len(p.Combinations); j++ {
		value := 1.0
		// Идем по всей комбинации.
		for _, index := range p.Combinations[combinationNumber] {
			value *= normalized[index+1]
		}

		normalized = append(normalized, value)

		combinationNumber++
	}

	row := make([]string, len(normalized))
	for i := range normalized {
		row[i] = fmt.Sprintf("%.1f", normalized[i])
	}

	row = append(row, fmt.Sprintf("%.3f", result))

	yLin := 0.0
	for i := range p.LinearCoefficients {
		yLin += normalized[i] * p.NormalizedCoefficients[i]
	}

	row = append(row, fmt.Sprintf("%.3f", yLin))
	row = append(row, fmt.Sprintf("%.3f", math.Abs(yLin-result)))

	yNonLin := 0.0
	for i := range normalized {
		yNonLin += normalized[i] * p.NormalizedCoefficients[i]
	}

	row = append(row, fmt.Sprintf("%.3f", yNonLin))
	row = append(row, fmt.Sprintf("%.3f", math.Abs(yNonLin-result)))

	return row
}

func (p *Planner) makeHeaderFromCombinations(combinations [][]int) []string {
	header := []string{"x0"}
	for _, combination := range combinations {
		name := new(strings.Builder)
		name.WriteString("x")

		for _, index := range combination {
			name.WriteString(strconv.Itoa(index + 1))
		}

		header = append(header, name.String())
	}

	header = append(header, "y")
	header = append(header, "y_лин")
	header = append(header, "|y - y_лин|")
	header = append(header, "y_нелин")
	header = append(header, "|y - y_нелин|")

	return header
}

type PlanTable struct {
	Header []string
	Matrix [][]float64
}

type factor interface {
	Encode(value float64) float64
	Decode(encoded float64) float64

	Max() float64
	Min() float64
}
