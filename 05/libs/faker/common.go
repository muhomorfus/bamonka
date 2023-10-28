package faker

import "math/rand"

// Bool generates random bool value.
func Bool() bool {
	return rand.Int()%2 == 1
}

// Int64WithLimits generates random int64 number with limits [from; to).
func Int64WithLimits(from, to int64) int64 {
	return rand.Int63()%(to-from) + from
}

// IntWithLimits generates random int number with limits [from; to).
func IntWithLimits(from, to int) int {
	return rand.Int()%(to-from) + from
}

// ArrayElement element selects random element from array.
func ArrayElement[T any](arr []T) T {
	return arr[rand.Int()%len(arr)]
}
