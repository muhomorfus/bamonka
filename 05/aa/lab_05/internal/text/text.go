package text

import (
	"gitlab.com/opennota/morph"
	"os"
	"strings"
)

const (
	textsSeparator = "***"
	wordsSeparator = "\n"
)

func init() {
	if err := morph.Init(); err != nil {
		panic(err)
	}
}

func ReadTexts(filename string) ([][]string, error) {
	bytes, err := os.ReadFile(filename)
	if err != nil {
		return nil, err
	}

	textsStr := strings.Split(string(bytes), textsSeparator)
	texts := make([][]string, len(textsStr))

	for i, v := range textsStr {
		splitted := strings.Split(v, wordsSeparator)

		for j := range splitted {
			term := strings.TrimSpace(splitted[j])

			if term != "" {
				texts[i] = append(texts[i], term)
			}
		}
	}

	return texts, nil
}

func Normalize(words []string) []string {
	terms := make([]string, 0)

	for _, v := range words {
		_, normals, _ := morph.Parse(strings.ToLower(v))
		if len(normals) == 0 {
			panic("no normals")
		}

		terms = append(terms, normals[0])
	}

	return terms
}

func Deduplicate(terms []string) map[string]int {
	termsMap := make(map[string]int)

	for _, v := range terms {
		termsMap[v] = termsMap[v] + 1
	}

	return termsMap
}

func CountTF(termsMap map[string]int) map[string]float64 {
	n := 0

	for _, v := range termsMap {
		n += v
	}

	tfs := make(map[string]float64)
	for k, v := range termsMap {
		tfs[k] = float64(v) / float64(n)
	}

	return tfs
}
