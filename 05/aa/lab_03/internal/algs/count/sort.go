package count

import (
	"git.parasha.space/go/libs/counter"
	"lab_03/internal/utils"
)

type Count struct {
	stat counter.DummyCounter
}

func New() *Count {
	return &Count{}
}

func (m *Count) MemStat() int {
	return m.stat.Max()
}

func (m *Count) Sort(a []int) []int {
	m.stat.Reset()

	return m.sort(a)
}

func (m *Count) sort(a []int) []int {
	m.stat.Add(m.memory(a))
	m.stat.Sub(m.memory(a))

	counts := make([]int, utils.Max(&m.stat, a...)+1)
	for _, e := range a {
		counts[e]++
	}

	res := make([]int, 0, len(a))
	for i, c := range counts {
		for j := 0; j < c; j++ {
			res = append(res, i)
		}
	}

	return res
}

func (m *Count) memory(a []int) int {
	return utils.SizeOf[[]int]() + // a header
		utils.SizeOf[int]()*len(a) + // a elements
		utils.SizeOf[[]int]() + // counts
		utils.SizeOf[int]()*(utils.Max(&m.stat, a...)+1) + // counts elements
		utils.SizeOf[int]() + // e
		utils.SizeOf[[]int]() + // res header
		utils.SizeOf[int]()*len(a) + // res elements
		utils.SizeOf[int]() + // i
		utils.SizeOf[int]() + // c
		utils.SizeOf[int]() // j
}
