package compute

import (
	"errors"

	"lab_01/func_info"
)

func getLessAndBiggerIndexes(fis []func_info.FuncInfo, x float64) (int, int) {
	for i, fi := range fis {
		if fi.X > x {
			return i - 1, i
		}
	}

	length := len(fis)
	return length - 1, length
}

func GetConfiguration(fis []func_info.FuncInfo, x float64, n int) ([]func_info.FuncInfo, error) {
	n--
	if len(fis) < n {
		return nil, errors.New("not enough info about function")
	}

	left, right := getLessAndBiggerIndexes(fis, x)
	atEnd := len(fis) - right
	atBegin := left + 1

	if atEnd*2 < n {
		left -= n - atEnd - 1
		right = len(fis)
	} else if atBegin*2 < n {
		left = 0
		right += n - atBegin
	} else {
		left -= n - n/2 - 1
		right += n / 2
	}

	return fis[left:right], nil
}
