package damerau_levenshtein

import (
	"git.parasha.space/go/libs/counter"
	"lab_01/internal/utils"
	"math"
)

type DamerauLevenshteinRecursiveCached struct {
	stat counter.DummyCounter
}

func New() *DamerauLevenshteinRecursiveCached {
	return &DamerauLevenshteinRecursiveCached{}
}

func (l *DamerauLevenshteinRecursiveCached) MemStat() int {
	return l.stat.Max()
}

func (l *DamerauLevenshteinRecursiveCached) Diff(s1, s2 string) int {
	l.stat.Reset()
	return l.diff([]rune(s1), []rune(s2))
}

func (l *DamerauLevenshteinRecursiveCached) diff(s1, s2 []rune) int {
	l.stat.Add(l.memory(s1, s2))
	defer l.stat.Sub(l.memory(s1, s2))

	m := make([][]int, len(s1)+1)
	for i := range m {
		m[i] = make([]int, len(s2)+1)
	}

	for i := range m[0] {
		m[0][i] = i
	}

	for i := range m {
		m[i][0] = i
	}

	for i := 1; i < len(m); i++ {
		for j := 1; j < len(m[i]); j++ {
			m[i][j] = math.MaxInt
		}
	}

	return l.diff1(s1, s2, m)
}

func (l *DamerauLevenshteinRecursiveCached) memory(s1, s2 []rune) int {
	return utils.SizeOf[[]rune]() + // s1
		utils.SizeOf[[]rune]() + // s2
		utils.SizeOf[int]() + // return value
		utils.SizeOf[[][]int]() + // matrix header (m)
		(len(s1)+1)*utils.SizeOf[[]int]() + // matrix rows headers
		(len(s1)+1)*(len(s2)+1) + utils.SizeOf[int]() + // matrix elements headers
		utils.SizeOf[int]() + // loop variable
		utils.SizeOf[int]() + // loop variable
		utils.SizeOf[int]() + // loop variable
		utils.SizeOf[int]() + // loop variable
		utils.SizeOf[int]() // loop variable
}

func (l *DamerauLevenshteinRecursiveCached) diff1(s1, s2 []rune, cache [][]int) int {
	l.stat.Add(l.memory1(s1, s2))
	defer l.stat.Sub(l.memory1(s1, s2))

	if cache[len(s1)][len(s2)] != math.MaxInt {
		return cache[len(s1)][len(s2)]
	}

	if len(s1) == 0 || len(s2) == 0 {
		cache[len(s1)][len(s2)] = len(s1) + len(s2)
	}

	r := 0
	if s1[len(s1)-1] != s2[len(s2)-1] {
		r++
	}

	cache[len(s1)][len(s2)] = utils.Min(
		&l.stat,
		l.diff1(s1, s2[:len(s2)-1], cache)+1,
		l.diff1(s1[:len(s1)-1], s2, cache)+1,
		l.diff1(s1[:len(s1)-1], s2[:len(s2)-1], cache)+r,
	)

	if len(s1) > 1 && len(s2) > 1 {
		if s1[len(s1)-1] == s2[len(s2)-2] && s1[len(s1)-2] == s2[len(s2)-1] {
			cache[len(s1)][len(s2)] = utils.Min(
				&l.stat,
				cache[len(s1)][len(s2)],
				l.diff1(s1[:len(s1)-2], s2[:len(s2)-2], cache)+1,
			)
		}
	}

	return cache[len(s1)][len(s2)]
}

func (l *DamerauLevenshteinRecursiveCached) memory1(_, _ []rune) int {
	return utils.SizeOf[[]rune]() + // s1
		utils.SizeOf[[]rune]() + // s2
		utils.SizeOf[int]() + // return value
		utils.SizeOf[[][]int]() + // matrix header (m)
		utils.SizeOf[int]() // r
}
