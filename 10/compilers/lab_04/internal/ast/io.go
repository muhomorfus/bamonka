package ast

import (
	"fmt"
	"os"
	"os/exec"
	"strings"
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

	return nil
}

func (n *Identifier) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s (%s)\n%s\"];\n", n, "<identifier>", n.Value, strings.Join(n.Postfix, " "))
	if err != nil {
		return err
	}

	return nil
}

func (n *Expression) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\n%s\"];\n", n, "<expression>", strings.Join(n.Postfix, " "))
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
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\n%s\"];\n", n, "<arithmetical expression>", strings.Join(n.Postfix, " "))
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
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\n%s\"];\n", n, "<term>", strings.Join(n.Postfix, " "))
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
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\n%s\"];\n", n, "<multiplier>", strings.Join(n.Postfix, " "))
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
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s (%s)\n%s\"];\n", n, "<number>", n.Value, strings.Join(n.Postfix, " "))
	if err != nil {
		return err
	}

	return nil
}

func (n *RelationOperation) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s (%s)\n%s\"];\n", n, "<relation operation>", n.Value, strings.Join(n.Postfix, " "))
	if err != nil {
		return err
	}

	return nil
}

func (n *ArithmeticalExpression1) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\n%s\"];\n", n, "<arithmetical expression>'", strings.Join(n.Postfix, " "))
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
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\n%s\"];\n", n, "<term>'", strings.Join(n.Postfix, " "))
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
	label := "<sum operation>"
	if n.Unary {
		label = "<unary sum operation>"
	}

	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s (%s)\n%s\"];\n", n, label, n.Value, strings.Join(n.Postfix, " "))
	if err != nil {
		return err
	}

	return nil
}

func (n *MulOperation) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s (%s)\n%s\"];\n", n, "<mul operation>", n.Value, strings.Join(n.Postfix, " "))
	if err != nil {
		return err
	}

	return nil
}

func (n *PrimaryExpression) render(file *os.File) error {
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\n%s\"];\n", n, "<primary expression>", strings.Join(n.Postfix, " "))
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
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s\n%s\"];\n", n, "<multiplier>'", strings.Join(n.Postfix, " "))
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
	_, err := fmt.Fprintf(file, "\"%p\" [label=\"%s (^)\n%s\"];\n", n, "<power operation>", strings.Join(n.Postfix, " "))
	if err != nil {
		return err
	}

	return nil
}
