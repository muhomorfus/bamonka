package grammar

import (
	"fmt"
	"strings"

	"github.com/samber/lo"
)

type Part struct {
	Coefficient []string
	Variable    string
}

type Equation struct {
	Left  string
	Right []Part
}

type System struct {
	Variables []string
	Equations []Equation
}

func (s *System) String() string {
	result := ""

	for _, e := range s.Equations {
		result += e.Left + " = "

		for i := 0; i < len(e.Right); i++ {
			result += joinCoefficients(e.Right[i].Coefficient)
			result += e.Right[i].Variable

			if i < len(e.Right)-1 {
				result += " + "
			}
		}

		result += "\n"
	}

	return result
}

func (s *System) Solve() string {
	for i := 0; i < len(s.Equations); i++ {
		fmt.Println(i)
		fmt.Println(s.String())
		fmt.Println()

		// Записать уравнение для Xi = alpha Xi + beta
		alpha := make([]string, 0)
		beta := make([]Part, 0)
		for _, part := range s.Equations[i].Right {
			if part.Variable == s.Equations[i].Left {
				alpha = part.Coefficient
			} else {
				beta = append(beta, part)
			}
		}

		// Тогда Xi = alpha*(beta) или же alpha* объединяется со всеми элементами beta
		// Представляем Xi
		alphaStar := ""
		if len(alpha) > 0 {
			alphaStar = joinCoefficients(alpha) + "*"
		}
		newRightPart := make([]Part, 0)
		for _, part := range beta {
			newRightPart = append(newRightPart, Part{
				Variable: part.Variable,
				Coefficient: lo.Map(part.Coefficient, func(item string, _ int) string {
					return alphaStar + item
				}),
			})
		}

		s.Equations[i].Right = newRightPart

		// Нужно пройтись по всем остальным уравнениям и продставить новое значение Xi
		for j := range s.Equations {
			if i == j {
				continue
			}

			newRightPart := make([]Part, 0)
			for _, part := range s.Equations[j].Right {
				if part.Variable == s.Equations[i].Left {
					for _, toReplacePart := range s.Equations[i].Right {
						newRightPart = append(newRightPart, Part{
							Variable: toReplacePart.Variable,
							Coefficient: lo.Map(toReplacePart.Coefficient, func(item string, _ int) string {
								return joinCoefficients(part.Coefficient) + item
							}),
						})
					}
				} else {
					newRightPart = append(newRightPart, part)
				}
			}

			s.Equations[j].Right = newRightPart

			// Сократить выражения с одинаковыми переменными
			partsMap := make(map[string]Part)
			for _, part := range s.Equations[j].Right {
				parts, ok := partsMap[part.Variable]
				if !ok {
					partsMap[part.Variable] = part
				} else {
					partsMap[part.Variable] = Part{
						Variable:    part.Variable,
						Coefficient: append(parts.Coefficient, part.Coefficient...),
					}
				}
			}

			newRightPart = make([]Part, 0)
			for _, part := range partsMap {
				newRightPart = append(newRightPart, part)
			}

			s.Equations[j].Right = newRightPart
		}
	}

	for _, equation := range s.Equations {
		if equation.Left == start {
			return cleanup(strings.ReplaceAll(
				strings.Join(equation.Right[0].Coefficient, "+"),
				"+", "|",
			), "()")
		}
	}

	return ""
}

func joinCoefficients(coefficients []string) string {
	if len(coefficients) == 1 {
		if len(coefficients[0]) == 1 {
			return coefficients[0]
		}
	}

	return "(" + strings.Join(coefficients, "+") + ")"
}

func cleanup(str string, toCleanStrs ...string) string {
	for _, toClean := range toCleanStrs {
		str = strings.ReplaceAll(str, toClean, "")
	}

	return str
}
