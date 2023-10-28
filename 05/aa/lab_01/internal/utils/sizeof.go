package utils

import "unsafe"

func SizeOf[T any]() int {
	var v T

	return int(unsafe.Sizeof(v))
}
