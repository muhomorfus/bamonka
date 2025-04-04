package ast

import (
	"fmt"
	"os"
	"os/exec"
)

func Show(root Node) error {
	file, err := os.Create("data/ast.dot")
	if err != nil {
		return err
	}
	defer file.Close()

	_, err = file.WriteString("digraph AST {\n")
	if err != nil {
		return err
	}

	err = root.render(file)
	if err != nil {
		return err
	}

	_, err = file.WriteString("}\n")
	if err != nil {
		return err
	}

	cmd := exec.Command("dot", "-Tpng", "data/ast.dot", "-o", "data/ast.png")
	err = cmd.Run()
	if err != nil {
		return err
	}

	// cmd = exec.Command("open", "data/ast.png")
	// err = cmd.Run()
	// if err != nil {
	// 	return err
	// }

	return nil
}

func (n *Program) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\"];\n", n, "<program>")
	if err != nil {
		return err
	}

	_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Block)
	if err != nil {
		return err
	}

	return n.Block.render(file)
}

func (n *Block) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\"];\n", n, "<block>")
	if err != nil {
		return err
	}

	_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.OperatorList)
	if err != nil {
		return err
	}

	return n.OperatorList.render(file)
}

func (n *OperatorList) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\"];\n", n, "<operator list>")
	if err != nil {
		return err
	}

	if n.Operator != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Operator)
		if err != nil {
			return err
		}

		err = n.Operator.render(file)
		if err != nil {
			return err
		}
	}

	if n.OperatorList != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.OperatorList)
		if err != nil {
			return err
		}

		err = n.OperatorList.render(file)
		if err != nil {
			return err
		}
	}

	return nil
}

func (n *Operator) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\"];\n", n, "<operator>")
	if err != nil {
		return err
	}

	if n.Identifier != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Identifier)
		if err != nil {
			return err
		}

		err = n.Identifier.render(file)
		if err != nil {
			return err
		}
	}

	if n.Expression != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Expression)
		if err != nil {
			return err
		}

		err = n.Expression.render(file)
		if err != nil {
			return err
		}
	}

	return nil
}

func (n *Identifier) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s (%s)\"];\n", n, "<identifier>", n.Value)
	if err != nil {
		return err
	}

	return nil
}

func (n *Expression) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\"];\n", n, "<expression>")
	if err != nil {
		return err
	}

	if n.Left != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Left)
		if err != nil {
			return err
		}

		err = n.Left.render(file)
		if err != nil {
			return err
		}
	}

	if n.Operation != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Operation)
		if err != nil {
			return err
		}

		err = n.Operation.render(file)
		if err != nil {
			return err
		}
	}

	if n.Right != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Right)
		if err != nil {
			return err
		}

		err = n.Right.render(file)
		if err != nil {
			return err
		}
	}

	return nil
}

func (n *ArithmeticalExpression) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\"];\n", n, "<arithmetical expression>")
	if err != nil {
		return err
	}

	if n.SumOperation != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.SumOperation)
		if err != nil {
			return err
		}

		err = n.SumOperation.render(file)
		if err != nil {
			return err
		}
	}

	if n.Term != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Term)
		if err != nil {
			return err
		}

		err = n.Term.render(file)
		if err != nil {
			return err
		}
	}

	if n.ArithmeticalExpression != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.ArithmeticalExpression)
		if err != nil {
			return err
		}

		err = n.ArithmeticalExpression.render(file)
		if err != nil {
			return err
		}
	}

	return nil
}

func (n *Term) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\"];\n", n, "<term>")
	if err != nil {
		return err
	}

	if n.Multiplier != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Multiplier)
		if err != nil {
			return err
		}

		err = n.Multiplier.render(file)
		if err != nil {
			return err
		}
	}

	if n.Term != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Term)
		if err != nil {
			return err
		}

		err = n.Term.render(file)
		if err != nil {
			return err
		}
	}

	return nil
}

