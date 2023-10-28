package main

import (
	"fmt"
	"github.com/steelseries/golisp"
	_ "lab_04/polynomial"
	"os"
	"path/filepath"
)

func loadLispFile(filename string, fileInfo os.FileInfo, err error) error {
	match, err := filepath.Match("*.lsp", fileInfo.Name())
	if err != nil {
		return fmt.Errorf("match path: %w", err)
	}

	if match {
		_, err = golisp.ProcessFile(filename)
		if err != nil {
			return fmt.Errorf("process file '%s': %w", fileInfo.Name(), err)
		}
	}

	return nil
}

func loadLispCode() error {
	err := filepath.Walk("./lisp", loadLispFile)
	if err != nil {
		return fmt.Errorf("walk: %w", err)
	}

	return nil
}

func main() {
	if err := loadLispCode(); err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	prompt := "ставки на спорт 1 икс бет > "
	golisp.LoadHistoryFromFile(".golisp_history")
	lastInput := ""
	for true {
		input := *golisp.ReadLine(&prompt)
		if input == "(quit)" {
			return
		} else if input != "" {
			if input != lastInput {
				golisp.AddHistory(input)
			}
			lastInput = input
			result, err := golisp.ParseAndEval(input)
			if err != nil {
				fmt.Println("Error:", err)
			} else {
				fmt.Println(golisp.PrintString(result))
			}
		}
	}
}

// Искать ВСЕ делители заданного полинома
