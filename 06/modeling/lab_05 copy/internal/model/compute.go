package model

import "math"

const (
	Eps = 1e-5
)

func (m *Model) f(x, z float64) float64 {
	return m.F0 * math.Exp(-m.Betta*(math.Pow(x-m.X0, 2)+math.Pow(z-m.Z0, 2)))
	//return m.F0*math.Exp(-m.Betta*(math.Pow(x-m.X0, 2)+math.Pow(z-m.Z0, 2))) +
	//	m.F0*math.Exp(-m.Betta*(math.Pow(x-2, 2)+math.Pow(z-2, 2)))
}

func (m *Model) lambda(u float64) float64 {
	return 1.0
}

func (m *Model) Ax() float64 {
	return m.Dz * m.Dx * m.Tau / 2
}

func (m *Model) Bx() float64 {
	return m.Dx*m.Dx*m.Dz*m.Dz + m.Tau*m.Dz*m.Dz
}

func (m *Model) Cx() float64 {
	return m.Dz * m.Dz * m.Tau / 2
}

func (m *Model) DxFunc(u_1, u, u1, x, z float64) float64 {
	return m.Dx*m.Dx*m.Dz*m.Dz*u + m.Tau/2*m.Dx*m.Dx*(u_1-2*u+u1) + m.Tau*m.Dx*m.Dx*m.Dz*m.Dz*m.f(x, z)/2/m.lambda(u)
	//-m.f(x, z)*m.Dx*m.Dx*m.Dz*m.Dz/m.lambda(u) - m.Dx*m.Dx*(u_1+u1)
}

func (m *Model) Az() float64 {
	return m.Dx * m.Dx * m.Tau / 2
}

func (m *Model) Bz() float64 {
	return m.Dx*m.Dx*m.Dz*m.Dz + m.Tau*m.Dx*m.Dx
}

func (m *Model) Cz() float64 {
	return m.Dx * m.Dx * m.Tau / 2
}

func (m *Model) DzFunc(u_1, u, u1, x, z float64) float64 {
	return m.Dx*m.Dx*m.Dz*m.Dz*u + m.Tau/2*m.Dz*m.Dz*(u_1-2*u+u1) + m.Tau*m.Dx*m.Dx*m.Dz*m.Dz*m.f(x, z)/2/m.lambda(u)
}

func (m *Model) border() float64 {
	return m.T0
}

func (m *Model) method(a, b, c, d []float64) []float64 {
	y := make([]float64, len(b))
	xi := make([]float64, len(b)-1)
	eta := make([]float64, len(b)-1)
	xi[0] = 0
	eta[0] = m.T0
	for i := 1; i < len(xi); i++ {
		tmp := b[i] - a[i]*xi[i-1]
		xi[i] = c[i] / tmp
		eta[i] = (a[i]*eta[i-1] + d[i]) / tmp
	}
	y[len(b)-1] = m.T0
	for i := 0; i < len(y)-1; i++ {
		num := len(y) - 1 - i
		y[num-1] = xi[num-1]*y[num] + eta[num-1]
	}
	return y
}

func (m *Model) calcXCoeffs(z float64, grid [][]float64) ([]float64, []float64, []float64, []float64) {
	x := m.X1
	xIndex := 0
	zIndex := int((z - m.Z1) / m.Dz)
	As := make([]float64, 0)
	Bs := make([]float64, 0)
	Cs := make([]float64, 0)
	Ds := make([]float64, 0)
	for x < m.X2-Eps {
		As = append(As, m.Ax())
		Bs = append(As, m.Bx())
		Cs = append(As, m.Cx())
		Ds = append(Ds, m.DxFunc(grid[xIndex][zIndex-1], grid[xIndex][zIndex], grid[xIndex][zIndex+1], x, z))
		x += m.Dx
		xIndex++
	}

	return As, Bs, Cs, Ds
}

func (m *Model) calcZCoeffs(x float64, grid [][]float64) ([]float64, []float64, []float64, []float64) {
	z := m.Z1
	zIndex := 0
	xIndex := int((x - m.X1) / m.Dx)
	As := make([]float64, 0)
	Bs := make([]float64, 0)
	Cs := make([]float64, 0)
	Ds := make([]float64, 0)
	for z < m.Z2-Eps {
		As = append(As, m.Ax())
		Bs = append(As, m.Bx())
		Cs = append(As, m.Cx())
		Ds = append(Ds, m.DxFunc(grid[xIndex-1][zIndex], grid[xIndex][zIndex], grid[xIndex+1][zIndex], x, z))
		z += m.Dz
		zIndex++
	}

	return As, Bs, Cs, Ds
}

func (m *Model) iteration(grid [][]float64) ([][]float64, float64) {
	nx := len(grid) - 1
	nz := len(grid[0]) - 1
	newGrid := make([][]float64, 0)
	for _, row := range grid {
		rowCopy := make([]float64, 0)
		copy(rowCopy, row)
		newGrid = append(newGrid, rowCopy)
	}
	maxErr := 5000.0

	for i := 0; i < nx+1; i++ {
		grid[i][0] = m.T0
		grid[i][nz] = m.T0
	}

	for i := 0; i < nz+1; i++ {
		grid[0][i] = m.T0
		grid[nx][i] = m.T0
	}

	its := 0
	for its < 100 && maxErr > 1e-2 {
		maxErr = 0.0
		its++
		for i := 1; i < nx; i++ {
			x := m.X1 + float64(i)*m.Dx
			As, Bs, Cs, Ds := m.calcZCoeffs(x, grid)
			us := m.method(As, Bs, Cs, Ds)
			for k := 0; k < nz+1; k++ {
				newGrid[i][k] = us[k]
				maxErr = math.Max(maxErr, math.Abs(newGrid[i][k]-grid[i][k]))
			}
		}
	}
	its = 0
	for its < 100 && maxErr > 1e-2 {
		maxErr = 0
		its++
		for i := 1; i < nz; i++ {
			z := m.Z1 + float64(i)*m.Dz
			As, Bs, Cs, Ds := m.calcXCoeffs(z, grid)
			us := m.method(As, Bs, Cs, Ds)
			for k := 0; k < nx+1; k++ {
				newGrid[k][i] = us[k]
				maxErr = math.Max(maxErr, math.Abs(newGrid[k][i]-grid[k][i]))
			}
		}
	}
	return newGrid, maxErr
}

func (m *Model) Run() [][]float64 {
	err := 5e100
	its := 0
	row := make([]float64, int((m.Z2-m.Z1)/m.Dz), int(m.T0))
	grid := make([][]float64, int((m.X2-m.X1)/m.Dx))
	for i := range grid {
		rowCopy := make([]float64, 0)
		copy(rowCopy, row)
		grid[i] = rowCopy
	}
	for its < 1 && err >= 1e-2 {
		grid, err = m.iteration(grid)
		its++
	}
	return grid
}

func (m *Model) XArray() []float64 {
	X := make([]float64, 0)
	x := m.X1
	for x < m.X2-1e-5 {
		X = append(X, x)
		x += m.Dx
	}
	return X
}

func (m *Model) YArray() []float64 {
	Z := make([]float64, 0)
	z := m.Z1
	for z < m.Z2-1e-5 {
		Z = append(Z, z)
		z += m.Dz
	}
	return Z
}
