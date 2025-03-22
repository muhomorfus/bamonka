package fa

import (
	"fmt"
	"maps"
)

type Node interface {
	BuildNFA(nameGenerator func() int) *NFA
}

type Star struct {
	Child Node
}

func (s *Star) BuildNFA(nameGenerator func() int) *NFA {
	childNFA := s.Child.BuildNFA(nameGenerator)

	i := &State{
		Name:  nameGenerator(),
		Start: true,
	}

	f := &State{
		Name:   nameGenerator(),
		Finish: true,
	}

	states := []*State{i, f}
	states = append(states, childNFA.States...)

	transitions := maps.Clone(childNFA.Transitions)
	transitions[i.Name] = map[string][]*State{
		"": {
			childNFA.Start(),
			f,
		},
	}

	if m := transitions[childNFA.Finish().Name]; m != nil {
		transitions[childNFA.Finish().Name][""] = append(transitions[childNFA.Finish().Name][""], f, childNFA.Start())
	} else {
		transitions[childNFA.Finish().Name] = map[string][]*State{
			"": {f, childNFA.Start()},
		}
	}

	childNFA.Start().Start = false
	childNFA.Finish().Finish = false

	return &NFA{
		States:      states,
		Transitions: transitions,
	}
}

type Concat struct {
	Left, Right Node
}

func (c *Concat) BuildNFA(nameGenerator func() int) *NFA {
	leftNFA := c.Left.BuildNFA(nameGenerator)
	rightNFA := c.Right.BuildNFA(nameGenerator)

	var states []*State
	states = append(states, leftNFA.States...)

	transitions := maps.Clone(leftNFA.Transitions)
	if m := transitions[leftNFA.Finish().Name]; m != nil {
		for k, v := range rightNFA.Transitions[rightNFA.Start().Name] {
			transitions[leftNFA.Finish().Name][k] = append(transitions[leftNFA.Finish().Name][k], v...)
		}
	} else {
		transitions[leftNFA.Finish().Name] = rightNFA.Transitions[rightNFA.Start().Name]
	}
	for _, state := range rightNFA.States {
		if state.Start {
			continue
		}

		states = append(states, state)
		transitions[state.Name] = rightNFA.Transitions[state.Name]
	}

	leftNFA.Finish().Finish = false

	return &NFA{
		States:      states,
		Transitions: transitions,
	}
}

type Alternative struct {
	Left, Right Node
}

func (a *Alternative) BuildNFA(nameGenerator func() int) *NFA {
	leftNFA := a.Left.BuildNFA(nameGenerator)
	rightNFA := a.Right.BuildNFA(nameGenerator)

	i := &State{
		Name:  nameGenerator(),
		Start: true,
	}

	f := &State{
		Name:   nameGenerator(),
		Finish: true,
	}

	states := []*State{i, f}
	states = append(states, leftNFA.States...)
	states = append(states, rightNFA.States...)

	transitions := maps.Clone(leftNFA.Transitions)
	for k, v := range rightNFA.Transitions {
		transitions[k] = v
	}
	transitions[i.Name] = map[string][]*State{
		"": {
			leftNFA.Start(),
			rightNFA.Start(),
		},
	}

	if m := transitions[leftNFA.Finish().Name]; m != nil {
		transitions[leftNFA.Finish().Name][""] = append(transitions[leftNFA.Finish().Name][""], f)
	} else {
		transitions[leftNFA.Finish().Name] = map[string][]*State{
			"": {f},
		}
	}

	if m := transitions[rightNFA.Finish().Name]; m != nil {
		transitions[rightNFA.Finish().Name][""] = append(transitions[rightNFA.Finish().Name][""], f)
	} else {
		transitions[rightNFA.Finish().Name] = map[string][]*State{
			"": {f},
		}
	}

	leftNFA.Start().Start = false
	leftNFA.Finish().Finish = false

	rightNFA.Start().Start = false
	rightNFA.Finish().Finish = false

	return &NFA{
		States:      states,
		Transitions: transitions,
	}
}

type Symbol struct {
	Symbol string
}

