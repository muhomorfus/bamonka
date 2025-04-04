package main

import (
	"fmt"

	"lab_02/internal/grammar"
)

func main() {
	g, err := grammar.NewFromFile("data/remove_left_recursion/in.txt")
	if err != nil {
		panic(err)
	}

	fmt.Println(g.RemoveLeftRecursion().String())
}
