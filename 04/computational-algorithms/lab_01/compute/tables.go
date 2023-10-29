package compute

import (
	"fmt"

	"github.com/jedib0t/go-pretty/v6/table"

	"lab_01/func_info"
)

func PrintNewtonDiffsTable(fis []func_info.FuncInfo, x float64, n int) error {
	fis, err := GetConfiguration(fis, x, n)
	if err != nil {
		return err
	}

	xs := make([]float64, len(fis))
	ys := make([]float64, len(fis))

	for i, fi := range fis {
		xs[i] = fi.X
		ys[i] = fi.Y
	}

	diffsTable := make([][]float64, 0)
	diffs := make([]float64, 1)
	diffs[0] = ys[0]

	diffsTable = append(diffsTable, append([]float64(nil), ys...))
	for i := 1; i < len(ys); i++ {
		for j := 0; j < len(ys)-i; j++ {
			ys[j] = (ys[j] - ys[j+1]) / (xs[j] - xs[j+i])
		}
		diffsTable = append(diffsTable, append([]float64(nil), ys[:len(ys)-i]...))
		diffs = append(diffs, ys[0])
	}

	tw := table.NewWriter()
	for j := 0; j < len(fis); j++ {
		row := make(table.Row, 0)
		for i := range diffsTable {
			if len(diffsTable[i]) <= j {
				break
			}
			row = append(row, fmt.Sprintf("%.4f ", diffsTable[i][j]))
		}
		tw.AppendRow(row)
	}
	tw.SetTitle("Таблица разностей для метода Ньютона")
	fmt.Println(tw.Render())

	return nil
}

func PrintInterpolationTable(fis []func_info.FuncInfo, x float64, n int) error {
	tw := table.NewWriter()
	tw.AppendHeader(table.Row{"/", "Ньютон", "Эрмит"})
	for i := 2; i <= n; i++ {
		resNewton, err := Polynomial(fis, x, i+1, NewtonDiffs)
		if err != nil {
			return err
		}
		resHermit, err := Polynomial(fis, x, i+1, HermitDiffs)
		if err != nil {
			return err
		}

		resNewtonStr := fmt.Sprintf("%.4f", resNewton)
		resHermitStr := fmt.Sprintf("%.4f", resHermit)
		tw.AppendRow(table.Row{fmt.Sprintf("n = %d", i), resNewtonStr, resHermitStr})
	}
	tw.SetTitle("Интерполяция х = %.4f\n", x)
	fmt.Println(tw.Render())
	return nil
}

func PrintRootTable(fis []func_info.FuncInfo, x float64, n int) error {
	tw := table.NewWriter()
	tw.AppendHeader(table.Row{"/", "Ньютон", "Эрмит"})
	for i := 2; i <= n; i++ {
		resNewton, err := FindRoot(fis, x, i+1, NewtonDiffs)
		if err != nil {
			continue
		}
		resHermit, err := FindRoot(fis, x, i+1, HermitDiffs)
		if err != nil {
			continue
		}

		resNewtonStr := fmt.Sprintf("%.4f", resNewton)
		resHermitStr := fmt.Sprintf("%.4f", resHermit)
		tw.AppendRow(table.Row{fmt.Sprintf("n = %d", i), resNewtonStr, resHermitStr})
	}
	tw.SetTitle("Поиск корня")
	fmt.Println(tw.Render())
	return nil
}
