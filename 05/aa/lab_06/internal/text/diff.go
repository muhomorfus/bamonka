package text

import "lab_06/internal/utils"

func diff(s1, s2 string) int {
	return diff1([]rune(s1), []rune(s2))
}

func diff1(s1, s2 []rune) int {
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
				m[i][j-1]+1,
				m[i-1][j]+1,
				m[i-1][j-1]+r,
			)

			if i > 1 && j > 1 {
				if s1[i-1] == s2[j-2] && s1[i-2] == s2[j-1] {
					m[i][j] = utils.Min(
						m[i][j],
						m[i-2][j-2]+1,
					)
				}
			}
		}
	}

	return m[len(m)-1][len(m[0])-1]
}
