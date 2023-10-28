package utils

import (
	"errors"
	"fmt"
	"math/rand"
	"os"
)

var (
	ErrInvalidSize = errors.New("некорректный размер матрицы")
)

func InputMatrixFromFile(filename string) ([][]int, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, fmt.Errorf("ошибка открытия файла: %w", err)
	}
	defer file.Close()

	var rows, cols int
	if _, err := fmt.Fscan(file, &rows); err != nil {
		return nil, fmt.Errorf("невалидное количество строк: %w", err)
	}

	if _, err := fmt.Fscan(file, &cols); err != nil {
		return nil, fmt.Errorf("невалидное количество cтолбцов: %w", err)
	}

	if rows <= 0 || cols <= 0 {
		return nil, ErrInvalidSize
	}

	m := make([][]int, rows)
	for i := 0; i < rows; i++ {
		m[i] = make([]int, cols)

		for j := 0; j < cols; j++ {
			if _, err := fmt.Fscan(file, &m[i][j]); err != nil {
				return nil, fmt.Errorf("невалидный элемент матрицы: %w", err)
			}
		}
	}

	return m, nil
}

func PrintMatrix(m [][]int) {
	for _, r := range m {
		for _, e := range r {
			fmt.Printf("%4d ", e)
		}

		fmt.Println()
	}
}

func GenMatrix(n int) [][]int {
	res := make([][]int, n)
	for i := range res {
		res[i] = make([]int, n)

		for j := range res[i] {
			res[i][j] = rand.Int() % 100
		}
	}

	return res
}