func (n *Multiplier) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\"];\n", n, "<multiplier>")
	if err != nil {
		return err
	}

	if n.PrimaryExpression != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.PrimaryExpression)
		if err != nil {
			return err
		}

		err = n.PrimaryExpression.render(file)
		if err != nil {
			return err
		}
	}

	if n.Multiplier != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Multiplier)
		if err != nil {
			return err
		}

		err = n.Multiplier.render(file)
		if err != nil {
			return err
		}
	}

	return nil
}

func (n *Number) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s (%s)\"];\n", n, "<number>", n.Value)
	if err != nil {
		return err
	}

	return nil
}

func (n *OperatorList1) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\"];\n", n, "<operator list>'")
	if err != nil {
		return err
	}

	if n.Operator != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Operator)
		if err != nil {
			return err
		}

		err = n.Operator.render(file)
		if err != nil {
			return err
		}
	}

	if n.OperatorList != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.OperatorList)
		if err != nil {
			return err
		}

		err = n.OperatorList.render(file)
		if err != nil {
			return err
		}
	}

	return nil
}

func (n *RelationOperation) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s (%s)\"];\n", n, "<relation operation>", n.Value)
	if err != nil {
		return err
	}

	return nil
}

func (n *ArithmeticalExpression1) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\"];\n", n, "<arithmetical expression>'")
	if err != nil {
		return err
	}

	if n.SumOperation != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.SumOperation)
		if err != nil {
			return err
		}

		err = n.SumOperation.render(file)
		if err != nil {
			return err
		}
	}

	if n.Term != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Term)
		if err != nil {
			return err
		}

		err = n.Term.render(file)
		if err != nil {
			return err
		}
	}

	if n.ArithmeticalExpression != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.ArithmeticalExpression)
		if err != nil {
			return err
		}

		err = n.ArithmeticalExpression.render(file)
		if err != nil {
			return err
		}
	}

	return nil
}

func (n *Term1) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\"];\n", n, "<term>'")
	if err != nil {
		return err
	}

	if n.MulOperation != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.MulOperation)
		if err != nil {
			return err
		}

		err = n.MulOperation.render(file)
		if err != nil {
			return err
		}
	}

	if n.Multiplier != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Multiplier)
		if err != nil {
			return err
		}

		err = n.Multiplier.render(file)
		if err != nil {
			return err
		}
	}

	if n.Term != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Term)
		if err != nil {
			return err
		}

		err = n.Term.render(file)
		if err != nil {
			return err
		}
	}

	return nil
}

func (n *SumOperation) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s (%s)\"];\n", n, "<sum operation>", n.Value)
	if err != nil {
		return err
	}

	return nil
}

func (n *MulOperation) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s (%s)\"];\n", n, "<mul operation>", n.Value)
	if err != nil {
		return err
	}

	return nil
}

func (n *PrimaryExpression) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\"];\n", n, "<primary expression>")
	if err != nil {
		return err
	}

	if n.Number != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Number)
		if err != nil {
			return err
		}

		err = n.Number.render(file)
		if err != nil {
			return err
		}
	}

	if n.Identifier != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Identifier)
		if err != nil {
			return err
		}

		err = n.Identifier.render(file)
		if err != nil {
			return err
		}
	}

	if n.ArithmeticalExpression != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.ArithmeticalExpression)
		if err != nil {
			return err
		}

		err = n.ArithmeticalExpression.render(file)
		if err != nil {
			return err
		}
	}

	return nil
}

func (n *Multiplier1) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\"];\n", n, "<multiplier>'")
	if err != nil {
		return err
	}

	if n.Pow != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Pow)
		if err != nil {
			return err
		}

		err = n.Pow.render(file)
		if err != nil {
			return err
		}
	}

	if n.PrimaryExpression != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.PrimaryExpression)
		if err != nil {
			return err
		}

		err = n.PrimaryExpression.render(file)
		if err != nil {
			return err
		}
	}

	if n.Multiplier != nil {
		_, err = fmt.Fprintf(file, "\"%p\" -> \"%p\";\n", n, n.Multiplier)
		if err != nil {
			return err
		}

		err = n.Multiplier.render(file)
		if err != nil {
			return err
		}
	}

	return nil
}

func (n *PowerOperation) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\"];\n", n, "<power operation> (^)")
	if err != nil {
		return err
	}

	return nil
}
