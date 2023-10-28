package text

import (
	"errors"
	"fmt"
	"gitlab.com/opennota/morph"
	"math"
	"os"
	"regexp"
	"strings"
)

const maxDiff = 2

func getMaxDiff(s string) int {
	half := len(s) / 2
	if half < maxDiff {
		return half
	}

	return maxDiff
}

func init() {
	if err := morph.Init(); err != nil {
		panic(err)
	}
}

type Manager struct {
	words []string
}

func New(fileName string) (*Manager, error) {
	file, err := os.ReadFile(fileName)
	if err != nil {
		return nil, err
	}

	words := strings.Split(string(file), "\n")

	return &Manager{
		words: words,
	}, nil
}

func (m *Manager) normalize(words []string) ([]string, string, error) {
	terms := make([]string, 0)
	maybe := make([]string, 0, len(words))
	flag := false

	for _, v := range words {
		var term string
		source := v

		_, normals, _ := morph.Parse(strings.ToLower(v))
		if len(normals) == 0 {
			minDiff := math.MaxInt
			candidate := ""

			for _, e := range m.words {
				d := diff(v, e)
				if d < minDiff {
					minDiff = d
					candidate = e
				}
			}

			_, normals, _ := morph.Parse(strings.ToLower(candidate))
			if len(normals) == 0 || getMaxDiff(v) <= minDiff {
				return nil, "", errors.New("не найдено слово")
			}

			flag = true
			source = candidate
			term = normals[0]
		} else {
			term = normals[0]
		}

		terms = append(terms, term)
		maybe = append(maybe, source)
	}

	maybeStr := ""
	if flag {
		maybeStr = strings.Join(maybe, " ")
	}

	return terms, maybeStr, nil
}

func (m *Manager) tokenize(s string) ([]string, error) {
	r := regexp.MustCompile(`([^0-9а-яА-ЯЁё-])`).Split(s, -1)
	tokens := make([]string, 0, len(r))

	for i := range r {
		if r[i] != "" && r[i] != "-" {
			tokens = append(tokens, strings.ToLower(r[i]))
		}
	}

	if len(tokens) == 0 {
		return nil, errors.New("неправильная последовательность символов")
	}

	return tokens, nil
}

func (m *Manager) ToTerms(s string) ([]string, error) {
	tokens, err := m.tokenize(s)
	if err != nil {
		return nil, err
	}

	result, maybe, err := m.normalize(tokens)
	if err != nil {
		return nil, err
	}

	if maybe != "" {
		fmt.Printf("Возможно, вы имели в виду «%s».\n", maybe)
	}

	return result, nil
}
