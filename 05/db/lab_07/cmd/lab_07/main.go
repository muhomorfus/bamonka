package main

import (
	"fmt"
	_ "github.com/lib/pq"
	"lab_07/internal/app"
	"os"
)

func main() {
	dsn := "host=localhost user=user password=password dbname=postgres sslmode=disable"

	a, err := app.New(dsn)
	if err != nil {
		fmt.Printf("Ошибка инициализации приложения: %s\n", err)
		os.Exit(1)
	}

	if err := a.Run(); err != nil {
		fmt.Printf("Ошибка запуска приложения: %s\n", err)
		os.Exit(1)
	}
}
