package coder

import "encoding/json"

var (
	JSON jsonCoder
)

type jsonCoder struct{}

func (j jsonCoder) Marshal(v any) (string, error) {
	data, err := json.Marshal(v)
	if err != nil {
		return "", err
	}

	return string(data), err
}

func (j jsonCoder) Unmarshal(data string, v any) error {
	return json.Unmarshal([]byte(data), v)
}
