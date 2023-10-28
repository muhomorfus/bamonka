package bruteforce

import (
	"lab_07/internal/graph"
	"math"
)

type Manager struct{}

func New() *Manager {
	return &Manager{}
}

func allPaths(n int) [][]int {
	arr := make([]int, n)
	for i := range arr {
		arr[i] = i
	}

	var shuffles [][]int
	var f func([]int, int)

	f = func(arr []int, n int) {
		if n == 1 {
			tmp := make([]int, len(arr))
			copy(tmp, arr)
			shuffles = append(shuffles, tmp)
		} else {
			for i := 0; i < n; i++ {
				f(arr, n-1)
				if n%2 == 1 {
					tmp := arr[i]
					arr[i] = arr[n-1]
					arr[n-1] = tmp
				} else {
					tmp := arr[0]
					arr[0] = arr[n-1]
					arr[n-1] = tmp
				}
			}
		}
	}

	f(arr, len(arr))

	return shuffles
}

func (m *Manager) MinPath(g *graph.Graph) ([]int, int, bool) {
	paths := allPaths(g.GetSize())
	minLen := math.MaxInt
	var minPath []int

	for _, path := range paths {
		l, ok := g.LenPath(path)
		if !ok {
			continue
		}

		if l < minLen {
			minLen = l
			minPath = path
		}
	}

	return minPath, minLen, minLen != math.MaxInt
}
