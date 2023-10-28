package polynomial

import (
	"errors"
	"fmt"
	"math"
)

var (
	ErrDivisionByZero = errors.New("dividion by zero")
)

type Monomial struct {
	Coefficient float64
	Power       uint
}

type Polynomial struct {
	Monomials []Monomial
}

func New(ms []Monomial) *Polynomial {
	return &Polynomial{
		Monomials: ms,
	}
}

func (p *Polynomial) Empty() bool {
	return p == nil || len(p.Monomials) <= 0
}

func (p *Polynomial) String() string {
	if len(p.Monomials) == 0 {
		return "0"
	}

	result := fmt.Sprintf("%fx^%d", p.Monomials[0].Coefficient, p.Monomials[0].Power)
	for i := 1; i < len(p.Monomials); i++ {
		sign := "+"
		if p.Monomials[i].Coefficient < 0 {
			sign = "-"
		}

		result += fmt.Sprintf(" %s %fx^%d", sign, math.Abs(p.Monomials[i].Coefficient), p.Monomials[i].Power)
	}

	return result
}

func (p *Polynomial) Clone() *Polynomial {
	if p.Empty() {
		return new(Polynomial)
	}

	result := new(Polynomial)
	result.Monomials = make([]Monomial, len(p.Monomials))

	for i := range result.Monomials {
		result.Monomials[i] = p.Monomials[i]
	}

	return result
}

func Sum(p1, p2 *Polynomial) *Polynomial {
	result := new(Polynomial)

	i1, i2 := 0, 0
	for i1 != len(p1.Monomials) || i2 != len(p2.Monomials) {
		switch {
		case i2 == len(p2.Monomials):
			result.Monomials = append(result.Monomials, p1.Monomials[i1])
			i1++
		case i1 == len(p1.Monomials):
			result.Monomials = append(result.Monomials, p2.Monomials[i2])
			i2++
		case p1.Monomials[i1].Power > p2.Monomials[i2].Power:
			result.Monomials = append(result.Monomials, p1.Monomials[i1])
			i1++
		case p1.Monomials[i1].Power < p2.Monomials[i2].Power:
			result.Monomials = append(result.Monomials, p2.Monomials[i2])
			i2++
		default:
			m := Monomial{
				Coefficient: p1.Monomials[i1].Coefficient + p2.Monomials[i2].Coefficient,
				Power:       p1.Monomials[i1].Power,
			}
			if m.Coefficient != 0 {
				result.Monomials = append(result.Monomials, m)
			}
			i1++
			i2++
		}
	}

	return result
}

func (p *Polynomial) Equal(p1 *Polynomial) bool {
	if len(p.Monomials) != len(p1.Monomials) {
		return false
	}

	for i := range p.Monomials {
		if p.Monomials[i] != p1.Monomials[i] {
			return false
		}
	}

	return true
}

func (p *Polynomial) Negative() *Polynomial {
	result := new(Polynomial)
	result.Monomials = make([]Monomial, len(p.Monomials))

	for i := range result.Monomials {
		result.Monomials[i] = p.Monomials[i]
		result.Monomials[i].Coefficient = -p.Monomials[i].Coefficient
	}

	return result
}

func Sub(p1, p2 *Polynomial) *Polynomial {
	return Sum(p1, p2.Negative())
}

func (p *Polynomial) MulMonomial(m Monomial) *Polynomial {
	result := new(Polynomial)
	result.Monomials = make([]Monomial, len(p.Monomials))

	for i := range result.Monomials {
		result.Monomials[i].Coefficient = p.Monomials[i].Coefficient * m.Coefficient
		result.Monomials[i].Power = p.Monomials[i].Power + m.Power
	}

	return result
}

func Mul(p1, p2 *Polynomial) *Polynomial {
	result := new(Polynomial)

	for i := range p2.Monomials {
		result = Sum(result, p1.MulMonomial(p2.Monomials[i]))
	}

	return result
}

func (p *Polynomial) DivMonomial(d *Polynomial) Monomial {
	return Monomial{
		Coefficient: p.Monomials[0].Coefficient / d.Monomials[0].Coefficient,
		Power:       p.Monomials[0].Power - d.Monomials[0].Power,
	}
}

func Div(p1, p2 *Polynomial) (*Polynomial, *Polynomial, error) {
	result := new(Polynomial)

	p1c := p1.Clone()
	p2c := p2.Clone()

	for {
		switch {
		case p2c.Empty():
			return nil, nil, fmt.Errorf("zero value: %w", ErrDivisionByZero)
		case p1c.Empty():
			return result, p1c, nil
		case p1c.Monomials[0].Power < p2c.Monomials[0].Power:
			return result, p1c, nil
		default:
			m := p1c.DivMonomial(p2c)
			result.Monomials = append(result.Monomials, m)
			p1c = Sub(p1c, p2c.MulMonomial(m))
		}
	}
}
