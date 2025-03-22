package fa

import (
	"fmt"
	"log"
	"os"
	"os/exec"
	"slices"
)

type State struct {
	Name int

	Start  bool
	Finish bool
}

type NFA struct {
	States      []*State
	Transitions map[int]map[string][]*State
}

func NewNFA(ast Node) *NFA {
	counter := 0
	return ast.BuildNFA(func() int {
		counter++
		return counter
	})
}

func (n *NFA) CheckString(s string, debug bool) bool {
	states := n.epsClosure(n.Start())

	for _, symbol := range s {
		if debug {
			log.Printf("Checking %q", string(symbol))
		}

		var move []*State
		for _, state := range states {
			if tr := n.Transitions[state.Name]; tr != nil {
				canGo := tr[string(symbol)]
				if debug {
					for _, cs := range canGo {
						log.Printf("Can go from %d to %d via %q", state.Name, cs.Name, string(symbol))
					}
				}

				move = append(move, canGo...)
			}
		}

		if len(move) == 0 {
			return false
		}

		states = n.epsClosure(move...)
	}

	for _, state := range states {
		if state.Finish {
			return true
		}
	}

	return false
}

func (n *NFA) epsClosure(states ...*State) []*State {
	stack := slices.Clone(states)
	result := slices.Clone(states)

	for len(stack) > 0 {
		t := stack[len(stack)-1]
		stack = stack[:len(stack)-1]

		if tr := n.Transitions[t.Name]; tr != nil {
			for _, u := range tr[""] {
				exists := false
				for _, s := range result {
					if s.Name == u.Name {
						exists = true
					}
				}

				if !exists {
					result = append(result, u)
				}

				stack = append(stack, u)
			}
		}
	}

	return result
}

func (n *NFA) Start() *State {
	for _, state := range n.States {
		if state.Start {
			return state
		}
	}

	return nil
}

func (n *NFA) Finish() *State {
	for _, state := range n.States {
		if state.Finish {
			return state
		}
	}

	return nil
}

func (n *NFA) Show(filename string) error {
	file, err := os.Create(filename + ".dot")
	if err != nil {
		return err
	}
	defer file.Close()

	_, err = file.WriteString("digraph NFA {\n")
	if err != nil {
		return err
	}
	_, err = file.WriteString("rankdir=LR;\n")
	if err != nil {
		return err
	}

	if len(n.States) > 0 {
		_, err = fmt.Fprintf(file, "start [shape=point];\n")
		if err != nil {
			return err
		}
		_, err = fmt.Fprintf(file, "start -> \"%d\" [label=\"\"];\n", n.States[0].Name)
		if err != nil {
			return err
		}
	}

	for _, s := range n.States {
		if s.Finish {
			_, err := fmt.Fprintf(file, "\"%d\" [label=\"%d\" peripheries=2];\n", s.Name, s.Name)
			if err != nil {
				return err
			}
		} else {
			_, err := fmt.Fprintf(file, "\"%d\" [label=\"%d\"];\n", s.Name, s.Name)
			if err != nil {
				return err
			}
		}
	}

	for src, transitions := range n.Transitions {
		for a, dsts := range transitions {
			if a == "" {
				a = "ε"
			}

			for _, dst := range dsts {
				_, err := fmt.Fprintf(file, "\"%d\" -> \"%d\" [label=\"%s\"];\n", src, dst.Name, a)
				if err != nil {
					return err
				}
			}
		}
	}

	_, err = file.WriteString("}\n")
	if err != nil {
		return err
	}

	cmd := exec.Command("dot", "-Tpng", filename+".dot", "-o", filename+".png")
	err = cmd.Run()
	if err != nil {
		return err
	}

	//cmd = exec.Command("open", filename+".png")
	return nil
}
