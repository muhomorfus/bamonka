package damerau_levenshtein

import (
	"git.parasha.space/go/libs/counter"
	"lab_01/internal/utils"
)

type DamerauLevenshtein struct {
	stat counter.DummyCounter
}

func New() *DamerauLevenshtein {
	return &DamerauLevenshtein{}
}

func (l *DamerauLevenshtein) MemStat() int {
	return l.stat.Max()
}

func (l *DamerauLevenshtein) Diff(s1, s2 string) int {
	l.stat.Reset()
	return l.diff([]rune(s1), []rune(s2))
}

func (l *DamerauLevenshtein) diff(s1, s2 []rune) int {
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
			r := 0
			if s1[i-1] != s2[j-1] {
				r++
			}

			m[i][j] = utils.Min(
				&l.stat,
				m[i][j-1]+1,
				m[i-1][j]+1,
				m[i-1][j-1]+r,
			)

			if i > 1 && j > 1 {
				if s1[i-1] == s2[j-2] && s1[i-2] == s2[j-1] {
					m[i][j] = utils.Min(
						&l.stat,
						m[i][j],
						m[i-2][j-2]+1,
					)
				}
			}
		}
	}

	return m[len(m)-1][len(m[0])-1]
}

func (l *DamerauLevenshtein) memory(s1, s2 []rune) int {
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
		utils.SizeOf[int]() + // loop variable
		utils.SizeOf[int]() // r
}
