package main

import (
	"fmt"

	"lab_02/internal/grammar"
)

func main() {
	g, err := grammar.NewFromFile("data/make_greibach_form/in.txt")
	if err != nil {
		panic(err)
	}

	fmt.Println(g.MakeGreibachForm().String())
}
