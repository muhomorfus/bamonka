package graph

import (
	"fmt"
	"os"
)

type Graph struct {
	size  int
	table [][]int
}

func FromFile(filename string) (*Graph, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, fmt.Errorf("ошибка открытия файла: %w", err)
	}

	g := new(Graph)

	if _, err := fmt.Fscan(file, &g.size); err != nil {
		return nil, fmt.Errorf("ошибка чтения размера графа: %w", err)
	}

	g.table = make([][]int, g.size)
	for i := 0; i < g.size; i++ {
		g.table[i] = make([]int, g.size)

		for j := 0; j < g.size; j++ {
			if _, err := fmt.Fscan(file, &g.table[i][j]); err != nil {
				return nil, fmt.Errorf("ошибка чтения элемента матрицы: %w", err)
			}
		}
	}

	return g, nil
}

func (g *Graph) GetSize() int {
	return g.size
}

func (g *Graph) GetDistance(u, v int) (int, bool) {
	if v < 0 || u < 0 {
		return 0, false
	}

	if v >= g.size || u >= g.size {
		return 0, false
	}

	if u == v {
		return 0, false
	}

	return g.table[u][v], g.table[u][v] != -1
}

func (g *Graph) ValidPath(path []int) bool {
	for i := 0; i < len(path)-1; i++ {
		if _, ok := g.GetDistance(path[i], path[i+1]); !ok {
			return false
		}
	}

	return true
}

func (g *Graph) ValidFullPath(path []int) bool {
	if len(path) != g.size {
		return false
	}

	m := make(map[int]struct{}, len(path))
	for _, e := range path {
		m[e] = struct{}{}
	}

	if len(m) != g.size {
		return false
	}

	return g.ValidPath(path)
}

func (g *Graph) LenPath(path []int) (int, bool) {
	res := 0
	for i := 0; i < len(path)-1; i++ {
		d, ok := g.GetDistance(path[i], path[i+1])
		if !ok {
			return 0, false
		}

		res += d
	}

	return res, true
}
