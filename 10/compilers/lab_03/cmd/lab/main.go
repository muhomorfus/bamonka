package main

import (
	"fmt"
	"os"

	"lab_03/internal/ast"
	"lab_03/internal/token"
)

func main() {
	tokens, err := token.ReadTokens("data/program.txt")
	if err != nil {
		fmt.Println("cannot read tokens: ", err)
		os.Exit(1)
	}

	root, err := ast.New(tokens)
	if err != nil {
		fmt.Println("cannot build ast: ", err)
		os.Exit(1)
	}

	if err := ast.Show(root); err != nil {
		fmt.Println("cannot render ast: ", err)
		os.Exit(1)
	}
}
