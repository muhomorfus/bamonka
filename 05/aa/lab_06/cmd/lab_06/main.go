package main

import (
	"bufio"
	"fmt"
	"lab_06/internal/beer"
	"lab_06/internal/query"
	"lab_06/internal/query/config"
	"lab_06/internal/text"
	"os"
)

func main() {
	dict, err := beer.FromJson("config/config.json")
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	txt, err := text.New("config/russian.txt")
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	m := query.New(config.DefaultConfig, dict, txt)

	fmt.Print("> ")

	reader := bufio.NewReader(os.Stdin)
	q, err := reader.ReadString('\n')
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	res, err := m.Answer(q)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	for i, e := range res {
		fmt.Printf("%2d. %s - %.1f%%\n", i+1, e.Name, e.Strength)
	}
}
