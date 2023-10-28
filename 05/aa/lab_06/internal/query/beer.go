package query

import (
	"errors"
	"lab_06/internal/beer"
	"lab_06/internal/query/config"
	"lab_06/internal/text"
	"strings"
)

type Manager struct {
	cfg  config.Config
	dict *beer.Dictionary
	txt  *text.Manager
}

func New(cfg config.Config, dict *beer.Dictionary, txt *text.Manager) *Manager {
	return &Manager{
		cfg:  cfg,
		dict: dict,
		txt:  txt,
	}
}

func (m *Manager) parse(s string) (float64, float64, error) {
	terms, err := m.txt.ToTerms(s)
	if err != nil {
		return 0, 0, err
	}

	for i, term := range terms {
		flag := false

		for _, v := range m.cfg.Trash {
			if term == v {
				terms[i] = " "
				flag = true
				break
			}
		}

		if flag {
			break
		}
	}

	flag := false
	for i, term := range terms {
		for _, v := range m.cfg.Objects {
			if term == v {
				terms[i] = " "
				flag = true
				break
			}
		}

		if flag {
			break
		}
	}

	if !flag {
		return 0, 0, errors.New("не понятно, о чем запрос")
	}

	joined := strings.Join(terms, " ")
	joined = strings.TrimSpace(joined)

	start := -1.0
	end := -1.0
	for _, g := range m.cfg.Groups {
		for _, v := range g.Names {
			if joined == v {
				if start == -1.0 {
					start = g.Start
				}
				if g.End > end {
					end = g.End
				}
			}
		}
	}

	if start == -1.0 || end == -1.0 {
		return 0, 0, errors.New("некорректный формат запроса")
	}

	return start, end, nil
}

func (m *Manager) Answer(s string) ([]beer.Beer, error) {
	from, to, err := m.parse(s)
	if err != nil {
		return nil, err
	}

	return m.dict.Search(from, to), nil
}
