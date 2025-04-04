package grammar

import (
	"slices"
)

const (
	empty = "ε"
)

type Rule struct {
	NonTerminal  string
	Combinations [][]string
}

type Grammar struct {
	NonTerminals []string
	Terminals    []string
	Start        string

	Rules []Rule
}

func (g *Grammar) RemoveLeftRecursion() *Grammar {
	n := len(g.Rules)
	for i := 0; i < n; i++ {
		for j := 0; j < i; j++ {
			newCombinations := make([][]string, 0, len(g.Rules[i].Combinations))
			for _, iCombination := range g.Rules[i].Combinations {
				if iCombination[0] != g.Rules[j].NonTerminal {
					newCombinations = append(newCombinations, iCombination)
				} else {
					for _, jCombination := range g.Rules[j].Combinations {
						newCombinations = append(newCombinations, append(slices.Clone(jCombination), iCombination[1:]...))
					}
				}
			}

			g.Rules[i].Combinations = newCombinations
		}

		recursiveCombinations := make([][]string, 0, len(g.Rules[i].Combinations))
		nonRecursiveCombinations := make([][]string, 0, len(g.Rules[i].Combinations))
		for _, iCombination := range g.Rules[i].Combinations {
			if iCombination[0] == g.Rules[i].NonTerminal {
				recursiveCombinations = append(recursiveCombinations, iCombination)
			} else {
				nonRecursiveCombinations = append(nonRecursiveCombinations, iCombination)
			}
		}

		if len(recursiveCombinations) == 0 {
			continue
		}

		newNonTerminal := g.Rules[i].NonTerminal + "'"

		g.Rules[i].Combinations = make([][]string, 0, len(nonRecursiveCombinations))
		for _, nonRecursiveCombination := range nonRecursiveCombinations {
			g.Rules[i].Combinations = append(g.Rules[i].Combinations, append(nonRecursiveCombination, newNonTerminal))
		}

		newRule := Rule{
			NonTerminal:  newNonTerminal,
			Combinations: make([][]string, 0, len(recursiveCombinations)),
		}
		for _, recursiveCombination := range recursiveCombinations {
			newRule.Combinations = append(newRule.Combinations, append(recursiveCombination[1:], newNonTerminal))
		}
		newRule.Combinations = append(newRule.Combinations, []string{empty})

		g.Rules = append(g.Rules, newRule)
		g.NonTerminals = append(g.NonTerminals, newNonTerminal)
	}

	return g
}

func (g *Grammar) MakeGreibachForm() *Grammar {
	a := g.createA()
	b := g.createB()
	r := g.createR()
	q := g.createQ()

	aSolved := solveA(b, q)
	qSolved := solveQ(r, q)

	newRules := make([]Rule, 0, len(g.Rules))

	for i, nonTerminal := range a {
		newRules = append(newRules, Rule{
			NonTerminal:  nonTerminal,
			Combinations: aSolved[i],
		})

		g.removeTerminals(newRules[len(newRules)-1].Combinations)
	}

	for i := range q {
		for j, nonTerminal := range q[i] {
			newRules = append(newRules, Rule{
				NonTerminal:  nonTerminal,
				Combinations: g.resolveLeadingA(newRules, qSolved[i][j]),
			})

			g.removeTerminals(newRules[len(newRules)-1].Combinations)
		}
	}

	g.Rules = newRules

	return g
}

func (g *Grammar) removeTerminals(combinations [][]string) {
	for i, combination := range combinations {
		for j := len(combination) - 1; j > 0; j-- {
			if slices.Contains(g.NonTerminals, combination[j]) {
				continue
			}

			oldTerminal := combinations[i][j]
			newNonTerminal := combinations[i][j] + "'"
			combinations[i][j] = newNonTerminal

			if !slices.Contains(g.NonTerminals, newNonTerminal) {
				g.NonTerminals = append(g.NonTerminals, newNonTerminal)
				g.Rules = append(g.Rules, Rule{
					NonTerminal:  newNonTerminal,
					Combinations: [][]string{{oldTerminal}},
				})
			}
		}
	}
}

func (g *Grammar) resolveLeadingA(rules []Rule, combinations [][]string) [][]string {
	newCombinations := make([][]string, 0, len(combinations))

	for _, combination := range combinations {
		if !slices.Contains(g.NonTerminals, combination[0]) {
			newCombinations = append(newCombinations, combination)
			continue
		}

		for _, rule := range rules {
			if rule.NonTerminal != combination[0] {
				continue
			}

			for _, ruleCombination := range rule.Combinations {
				newCombinations = append(newCombinations, append(slices.Clone(ruleCombination), combination[1:]...))
			}
		}
	}

	return newCombinations
}

func (g *Grammar) createA() []string {
	return slices.Clone(g.NonTerminals)
}

func (g *Grammar) createB() [][][]string {
	b := make([][][]string, len(g.NonTerminals))

	for i, rule := range g.Rules {
		b[i] = make([][]string, 0, len(rule.Combinations))

		for _, combination := range rule.Combinations {
			if !slices.Contains(g.NonTerminals, combination[0]) {
				b[i] = append(b[i], combination)
			}
		}
	}

	return b
}

func (g *Grammar) createQ() [][]string {
	q := make([][]string, len(g.NonTerminals))
	newNonTerminals := makeNewNonTerminals(g.NonTerminals, len(g.NonTerminals)*len(g.NonTerminals))

	k := 0
	for i := range g.NonTerminals {
		q[i] = make([]string, len(g.NonTerminals))
		for j := range g.NonTerminals {
			q[i][j] = newNonTerminals[k]
			k++
		}
	}

	g.NonTerminals = append(g.NonTerminals, newNonTerminals...)
	return q
}

func (g *Grammar) createR() [][][][]string {
	r := make([][][][]string, len(g.NonTerminals))

	for i := range g.NonTerminals {
		r[i] = make([][][]string, len(g.NonTerminals))
	}

	for j, rule := range g.Rules {
		for _, combination := range rule.Combinations {
			for i, nonTerminal := range g.NonTerminals {
				if nonTerminal == combination[0] {
					r[i][j] = append(r[i][j], combination[1:])
				}
			}
		}
	}

	return r
}

func solveA(b [][][]string, q [][]string) [][][]string {
	result := make([][][]string, len(q[0]))

	for j := range q[0] {
		for i := range b {
			for _, combination := range b[i] {
				result[j] = append(result[j], append(combination, q[i][j]))
			}
		}
	}

	for i := range b {
		for _, combination := range b[i] {
			result[i] = append(result[i], combination)
		}
	}

	return result
}

func solveQ(r [][][][]string, q [][]string) [][][][]string {
	result := make([][][][]string, len(q))
	for k := range result {
		result[k] = make([][][]string, len(q[0]))

		for j := range q[0] {
			for i := range r[k] {
				for _, combination := range r[k][i] {
					result[k][j] = append(result[k][j], append(combination, q[i][j]))
				}
			}
		}

		for i := range r[k] {
			for _, combination := range r[k][i] {
				result[k][i] = append(result[k][i], combination)
			}
		}
	}

	return result
}

func makeNewNonTerminals(nonTerminals []string, count int) []string {
	variants := []string{"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"}

	variants = slices.DeleteFunc(variants, func(s string) bool {
		return slices.Contains(nonTerminals, s)
	})

	return variants[len(variants)-count:]
}
