package polynomial

import (
	"errors"
	"fmt"
	"github.com/steelseries/golisp"
	"unsafe"
)

var (
	ErrInvalidType = errors.New("invalid type")
)

func init() {
	golisp.MakePrimitiveFunction("from_list", "1", FromListLisp)
	golisp.MakePrimitiveFunction("to_string", "1", ToStringLisp)
	golisp.MakePrimitiveFunction("p_sum", "2", PSumLisp)
	golisp.MakePrimitiveFunction("p_sub", "2", PSubLisp)
	golisp.MakePrimitiveFunction("p_mul", "2", PMulLisp)
	golisp.MakePrimitiveFunction("p_div", "2", PDivLisp)
}

func FromListLisp(args *golisp.Data, env *golisp.SymbolTableFrame) (*golisp.Data, error) {
	val := golisp.Car(args)

	if !golisp.ListP(val) {
		return nil, fmt.Errorf("check list: %w", ErrInvalidType)
	}

	p := new(Polynomial)

	for val != nil {
		fmt.Println(golisp.PrintString(golisp.Car(val)))
		if !golisp.PairP(golisp.Car(val)) {
			return nil, fmt.Errorf("check pair: %w", ErrInvalidType)
		}

		if !golisp.FloatP(golisp.Caar(val)) || !golisp.IntegerP(golisp.Cdar(val)) {
			return nil, fmt.Errorf("check numeric types: %w", ErrInvalidType)
		}

		p.Monomials = append(p.Monomials, Monomial{
			Coefficient: float64(golisp.FloatValue(golisp.Caar(val))),
			Power:       uint(golisp.IntegerValue(golisp.Cdar(val))),
		})

		val = golisp.Cdr(val)
	}

	return golisp.ObjectWithTypeAndValue("polynomial", unsafe.Pointer(p)), nil
}

func ToStringLisp(args *golisp.Data, env *golisp.SymbolTableFrame) (*golisp.Data, error) {
	val, err := golisp.Eval(golisp.Car(args), env)
	if err != nil {
		return nil, fmt.Errorf("eval: %w", ErrInvalidType)
	}

	if !golisp.ObjectP(val) {
		return nil, fmt.Errorf("check object: %w", err)
	}

	p := (*Polynomial)(golisp.ObjectValue(val))
	fmt.Println(p.String())

	return nil, nil
}

func PSumLisp(args *golisp.Data, env *golisp.SymbolTableFrame) (*golisp.Data, error) {
	val1, err := golisp.Eval(golisp.Car(args), env)
	if err != nil {
		return nil, fmt.Errorf("eval val1: %w", ErrInvalidType)
	}

	val2, err := golisp.Eval(golisp.Cadr(args), env)
	if err != nil {
		return nil, fmt.Errorf("eval val2: %w", ErrInvalidType)
	}

	if !golisp.ObjectP(val1) || !golisp.ObjectP(val2) {
		return nil, fmt.Errorf("check object: %w", ErrInvalidType)
	}

	p1 := (*Polynomial)(golisp.ObjectValue(val1))
	p2 := (*Polynomial)(golisp.ObjectValue(val2))

	res := Sum(p1, p2)

	return golisp.ObjectWithTypeAndValue("polynomial", unsafe.Pointer(res)), nil
}

func PSubLisp(args *golisp.Data, env *golisp.SymbolTableFrame) (*golisp.Data, error) {
	val1, err := golisp.Eval(golisp.Car(args), env)
	if err != nil {
		return nil, fmt.Errorf("eval val1: %w", ErrInvalidType)
	}

	val2, err := golisp.Eval(golisp.Cadr(args), env)
	if err != nil {
		return nil, fmt.Errorf("eval val2: %w", ErrInvalidType)
	}

	if !golisp.ObjectP(val1) || !golisp.ObjectP(val2) {
		return nil, fmt.Errorf("check object: %w", ErrInvalidType)
	}

	p1 := (*Polynomial)(golisp.ObjectValue(val1))
	p2 := (*Polynomial)(golisp.ObjectValue(val2))

	res := Sub(p1, p2)

	return golisp.ObjectWithTypeAndValue("polynomial", unsafe.Pointer(res)), nil
}

func PMulLisp(args *golisp.Data, env *golisp.SymbolTableFrame) (*golisp.Data, error) {
	val1, err := golisp.Eval(golisp.Car(args), env)
	if err != nil {
		return nil, fmt.Errorf("eval val1: %w", ErrInvalidType)
	}

	val2, err := golisp.Eval(golisp.Cadr(args), env)
	if err != nil {
		return nil, fmt.Errorf("eval val2: %w", ErrInvalidType)
	}

	if !golisp.ObjectP(val1) || !golisp.ObjectP(val2) {
		return nil, fmt.Errorf("check object: %w", ErrInvalidType)
	}

	p1 := (*Polynomial)(golisp.ObjectValue(val1))
	p2 := (*Polynomial)(golisp.ObjectValue(val2))

	res := Mul(p1, p2)

	return golisp.ObjectWithTypeAndValue("polynomial", unsafe.Pointer(res)), nil
}

func PDivLisp(args *golisp.Data, env *golisp.SymbolTableFrame) (*golisp.Data, error) {
	val1, err := golisp.Eval(golisp.Car(args), env)
	if err != nil {
		return nil, fmt.Errorf("eval val1: %w", ErrInvalidType)
	}

	val2, err := golisp.Eval(golisp.Cadr(args), env)
	if err != nil {
		return nil, fmt.Errorf("eval val2: %w", ErrInvalidType)
	}

	if !golisp.ObjectP(val1) || !golisp.ObjectP(val2) {
		return nil, fmt.Errorf("check object: %w", ErrInvalidType)
	}

	p1 := (*Polynomial)(golisp.ObjectValue(val1))
	p2 := (*Polynomial)(golisp.ObjectValue(val2))

	res, _, err := Div(p1, p2)
	if err != nil {
		return nil, fmt.Errorf("div: %w", ErrInvalidType)
	}

	return golisp.ObjectWithTypeAndValue("polynomial", unsafe.Pointer(res)), nil
}
