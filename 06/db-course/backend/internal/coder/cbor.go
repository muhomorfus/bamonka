package coder

import (
	"github.com/fxamacker/cbor/v2"
)

var (
	CBOR cborCoder
)

type cborCoder struct{}

func (cborCoder) Marshal(v any) (string, error) {
	data, err := cbor.Marshal(v)
	if err != nil {
		return "", err
	}

	return string(data), err
}

func (cborCoder) Unmarshal(data string, v any) error {
	return cbor.Unmarshal([]byte(data), v)
}
