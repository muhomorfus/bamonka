package main

import (
	"fmt"
	"os"

	"lab_01/internal/fa"
	"lab_01/internal/grammar"
)

const debug = true

func main() {
	g, err := grammar.NewFromFile("data/in.txt")
	if err != nil {
		panic(err)
	}

	s := g.MakeSystem()

	fmt.Println("System:")
	fmt.Println(s)

	re := s.Solve()
	fmt.Println("Reg expr:")
	fmt.Println(re)

	//re := "((0(00)*1)(1(00)*1)*1(00)*01|(0(00)*1)(1(00)*1)*0|0(00)*01|1)(((10)(00)*1|0)(1(00)*1)*1(00)*01|((10)(00)*1|0)(1(00)*1)*0|(10)(00)*01|11)*"

	ast, err := fa.MakeAST(re)
	if err != nil {
		fmt.Println("Error make AST:", err)
		os.Exit(1)
	}

	nfa := fa.NewNFA(ast)
	_ = nfa.Show("data/nfa")

	var in string
	fmt.Print("\nInput string to check: ")
	if _, err := fmt.Scanln(&in); err != nil {
		fmt.Println("Error reading input:", err)
		os.Exit(1)
	}

	ok := nfa.CheckString(in, debug)
	fmt.Println("\nCheck result:", ok)
}
