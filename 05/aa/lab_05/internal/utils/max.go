package utils

import "git.parasha.space/go/libs/counter"

func Max(stat *counter.DummyCounter, values ...int) int {
	stat.Add(sizeOfMin(len(values)))
	defer stat.Sub(sizeOfMin(len(values)))

	if len(values) == 0 {
		return 0
	}

	m := values[0]
	for _, v := range values {
		if v > m {
			m = v
		}
	}

	return m
}

func sizeOfMax(n int) int {
	return SizeOf[[]int]() + // values header
		n*SizeOf[int]() + // value content
		SizeOf[int]() + // return value
		SizeOf[int]() // loop variable
}
