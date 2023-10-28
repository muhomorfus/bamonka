package utils

func Min(values ...int) int {
	if len(values) == 0 {
		return 0
	}

	m := values[0]
	for _, v := range values {
		if v < m {
			m = v
		}
	}

	return m
}
