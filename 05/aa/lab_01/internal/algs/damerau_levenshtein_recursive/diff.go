package damerau_levenshtein

import (
	"git.parasha.space/go/libs/counter"
	"lab_01/internal/utils"
)

type DamerauLevenshteinRecursive struct {
	stat counter.DummyCounter
}

func New() *DamerauLevenshteinRecursive {
	return &DamerauLevenshteinRecursive{}
}

func (l *DamerauLevenshteinRecursive) MemStat() int {
	return l.stat.Max()
}

func (l *DamerauLevenshteinRecursive) Diff(s1, s2 string) int {
	l.stat.Reset()
	return l.diff([]rune(s1), []rune(s2))
}

func (l *DamerauLevenshteinRecursive) diff(s1, s2 []rune) int {
	l.stat.Add(l.memory(s1, s2))
	defer l.stat.Sub(l.memory(s1, s2))

	if len(s1) == 0 || len(s2) == 0 {
		return len(s1) + len(s2)
	}

	r := 0
	if s1[len(s1)-1] != s2[len(s2)-1] {
		r++
	}

	result := utils.Min(
		&l.stat,
		l.diff(s1, s2[:len(s2)-1])+1,
		l.diff(s1[:len(s1)-1], s2)+1,
		l.diff(s1[:len(s1)-1], s2[:len(s2)-1])+r,
	)

	if len(s1) > 1 && len(s2) > 1 {
		if s1[len(s1)-1] == s2[len(s2)-2] && s1[len(s1)-2] == s2[len(s2)-1] {
			result = utils.Min(
				&l.stat,
				result,
				l.diff(s1[:len(s1)-2], s2[:len(s2)-2])+1,
			)
		}
	}

	return result
}

func (l *DamerauLevenshteinRecursive) memory(_, _ []rune) int {
	return utils.SizeOf[[]rune]() + // s1
		utils.SizeOf[[]rune]() + // s2
		utils.SizeOf[int]() + // return value
		utils.SizeOf[int]() + // r
		utils.SizeOf[int]() // result
}
