package heap

import (
	"git.parasha.space/go/libs/counter"
	"lab_03/internal/utils"
)

type Heap struct {
	stat counter.DummyCounter
}

func New() *Heap {
	return &Heap{}
}

func (m *Heap) MemStat() int {
	return m.stat.Max()
}

func (m *Heap) Sort(a []int) []int {
	m.stat.Reset()

	return m.sort(a)
}

func (m *Heap) heapify(a []int, i, heapSize int) []int {
	m.stat.Add(m.memoryHeapify())
	m.stat.Sub(m.memoryHeapify())

	left := i<<1 + 1
	right := i<<1 + 2

	largest := i
	if left < heapSize && a[left] > a[i] {
		largest = left
	}

	if right < heapSize && a[right] > a[largest] {
		largest = right
	}

	if i != largest {
		tmp := a[i]
		a[i] = a[largest]
		a[largest] = tmp
		a = m.heapify(a, largest, heapSize)
	}

	return a
}

func (m *Heap) sort(a []int) []int {
	m.stat.Add(m.memory(a))
	m.stat.Sub(m.memory(a))

	heapSize := len(a)

	for i := len(a)/2 - 1; i >= 0; i-- {
		a = m.heapify(a, i, heapSize)
	}

	for i := len(a) - 1; i >= 1; i-- {
		tmp := a[0]
		a[0] = a[i]
		a[i] = tmp

		heapSize--
		a = m.heapify(a, 0, heapSize)
	}

	return a
}

func (m *Heap) memory(a []int) int {
	return utils.SizeOf[[]int]() + // a header
		utils.SizeOf[int]()*len(a) + // a elements
		utils.SizeOf[int]() + // heapSize
		utils.SizeOf[int]() + // i
		utils.SizeOf[int]() + // i
		utils.SizeOf[int]() // tmp
}

func (m *Heap) memoryHeapify() int {
	return utils.SizeOf[[]int]() + // a header
		utils.SizeOf[int]() + // i
		utils.SizeOf[int]() + // heapSize
		utils.SizeOf[[]int]() + // return value
		utils.SizeOf[int]() + // left
		utils.SizeOf[int]() + // right
		utils.SizeOf[int]() + // largest
		utils.SizeOf[int]() // tmp
}
