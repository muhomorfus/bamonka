package algs

type Sorter interface {
	Sort([]int) []int
	MemStat() int
}
