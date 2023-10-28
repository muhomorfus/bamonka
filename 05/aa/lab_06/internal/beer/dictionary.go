package beer

import (
	"encoding/json"
	"os"
	"sort"
)

type Dictionary struct {
	Elements []Beer
}

func FromJson(fileName string) (*Dictionary, error) {
	file, err := os.ReadFile(fileName)
	if err != nil {
		return nil, err
	}

	var d Dictionary
	if err := json.Unmarshal(file, &d); err != nil {
		return nil, err
	}

	sort.SliceStable(d.Elements, func(i, j int) bool {
		return d.Elements[i].Strength < d.Elements[j].Strength
	})

	return &d, nil
}

func (d *Dictionary) search(v float64, flag bool) int {
	last := len(d.Elements) - 1
	if v > d.Elements[last].Strength {
		return last
	}
	if v < d.Elements[0].Strength {
		return 0
	}

	l := 0
	r := last
	for l <= r {
		m := (l + r) / 2

		if v > d.Elements[m].Strength {
			l = m + 1
		} else if v < d.Elements[m].Strength {
			r = m - 1
		} else {
			return m
		}
	}

	if d.Elements[l].Strength > d.Elements[r].Strength == flag {
		return l
	}

	return r
}

func (d *Dictionary) Search(from, to float64) []Beer {
	begin := d.search(from, true)
	end := d.search(to, false)

	return d.Elements[begin : end+1]
}
