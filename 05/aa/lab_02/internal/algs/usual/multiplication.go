package usual

import (
	"git.parasha.space/go/libs/counter"
	"lab_02/internal/algs"
	"lab_02/internal/utils"
)

type Usual struct {
	stat counter.DummyCounter
}

func New() *Usual {
	return &Usual{}
}

func (m *Usual) MemStat() int {
	return m.stat.Max()
}

func (m *Usual) Multiply(a, b [][]int) ([][]int, error) {
	m.stat.Reset()
	m.stat.Add(m.memory(a, b))
	defer m.stat.Sub(m.memory(a, b))

	if len(a[0]) != len(b) {
		return nil, algs.ErrInvalidArgsSize
	}

	res := make([][]int, len(a))
	for i := range res {
		res[i] = make([]int, len(b[0]))
	}

	for i := range res {
		for j := range res[0] {
			for k := range b {
				res[i][j] = res[i][j] + a[i][k]*b[k][j]
			}
		}
	}

	return res, nil
}

func (m *Usual) memory(a, b [][]int) int {
	return utils.SizeOf[[][]int]()*2 + // input arguments
		utils.SizeOf[[][]int]() + // output parameter
		utils.SizeOf[error]() + // output
		utils.SizeOf[[][]int]() + // res
		utils.SizeOf[[]int]()*len(a) + // res slices
		utils.SizeOf[int]()*len(a)*len(b[0]) + // res elements
		utils.SizeOf[int]() + // i
		utils.SizeOf[int]() + // i
		utils.SizeOf[int]() + // j
		utils.SizeOf[int]() // k
}
