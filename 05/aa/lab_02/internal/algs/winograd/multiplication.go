package winograd

import (
	"git.parasha.space/go/libs/counter"
	"lab_02/internal/algs"
	"lab_02/internal/utils"
)

type Winograd struct {
	stat counter.DummyCounter
}

func New() *Winograd {
	return &Winograd{}
}

func (m *Winograd) MemStat() int {
	return m.stat.Max()
}

func (m *Winograd) Multiply(a, b [][]int) ([][]int, error) {
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

	r := m.rowCoefficients(a)
	c := m.colCoefficients(b)

	for i := 0; i < len(res); i++ {
		for j := 0; j < len(res[0]); j++ {
			res[i][j] = res[i][j] - r[i] - c[j]

			for k := 0; k < len(b)/2; k++ {
				res[i][j] = res[i][j] + (a[i][2*k]+b[2*k+1][j])*(a[i][2*k+1]+b[2*k][j])
			}
		}
	}

	if len(b)%2 != 0 {
		for i := 0; i < len(res); i++ {
			for j := 0; j < len(res[0]); j++ {
				res[i][j] = res[i][j] + a[i][len(a[0])-1]*b[len(a[0])-1][j]
			}
		}
	}

	return res, nil
}

func (m *Winograd) rowCoefficients(a [][]int) []int {
	m.stat.Add(m.memoryRowCoefficients(a))
	defer m.stat.Sub(m.memoryRowCoefficients(a))

	c := make([]int, len(a))

	for i := 0; i < len(a); i++ {
		for j := 0; j < len(a[0])/2; j++ {
			c[i] = c[i] + a[i][2*j]*a[i][2*j+1]
		}
	}

	return c
}

func (m *Winograd) colCoefficients(a [][]int) []int {
	m.stat.Add(m.memoryColCoefficients(a))
	defer m.stat.Sub(m.memoryColCoefficients(a))

	c := make([]int, len(a[0]))

	for i := 0; i < len(a[0]); i++ {
		for j := 0; j < len(a)/2; j++ {
			c[i] = c[i] + a[2*j][i]*a[2*j+1][i]
		}
	}

	return c
}

func (m *Winograd) memory(a, b [][]int) int {
	return utils.SizeOf[[][]int]()*2 + // input arguments
		utils.SizeOf[[][]int]() + // output parameter
		utils.SizeOf[error]() + // output
		utils.SizeOf[[][]int]() + // res
		utils.SizeOf[[]int]()*len(a) + // res slices
		utils.SizeOf[int]()*len(a)*len(b[0]) + // res elements
		utils.SizeOf[[]int]() + // r
		utils.SizeOf[[]int]() + // c
		utils.SizeOf[int]() + // i
		utils.SizeOf[int]() + // i
		utils.SizeOf[int]() + // j
		utils.SizeOf[int]() + // k
		utils.SizeOf[int]() + // i
		utils.SizeOf[int]() // j
}

func (m *Winograd) memoryRowCoefficients(a [][]int) int {
	return utils.SizeOf[[][]int]() + // input arguments
		utils.SizeOf[[]int]() + // output parameter
		utils.SizeOf[[]int]() + // c
		utils.SizeOf[int]()*len(a) + // c elements
		utils.SizeOf[int]() + // i
		utils.SizeOf[int]() // j
}

func (m *Winograd) memoryColCoefficients(a [][]int) int {
	return utils.SizeOf[[][]int]() + // input arguments
		utils.SizeOf[[]int]() + // output parameter
		utils.SizeOf[[]int]() + // c
		utils.SizeOf[int]()*len(a[0]) + // c elements
		utils.SizeOf[int]() + // i
		utils.SizeOf[int]() // j
}
