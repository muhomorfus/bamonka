package winograd_improved

import (
	"git.parasha.space/go/libs/counter"
	"lab_02/internal/algs"
	"lab_02/internal/utils"
)

type WinogradImproved struct {
	stat counter.DummyCounter
}

func New() *WinogradImproved {
	return &WinogradImproved{}
}

func (m *WinogradImproved) MemStat() int {
	return m.stat.Max()
}

func (m *WinogradImproved) Multiply(a, b [][]int) ([][]int, error) {
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

	odd := len(b)%2 != 0
	len2 := len(res[0])

	for i := 0; i < len(res); i++ {
		for j := 0; j < len2; j++ {
			res[i][j] -= r[i] + c[j]

			for k := 0; k < len(b)-1; k += 2 {
				res[i][j] += (a[i][k] + b[k+1][j]) * (a[i][k+1] + b[k][j])
			}

			if odd {
				res[i][j] += a[i][len(b)-1] * b[len(b)-1][j]
			}
		}
	}

	return res, nil
}

func (m *WinogradImproved) rowCoefficients(a [][]int) []int {
	m.stat.Add(m.memoryRowCoefficients(a))
	defer m.stat.Sub(m.memoryRowCoefficients(a))

	c := make([]int, len(a))

	len2 := len(a[0]) >> 1
	for i := 0; i < len(a); i++ {
		for j := 0; j < len2; j++ {
			j2 := j << 1
			c[i] += a[i][j2] * a[i][j2+1]
		}
	}

	return c
}

func (m *WinogradImproved) colCoefficients(a [][]int) []int {
	m.stat.Add(m.memoryColCoefficients(a))
	defer m.stat.Sub(m.memoryColCoefficients(a))

	c := make([]int, len(a[0]))

	len1 := len(a[0])
	len2 := len(a) >> 1
	for i := 0; i < len1; i++ {
		for j := 0; j < len2; j++ {
			j2 := j << 1
			c[i] += a[j2][i] * a[j2+1][i]
		}
	}

	return c
}

func (m *WinogradImproved) memory(a, b [][]int) int {
	return utils.SizeOf[[][]int]()*2 + // input arguments
		utils.SizeOf[[][]int]() + // output parameter
		utils.SizeOf[error]() + // output
		utils.SizeOf[[][]int]() + // res
		utils.SizeOf[[]int]()*len(a) + // res slices
		utils.SizeOf[int]()*len(a)*len(b[0]) + // res elements
		utils.SizeOf[[]int]() + // r
		utils.SizeOf[[]int]() + // c
		utils.SizeOf[bool]() + // odd
		utils.SizeOf[int]() + // len2
		utils.SizeOf[int]() + // i
		utils.SizeOf[int]() + // i
		utils.SizeOf[int]() + // j
		utils.SizeOf[int]() // k
}

func (m *WinogradImproved) memoryRowCoefficients(a [][]int) int {
	return utils.SizeOf[[][]int]() + // input arguments
		utils.SizeOf[[]int]() + // output parameter
		utils.SizeOf[[]int]() + // c
		utils.SizeOf[int]()*len(a) + // c elements
		utils.SizeOf[int]() + // len2
		utils.SizeOf[int]() + // i
		utils.SizeOf[int]() + // j
		utils.SizeOf[int]() // j2
}

func (m *WinogradImproved) memoryColCoefficients(a [][]int) int {
	return utils.SizeOf[[][]int]() + // input arguments
		utils.SizeOf[[]int]() + // output parameter
		utils.SizeOf[[]int]() + // c
		utils.SizeOf[int]()*len(a[0]) + // c elements
		utils.SizeOf[int]() + // len1
		utils.SizeOf[int]() + // len2
		utils.SizeOf[int]() + // i
		utils.SizeOf[int]() + // j
		utils.SizeOf[int]() // j2
}
