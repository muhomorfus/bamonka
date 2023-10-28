package main

import (
	"context"
	"fmt"
	"os"

	"git.iu7.bmstu.ru/iu7-kostritsky/iu7-db-course-project-2023-knyazhevaleksey-backend/internal/app"
)

func main() {
	a := app.New()

	if err := a.Init(); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}

	if err := a.Benchmark(context.Background()); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
}
