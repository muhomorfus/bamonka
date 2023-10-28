package model

import (
	"encoding/json"
	"fmt"
	"os"
)

type Model struct {
	F0    float64
	Betta float64
	X0    float64
	Z0    float64
	T0    float64
	Dx    float64
	Dz    float64

	X1 float64
	Z1 float64
	X2 float64
	Z2 float64

	A1  float64
	B1  float64
	C1  float64
	M1  float64
	Tau float64
}

func FromFile(name string) (*Model, error) {
	file, err := os.Open(name)
	if err != nil {
		return nil, fmt.Errorf("open file: %w", err)
	}

	var c config
	if err := json.NewDecoder(file).Decode(&c); err != nil {
		return nil, fmt.Errorf("decode config: %w", err)
	}

	m := &Model{
		F0:    c.F0,
		Betta: c.Betta,
		X0:    c.X0,
		Z0:    c.Z0,
		T0:    c.T0,
		Dx:    c.Dx,
		Dz:    c.Dz,

		X1: c.X1,
		Z1: c.Z1,
		X2: c.X2,
		Z2: c.Z2,

		A1:  c.A1,
		B1:  c.B1,
		C1:  c.C1,
		M1:  c.M1,
		Tau: c.Tau,
	}

	return m, nil
}

type config struct {
	F0    float64 `json:"F0"`
	Betta float64 `json:"Betta"`
	X0    float64 `json:"X0"`
	Z0    float64 `json:"Z0"`
	T0    float64 `json:"T0"`
	Dx    float64 `json:"Dx"`
	Dz    float64 `json:"Dz"`

	X1 float64 `json:"X1"`
	Z1 float64 `json:"Z1"`
	X2 float64 `json:"X2"`
	Z2 float64 `json:"Z2"`

	A1  float64 `json:"A1"`
	B1  float64 `json:"B1"`
	C1  float64 `json:"C1"`
	M1  float64 `json:"M1"`
	Tau float64 `json:"Tau"`
}
