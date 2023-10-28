package utils

import (
	"errors"
	"fmt"
	"math"
	"math/rand"
	"os"
)

var (
	ErrInvalidSize = errors.New("некорректный размер массива")
)

func InputArrayFromFile(filename string) ([]int, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, fmt.Errorf("ошибка открытия файла: %w", err)
	}
	defer file.Close()

	var n int
	if _, err := fmt.Fscan(file, &n); err != nil {
		return nil, fmt.Errorf("невалидное количество элементов': %w", err)
	}

	if n <= 0 {
		return nil, ErrInvalidSize
	}

	res := make([]int, n)
	for i := 0; i < n; i++ {
		if _, err := fmt.Fscan(file, &res[i]); err != nil {
			return nil, fmt.Errorf("невалидный элемент массива: %w", err)
		}
	}

	return res, err
}

func GenerateRandomArray(n int, mod int) []int {
	res := make([]int, n)

	for i := range res {
		res[i] = rand.Int() % mod
	}

	return res
}

func GenerateSortedArray(n int) []int {
	res := make([]int, n)

	for i := range res {
		res[i] = i
	}

	return res
}

func GenerateEqualsArray(n int, v int) []int {
	res := make([]int, n)

	for i := range res {
		res[i] = v
	}

	return res
}

func GenerateStrangeArray(n int, mod int) []int {
	res := make([]int, n)

	k := n/int(math.Log2(float64(mod))) + 1

	i := 0
	var v int
	vPrev := -mod

	for i < len(res) {
		for j := 0; j < k && i < len(res); j++ {
			res[i] = v
			i++
		}

		vNew := (v - vPrev) / 2
		vPrev = v
		v = vNew
	}

	return res
}
