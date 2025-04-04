package ast

import (
	"fmt"
	"os"
	"slices"
	"strings"

	"lab_04/internal/token"
)

type Node interface {
	render(file *os.File) error
	makePostfix() []string
}

func New(tokens []token.Token) (Node, error) {
	p := newParser(tokens)
	program, err := p.parseExpression()
	if err != nil {
		return nil, fmt.Errorf("parse program: %w", err)
	}

	return program, nil
}

func Postfix(root Node) string {
	return strings.Join(root.makePostfix(), " ")
}

type Expression struct {
	Left      *ArithmeticalExpression
	Operation *RelationOperation
	Right     *ArithmeticalExpression
	Postfix   []string
}

func (n *Expression) makePostfix() []string {
	if n == nil {
		return nil
	}

	n.Postfix = slices.Clone(n.Left.makePostfix())
	n.Postfix = append(n.Postfix, n.Right.makePostfix()...)
	n.Postfix = append(n.Postfix, n.Operation.makePostfix()...)

	return n.Postfix
}

type ArithmeticalExpression struct {
	SumOperation           *SumOperation
	Term                   *Term
	ArithmeticalExpression *ArithmeticalExpression1
	Postfix                []string
}

func (n *ArithmeticalExpression) makePostfix() []string {
	if n == nil {
		return nil
	}

	n.Postfix = slices.Clone(n.Term.makePostfix())
	n.Postfix = append(n.Postfix, n.ArithmeticalExpression.makePostfix()...)
	n.Postfix = append(n.Postfix, n.SumOperation.makePostfix()...)

	return n.Postfix
}

type Term struct {
	Multiplier *Multiplier
	Term       *Term1
	Postfix    []string
}

func (n *Term) makePostfix() []string {
	if n == nil {
		return nil
	}

	n.Postfix = slices.Clone(n.Multiplier.makePostfix())
	n.Postfix = append(n.Postfix, n.Term.makePostfix()...)

	return n.Postfix
}

type Multiplier struct {
	PrimaryExpression *PrimaryExpression
	Multiplier        *Multiplier1
	Postfix           []string
}

func (n *Multiplier) makePostfix() []string {
	if n == nil {
		return nil
	}

	n.Postfix = slices.Clone(n.PrimaryExpression.makePostfix())
	n.Postfix = append(n.Postfix, n.Multiplier.makePostfix()...)

	return n.Postfix
}

type PrimaryExpression struct {
	Number                 *Number
	Identifier             *Identifier
	ArithmeticalExpression *ArithmeticalExpression
	Postfix                []string
}

func (n *PrimaryExpression) makePostfix() []string {
	if n == nil {
		return nil
	}

	n.Postfix = slices.Clone(n.Number.makePostfix())
	n.Postfix = append(n.Postfix, n.Identifier.makePostfix()...)
	n.Postfix = append(n.Postfix, n.ArithmeticalExpression.makePostfix()...)

	return n.Postfix
}

type RelationOperation struct {
	Value   string
	Postfix []string
}

func (n *RelationOperation) makePostfix() []string {
	if n == nil {
		return nil
	}

	n.Postfix = []string{n.Value}

	return n.Postfix
}

type SumOperation struct {
	Value   string
	Unary   bool
	Postfix []string
}

func (n *SumOperation) makePostfix() []string {
	if n == nil {
		return nil
	}

	if n.Unary {
		n.Postfix = []string{"unary" + n.Value}
	} else {
		n.Postfix = []string{n.Value}
	}

	return n.Postfix
}

type MulOperation struct {
	Value   string
	Postfix []string
}

func (n *MulOperation) makePostfix() []string {
	if n == nil {
		return nil
	}

	n.Postfix = []string{n.Value}

	return n.Postfix
}

type ArithmeticalExpression1 struct {
	SumOperation           *SumOperation
	Term                   *Term
	ArithmeticalExpression *ArithmeticalExpression1
	Postfix                []string
}

func (n *ArithmeticalExpression1) makePostfix() []string {
	if n == nil {
		return nil
	}

	n.Postfix = slices.Clone(n.Term.makePostfix())
	n.Postfix = append(n.Postfix, n.ArithmeticalExpression.makePostfix()...)
	n.Postfix = append(n.Postfix, n.SumOperation.makePostfix()...)

	return n.Postfix
}

type Term1 struct {
	MulOperation *MulOperation
	Multiplier   *Multiplier
	Term         *Term1
	Postfix      []string
}

func (n *Term1) makePostfix() []string {
	if n == nil {
		return nil
	}

	n.Postfix = slices.Clone(n.Term.makePostfix())
	n.Postfix = append(n.Postfix, n.Multiplier.makePostfix()...)
	n.Postfix = append(n.Postfix, n.MulOperation.makePostfix()...)

	return n.Postfix
}

type PowerOperation struct {
	Postfix []string
}

func (n *PowerOperation) makePostfix() []string {
	if n == nil {
		return nil
	}

	n.Postfix = []string{"^"}

	return n.Postfix
}

type Identifier struct {
	Value   string
	Postfix []string
}

func (n *Identifier) makePostfix() []string {
	if n == nil {
		return nil
	}

	n.Postfix = []string{n.Value}

	return n.Postfix
}

type Number struct {
	Value   string
	Postfix []string
}

func (n *Number) makePostfix() []string {
	if n == nil {
		return nil
	}

	n.Postfix = []string{n.Value}

	return n.Postfix
}

type Multiplier1 struct {
	Pow               *PowerOperation
	PrimaryExpression *PrimaryExpression
	Multiplier        *Multiplier1
	Postfix           []string
}

func (n *Multiplier1) makePostfix() []string {
	if n == nil {
		return nil
	}

	n.Postfix = slices.Clone(n.PrimaryExpression.makePostfix())
	n.Postfix = append(n.Postfix, n.Multiplier.makePostfix()...)
	n.Postfix = append(n.Postfix, n.Pow.makePostfix()...)

	return n.Postfix
}
