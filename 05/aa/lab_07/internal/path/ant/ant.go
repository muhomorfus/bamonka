package ant

import (
	"lab_07/internal/graph"
	"math"
	"math/rand"
	"time"
)

const (
	t0               = 0.1
	probabilityElite = 0.2
)

type Manager struct {
	n           int
	alpha, beta float64
	k           float64
}

func New(n int, alpha, beta, k float64) *Manager {
	return &Manager{
		n:     n,
		alpha: alpha,
		beta:  beta,
		k:     k,
	}
}

func (m *Manager) MinPath(g *graph.Graph) ([]int, int, bool) {
	minLen := math.MaxInt
	var minPath []int

	pheromones := newPheromones(g)
	visibles := newVisibles(g)

	for t := 0; t < m.n; t++ {
		c := newColony(g, m.alpha, m.beta, m.k)

		c.evaporate(pheromones)

		for _, v := range c.ants {
			ok := v.findPath(g, pheromones, visibles)
			if !ok {
				return nil, 0, false
			}

			if v.len < minLen && len(v.path) == g.GetSize() {
				minLen = v.len
				minPath = v.path
			}
		}

		c.correct(pheromones)
	}

	if len(minPath) == 0 {
		return nil, 0, false
	}

	return minPath, minLen, true
}

type ant struct {
	elite bool
	path  []int
	len   int
	alpha float64
	beta  float64
}

func (a *ant) getPos() int {
	if len(a.path) == 0 {
		return -1
	}

	return a.path[len(a.path)-1]
}

func (a *ant) move(pos, len int) {
	a.path = append(a.path, pos)
	a.len += len
}

func randVariant(ps []float64, p float64) int {
	idx := 0
	s := 0.0

	rand.Seed(time.Now().UnixNano())
	randPoint := rand.Float64() * p
	for s < randPoint {
		s += ps[idx]
		idx++
	}

	return idx
}

func (a *ant) trip(vertices []int, pheromones [][]float64, visibles [][]float64) (int, bool) {
	probabilities := make([]float64, 0)
	probability := 0.0

	for _, v := range vertices {
		if visibles[a.getPos()][v] != -1 {
			p := math.Pow(visibles[a.getPos()][v], a.beta) * math.Pow(pheromones[a.getPos()][v], a.alpha)

			probabilities = append(probabilities, p)
			probability += p
		}
	}

	if probability <= 0 {
		return 0, false
	}

	maxProbability := 0.0
	idx := 0
	for i, v := range probabilities {
		v /= probability
		if v > maxProbability {
			maxProbability = v
			idx = vertices[i]
		}
	}

	if a.elite {
		return idx, true
	}

	return vertices[randVariant(probabilities, probability)-1], true
}

func rm(a []int, e int) []int {
	res := make([]int, 0, len(a))
	for _, v := range a {
		if v != e {
			res = append(res, v)
		}
	}

	return res
}

func (a *ant) findPath(g *graph.Graph, pheromones [][]float64, visibles [][]float64) bool {
	vertices := make([]int, 0, g.GetSize())

	for i := 0; i < g.GetSize(); i++ {
		if i != a.getPos() {
			vertices = append(vertices, i)
		}
	}

	for len(vertices) != 0 {
		next, ok := a.trip(vertices, pheromones, visibles)
		if !ok {
			return false
		}

		dst, _ := g.GetDistance(a.getPos(), next)
		a.move(next, dst)
		vertices = rm(vertices, next)
	}

	return true
}

type colony struct {
	ants []ant
	q    float64
	e    float64
}

func newColony(g *graph.Graph, alpha, beta, ev float64) *colony {
	ants := make([]ant, g.GetSize())

	for i := range ants {
		rand.Seed(time.Now().UnixNano())

		ants[i] = ant{
			elite: rand.Float64() < probabilityElite,
			path:  []int{i},
			alpha: alpha,
			beta:  beta,
		}
	}

	return &colony{
		ants: ants,
		q:    getQuantity(g),
		e:    ev,
	}
}

func getQuantity(g *graph.Graph) float64 {
	q := 0.0

	for i := 0; i < g.GetSize(); i++ {
		for j := 0; j < g.GetSize(); j++ {
			dst, ok := g.GetDistance(i, j)
			if ok {
				q += float64(dst)
			}
		}
	}

	q /= float64(g.GetSize())

	return q
}

func newPheromones(g *graph.Graph) [][]float64 {
	m := make([][]float64, g.GetSize())

	for i := range m {
		m[i] = make([]float64, g.GetSize())

		for j := range m[i] {
			m[i][j] = t0
		}
	}

	return m
}

func newVisibles(g *graph.Graph) [][]float64 {
	m := make([][]float64, g.GetSize())

	for i := range m {
		m[i] = make([]float64, g.GetSize())

		for j := range m[i] {
			m[i][j] = -1

			dst, ok := g.GetDistance(i, j)
			if ok {
				m[i][j] = 1 / float64(dst)
			}
		}
	}

	return m
}

func (c *colony) evaporate(p [][]float64) {
	for i := range p {
		for j := range p[i] {
			p[i][j] = (1 - c.e) * p[i][j]
		}
	}

	c.correct(p)
}

func (c *colony) increase(p [][]float64) {
	for _, v := range c.ants {
		for i := 0; i < len(v.path)-1; i++ {
			p[v.path[i]][v.path[i+1]] += c.q / float64(v.len)
		}
	}
}

func (c *colony) correct(p [][]float64) {
	for i := range p {
		for j := range p[i] {
			if p[i][j] < t0 {
				p[i][j] = t0
			}
		}
	}
}
