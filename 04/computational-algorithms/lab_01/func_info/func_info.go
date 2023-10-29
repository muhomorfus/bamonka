package func_info

import (
	"fmt"
	"io"
	"sort"
)

type FuncInfo struct {
	X          float64
	Y          float64
	Derivative float64
}

func (fi *FuncInfo) FromReaderOne(reader io.Reader) (bool, error) {
	n, err := fmt.Fscanf(reader, "%f%f%f", &fi.X, &fi.Y, &fi.Derivative)
	if err == nil {
		return true, nil
	}

	if err == io.EOF && n == 0 {
		return false, nil
	}

	return false, err
}

func FromReaderMany(reader io.Reader) ([]FuncInfo, error) {
	result := make([]FuncInfo, 0)
	var fi FuncInfo

	ok, err := fi.FromReaderOne(reader)
	if !ok || err != nil {
		return nil, err
	}
	for err == nil && ok {
		result = append(result, fi)
		ok, err = fi.FromReaderOne(reader)
	}

	if err != nil {
		return nil, err
	}

	sort.Slice(result, func(i, j int) bool {
		return result[i].X < result[j].X
	})

	return result, nil
}
