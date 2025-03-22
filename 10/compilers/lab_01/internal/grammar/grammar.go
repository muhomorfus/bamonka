package grammar

import (
	"fmt"
	"os"
	"strings"

	"github.com/samber/lo"
)

const nonTerminals = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

const (
	start = "S"

	empty = "ε"
)

type Combination struct {
	Terminals   string
	NonTerminal *string
}

type Rule struct {
	NonTerminal  string
	Combinations []Combination
}

type Grammar struct {
	Rules []Rule
}

func NewFromFile(path string) (*Grammar, error) {
	data, err := os.ReadFile(path)
	if err != nil {
		return nil, fmt.Errorf("failed to read file %s: %w", path, err)
	}

	rulesRaw := strings.Split(strings.Replace(strings.TrimSpace(string(data)), " ", "", -1), "\n")
	rules := make([]Rule, 0, len(rulesRaw))

	for _, ruleRaw := range rulesRaw {
		rule := Rule{}
		splitted := strings.Split(ruleRaw, "->")
		if len(splitted) != 2 {
			return nil, fmt.Errorf("invalid rule format: %q", ruleRaw)
		}

		if len(splitted[0]) != 1 {
			return nil, fmt.Errorf("invalid rule format: %q", ruleRaw)
		}

		rule.NonTerminal = splitted[0]

		combinationsRaw := strings.Split(splitted[1], "|")
		for _, combinationRaw := range combinationsRaw {
			var terminalPart, nonTerminalPart string

			if strings.ContainsAny(combinationRaw, nonTerminals) {
				if len(combinationRaw) <= 1 {
					return nil, fmt.Errorf("invalid combination: %q", combinationRaw)
				}

				terminalPart = combinationRaw[:len(combinationRaw)-1]
				nonTerminalPart = string(combinationRaw[len(combinationRaw)-1])
			} else {
				terminalPart = combinationRaw
			}

			if terminalPart == "" {
				return nil, fmt.Errorf("need terminals: %q", combinationRaw)
			}

			if strings.ContainsAny(terminalPart, nonTerminals) {
				return nil, fmt.Errorf("invalid combination: %q", combinationRaw)
			}

			if nonTerminalPart != "" && (!strings.ContainsAny(nonTerminalPart, nonTerminals) || len(nonTerminalPart) != 1) {
				return nil, fmt.Errorf("invalid combination: %q", combinationRaw)
			}

			rule.Combinations = append(rule.Combinations, Combination{
				Terminals:   terminalPart,
				NonTerminal: lo.EmptyableToPtr(nonTerminalPart),
			})
		}

		rules = append(rules, rule)
	}

	return &Grammar{
		Rules: rules,
	}, nil
}

func (g *Grammar) MakeSystem() *System {
	equations := make([]Equation, 0, len(g.Rules))
	variables := make([]string, 0, len(g.Rules))

	for _, rule := range g.Rules {
		right := make([]Part, 0, len(g.Rules)+1)

		part := Part{}
		for _, combination := range rule.Combinations {
			if combination.NonTerminal == nil {
				part.Coefficient = append(part.Coefficient, combination.Terminals)
			}
		}
		if len(part.Coefficient) > 0 {
			right = append(right, part)
		}

		for i := range g.Rules {
			part := Part{}

			for _, combination := range rule.Combinations {
				if combination.NonTerminal != nil && *combination.NonTerminal == g.Rules[i].NonTerminal {
					part.Coefficient = append(part.Coefficient, combination.Terminals)
				}
			}

			part.Variable = g.Rules[i].NonTerminal

			if len(part.Coefficient) > 0 {
				right = append(right, part)
			}
		}

		equations = append(equations, Equation{
			Left:  rule.NonTerminal,
			Right: right,
		})
		variables = append(variables, rule.NonTerminal)
	}

	return &System{
		Variables: variables,
		Equations: equations,
	}
}
