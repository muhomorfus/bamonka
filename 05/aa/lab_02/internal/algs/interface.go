package algs

import "errors"

var (
	ErrInvalidArgsSize = errors.New("размеры матриц не подходят для умножения")
)

type Multiplier interface {
	Multiply([][]int, [][]int) ([][]int, error)
	MemStat() int
}
