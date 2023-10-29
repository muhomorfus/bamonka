package compute

import (
	"errors"
	"math"
	"sort"

	"lab_01/func_info"
)

const Eps = 1e-7

func isMonotonous(fis []func_info.FuncInfo) bool {
	flag := fis[0].Y < fis[1].Y
	for i := 0; i < len(fis)-1; i++ {
		if fis[i].Y < fis[i+1].Y != flag || fis[i].Derivative == 0 || fis[i+1].Derivative == 0 {
			return false
		}
	}

	return true
}

func getReversedFunction(fis []func_info.FuncInfo) []func_info.FuncInfo {
	reversed := make([]func_info.FuncInfo, len(fis))
	for i, fi := range fis {
		reversed[i] = func_info.FuncInfo{
			X:          fi.Y,
			Y:          fi.X,
			Derivative: 1 / fi.Derivative,
		}
	}

	sort.Slice(reversed, func(i, j int) bool {
		return reversed[i].X < reversed[j].X
	})

	return reversed
}

func getMonotonousIntervals(fis []func_info.FuncInfo) [][]func_info.FuncInfo {
	intervals := make([][]func_info.FuncInfo, 1)

	start := 0
	for fis[start].Derivative == 0 && start < len(fis)-1 {
		start++
	}

	if start >= len(fis)-1 {
		return intervals
	}

	intervals[0] = make([]func_info.FuncInfo, 0)
	intervals[0] = append(intervals[0], fis[start])

	biggerPrevious := fis[start+1].Y > fis[start].Y
	for i := start + 1; i < len(fis); i++ {
		bigger := fis[i].Y > fis[i-1].Y
		if fis[i].Derivative == 0 {
			intervals = append(intervals, make([]func_info.FuncInfo, 0))
			continue
		}

		if bigger != biggerPrevious {
			intervals = append(intervals, make([]func_info.FuncInfo, 0))

			if fis[i-1].Derivative != 0 {
				last := len(intervals) - 1
				intervals[last] = append(intervals[last], fis[i-1])
			}
		}

		last := len(intervals) - 1
		intervals[last] = append(intervals[last], fis[i])
		biggerPrevious = bigger
	}

	//fmt.Println(intervals)
	return intervals
}

func rootByReverseInterpolation(fis []func_info.FuncInfo, n int, diffsFunc DiffFunc) (float64, error) {
	if len(fis) < 2 {
		return 0, errors.New("empty interval")
	}

	if fis[0].Y*fis[len(fis)-1].Y > 0 {
		return 0, errors.New("there are no root in this interval")
	}
	reversed := getReversedFunction(fis)
	return Polynomial(reversed, 0, n, diffsFunc)
}

func rootByBisection(fis []func_info.FuncInfo, n int, diffsFunc DiffFunc) (float64, error) {
	left := fis[0].X
	right := fis[len(fis)-1].X
	middle := (left + right) / 2

	y, err := Polynomial(fis, middle, n, diffsFunc)
	for math.Abs(y) > Eps && err == nil {
		middle = (left + right) / 2
		y, err = Polynomial(fis, middle, n, diffsFunc)

		if y < 0 {
			left = middle
		} else {
			right = middle
		}

		y, err = Polynomial(fis, middle, n, diffsFunc)
	}

	return (left + right) / 2, err
}

func FindRoot(fis []func_info.FuncInfo, x float64, n int, diffsFunc DiffFunc) (float64, error) {
	configuration, err := GetConfiguration(fis, x, n)
	if err != nil {
		return 0, err
	}

	//intervals := getMonotonousIntervals(configuration)
	//for _, interval := range intervals {
	//	root, err := rootByReverseInterpolation(interval, len(interval)+1, diffsFunc)
	//
	//	if math.IsNaN(root) || math.IsInf(root, 0) {
	//		continue
	//	}
	//
	//	if err == nil {
	//		return root, nil
	//	}
	//}

	return rootByBisection(configuration, n, diffsFunc)
}
