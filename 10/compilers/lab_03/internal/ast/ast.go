package ast

import (
	"fmt"
	"os"

	"lab_03/internal/token"
)

type Node interface {
	render(file *os.File) error
}

func New(tokens []token.Token) (Node, error) {
	p := newParser(tokens)
	program, err := p.parseProgram()
	if err != nil {
		return nil, fmt.Errorf("parse program: %w", err)
	}

	return program, nil
}

type Program struct {
	Block *Block
}

type Block struct {
	OperatorList *OperatorList
}

type OperatorList struct {
	Operator     *Operator
	OperatorList *OperatorList1
}

type Operator struct {
	Identifier *Identifier
	Expression *Expression
}

type Expression struct {
	Left      *ArithmeticalExpression
	Operation *RelationOperation
	Right     *ArithmeticalExpression
}

type ArithmeticalExpression struct {
	SumOperation           *SumOperation
	Term                   *Term
	ArithmeticalExpression *ArithmeticalExpression1
}

type Term struct {
	Multiplier *Multiplier
	Term       *Term1
}

type Multiplier struct {
	PrimaryExpression *PrimaryExpression
	Multiplier        *Multiplier1
}

type PrimaryExpression struct {
	Number                 *Number
	Identifier             *Identifier
	ArithmeticalExpression *ArithmeticalExpression
}

type RelationOperation struct {
	Value string
}

type SumOperation struct {
	Value string
}

type MulOperation struct {
	Value string
}

type OperatorList1 struct {
	Operator     *Operator
	OperatorList *OperatorList1
}

type ArithmeticalExpression1 struct {
	SumOperation           *SumOperation
	Term                   *Term
	ArithmeticalExpression *ArithmeticalExpression1
}

type Term1 struct {
	MulOperation *MulOperation
	Multiplier   *Multiplier
	Term         *Term1
}

type PowerOperation struct {
}

type Identifier struct {
	Value string
}

type Number struct {
	Value string
}

type Multiplier1 struct {
	Pow               *PowerOperation
	PrimaryExpression *PrimaryExpression
	Multiplier        *Multiplier1
}
