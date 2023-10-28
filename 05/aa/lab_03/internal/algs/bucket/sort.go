package bucket

import (
	"git.parasha.space/go/libs/counter"
	"lab_03/internal/utils"
)

type Bucket struct {
	stat    counter.DummyCounter
	buckets int
}

func New(buckets int) *Bucket {
	return &Bucket{buckets: buckets}
}

func (m *Bucket) MemStat() int {
	return m.stat.Max()
}

func (m *Bucket) Sort(a []int) []int {
	m.stat.Reset()

	return m.sort(a)
}

func (m *Bucket) getBucketFunc(max, min int) (func(int) int, int) {
	m.stat.Add(m.memoryGetBucketFunc())
	defer m.stat.Sub(m.memoryGetBucketFunc())

	d := max - min + 1
	n := m.buckets
	if n > d {
		n = d
	}

	size := d / n
	if d%n != 0 {
		size++
	}

	return func(k int) int {
		m.stat.Add(m.memoryFunc())
		defer m.stat.Sub(m.memoryFunc())

		k -= min
		bucket := k / size
		if bucket > n-1 {
			bucket = n - 1
		}

		return bucket
	}, n
}

func (m *Bucket) isSorted(a []int) bool {
	m.stat.Add(m.memoryIsSorted())
	defer m.stat.Sub(m.memoryIsSorted())

	for i := 1; i < len(a); i++ {
		if a[i] < a[i-1] {
			return false
		}
	}

	return true
}

func (m *Bucket) sort(a []int) []int {
	m.stat.Add(m.memory(a))
	m.stat.Sub(m.memory(a))

	if len(a) <= 1 {
		return a
	}

	f, n := m.getBucketFunc(utils.Max(&m.stat, a...), utils.Min(&m.stat, a...))

	buckets := make([][]int, n)

	for _, e := range a {
		buckets[f(e)] = append(buckets[f(e)], e)
	}

	for i := range buckets {
		if !m.isSorted(buckets[i]) {
			buckets[i] = m.sort(buckets[i])
		}
	}

	res := make([]int, 0, len(a))
	for _, b := range buckets {
		for _, e := range b {
			res = append(res, e)
		}
	}

	return res
}

func (m *Bucket) memory(a []int) int {
	return utils.SizeOf[[]int]() + // a header
		utils.SizeOf[int]()*len(a) + // a elements
		utils.SizeOf[func(int) int]() + // f
		utils.SizeOf[int]() + // n
		utils.SizeOf[[]int]()*m.buckets + // buckets
		utils.SizeOf[int]()*len(a) + // buckets elements
		utils.SizeOf[int]() + // e
		utils.SizeOf[int]() + // i
		utils.SizeOf[[]int]() + // res header
		utils.SizeOf[int]()*len(a) + // res elements
		utils.SizeOf[int]() + // b
		utils.SizeOf[int]() // e
}

func (m *Bucket) memoryGetBucketFunc() int {
	return utils.SizeOf[int]() + // max
		utils.SizeOf[func(int) int]() + // return value
		utils.SizeOf[int]() + // return value
		utils.SizeOf[int]() // size
}

func (m *Bucket) memoryFunc() int {
	return utils.SizeOf[int]() + // k
		utils.SizeOf[int]() + // return value
		utils.SizeOf[int]() // bucket
}

func (m *Bucket) memoryIsSorted() int {
	return utils.SizeOf[[]int]() + // a
		utils.SizeOf[bool]() + // return value
		utils.SizeOf[int]() // i
}