func (s *Symbol) BuildNFA(nameGenerator func() int) *NFA {
	i := &State{
		Name:  nameGenerator(),
		Start: true,
	}

	f := &State{
		Name:   nameGenerator(),
		Finish: true,
	}

	return &NFA{
		States: []*State{i, f},
		Transitions: map[int]map[string][]*State{
			i.Name: {
				s.Symbol: []*State{f},
			},
		},
	}
}

func MakeAST(re string) (Node, error) {
	postfix := infixToPostfix(re)

	root, tail, err := makeAST(postfix)
	if err != nil {
		return nil, fmt.Errorf("invalid syntax: %q", re)
	}

	if len(tail) != 0 {
		return nil, fmt.Errorf("invalid syntax: %q", re)
	}

	return root, nil
}

func makeAST(postfix []rune) (Node, []rune, error) {
	if len(postfix) == 0 {
		return nil, nil, nil
	}

	switch postfix[len(postfix)-1] {
	case '*':
		if len(postfix) <= 1 {
			return nil, nil, fmt.Errorf("invalid postfix: %q", postfix)
		}

		child, tail, err := makeAST(postfix[:len(postfix)-1])
		if err != nil {
			return nil, nil, err
		}

		return &Star{
			Child: child,
		}, tail, nil
	case '.':
		if len(postfix) <= 1 {
			return nil, nil, fmt.Errorf("invalid postfix: %q", postfix)
		}

		right, tail, err := makeAST(postfix[:len(postfix)-1])
		if err != nil {
			return nil, nil, err
		}

		left, tail, err := makeAST(tail)
		if err != nil {
			return nil, nil, err
		}

		return &Concat{
			Left:  left,
			Right: right,
		}, tail, nil
	case '|':
		if len(postfix) <= 1 {
			return nil, nil, fmt.Errorf("invalid postfix: %q", postfix)
		}

		right, tail, err := makeAST(postfix[:len(postfix)-1])
		if err != nil {
			return nil, nil, err
		}

		left, tail, err := makeAST(tail)
		if err != nil {
			return nil, nil, err
		}

		return &Alternative{
			Left:  left,
			Right: right,
		}, tail, nil
	default:
		if postfix[len(postfix)-1] == 'ε' {
			return &Symbol{
				Symbol: "",
			}, postfix[:len(postfix)-1], nil
		}

		return &Symbol{
			Symbol: string(postfix[len(postfix)-1]),
		}, postfix[:len(postfix)-1], nil
	}
}

func infixToPostfix(infix string) []rune {
	specials := map[rune]int{
		'*': 3,
		'.': 2,
		'|': 1,
	}

	postfix := []rune{}
	stack := []rune{}

	lastSymbol := false
	for _, r := range []rune(infix) {
		switch {
		case r == '(':
			if lastSymbol {
				for len(stack) > 0 && specials['.'] <= specials[stack[len(stack)-1]] {
					postfix = append(postfix, stack[len(stack)-1])
					stack = stack[:len(stack)-1]
				}
				stack = append(stack, '.')
			}

			stack = append(stack, r)
			lastSymbol = false
		case r == ')':
			for stack[len(stack)-1] != '(' {
				postfix = append(postfix, stack[len(stack)-1])
				stack = stack[:len(stack)-1]
			}
			stack = stack[:len(stack)-1]
			lastSymbol = true
		case specials[r] > 0:
			for len(stack) > 0 && specials[r] <= specials[stack[len(stack)-1]] {
				postfix = append(postfix, stack[len(stack)-1])
				stack = stack[:len(stack)-1]
			}
			stack = append(stack, r)
			lastSymbol = r == '*'
		default:
			if lastSymbol {
				for len(stack) > 0 && specials['.'] <= specials[stack[len(stack)-1]] {
					postfix = append(postfix, stack[len(stack)-1])
					stack = stack[:len(stack)-1]
				}
				stack = append(stack, '.')
			}

			postfix = append(postfix, r)
			lastSymbol = true
		}
	}

	for len(stack) > 0 {
		postfix = append(postfix, stack[len(stack)-1])
		stack = stack[:len(stack)-1]
	}

	return []rune(string(postfix))
}
