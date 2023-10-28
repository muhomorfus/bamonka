package main

import (
	"lab_01/internal/generator"
	"log"
)

func main() {
	if err := generator.GenerateToCSV(); err != nil {
		log.Fatalf("Generating data: %w", err)
	}

	log.Println("Successfully generated all data")
}
