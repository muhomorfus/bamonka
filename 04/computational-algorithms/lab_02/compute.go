package main

func GetRange(start, end float64, n int) []float64 {
	arr := make([]float64, 0)
	step := (end - start) / float64(n)

	for i := 0; i < n; i++ {
		arr = append(arr, start)
		start += step
	}

	return arr
}

func GetMatrix(f function, x, y, z []float64) [][][]float64 {
	matrix := make([][][]float64, len(z))

	for i := range z {
		matrix[i] = make([][]float64, len(y))

		for j := range y {
			matrix[i][j] = make([]float64, len(x))

			for k := range x {
				matrix[i][j][k] = f(x[k], y[j], z[i])
			}
		}
	}

	return matrix
}

func GetLessAndBiggerIndexes(arr []float64, x float64) (int, int) {
	for i, el := range arr {
		if el > x {
			return i - 1, i
		}
	}

	length := len(arr)
	return length - 1, length
}

func ChooseDots(arr []float64, value float64, n int) (int, int) {
	begin, end := GetLessAndBiggerIndexes(arr, value)
	remainEnd := len(arr) - end
	remainBegin := begin + 1

	if remainEnd*2 < n {
		end = len(arr)
		begin -= n - remainEnd - 1
	} else if remainBegin*2 < n {
		begin = 0
		end += n - remainBegin
	} else {
		begin -= n - n/2 - 1
		end += n / 2
	}

	return begin, end
}

func DiffsMatrix(matrix [][]float64, n int) [][]float64 {
	for i := 0; i < n; i++ {
		temp := make([]float64, 0)
		count := n

		if n >= len(matrix[1]) {
			count = len(matrix[1]) - 1
		}

		for j := 0; j < count-i; j++ {
			element := (matrix[i+1][j] - matrix[i+1][j+1]) / (matrix[0][j] - matrix[0][i+j+1])

			temp = append(temp, element)
		}

		matrix = append(matrix, temp)
	}

	return matrix
}

func NewtonsInterpolation(matrix [][]float64, value float64, n int) float64 {
	matrix = DiffsMatrix(matrix, n)

	m := float64(1)
	result := float64(0)

	count := n

	if n >= len(matrix[1]) {
		count = len(matrix[1]) - 1
	}

	for i := 0; i < count+1; i++ {
		result += m * matrix[i+1][0]
		m *= value - matrix[0][i]
	}

	return result
}

func CopyMatrix(matrix [][][]float64) [][][]float64 {
	newMatrix := make([][][]float64, len(matrix))

	for i := range matrix {
		newMatrix[i] = make([][]float64, len(matrix[i]))

		for j := range matrix[i] {
			newMatrix[i][j] = make([]float64, len(matrix[i][j]))

			for k := range matrix[i][j] {
				newMatrix[i][j][k] = matrix[i][j][k]
			}
		}
	}

	return newMatrix
}

func MultiInterpolationXYZ(matrix [][][]float64, xArr, yArr, zArr []float64, x, y, z float64, xN, yN, zN int) float64 {
	newMatrix := CopyMatrix(matrix)

	newXArr := make([]float64, len(xArr))
	copy(newXArr, xArr)

	newYArr := make([]float64, len(yArr))
	copy(newYArr, yArr)

	newZArr := make([]float64, len(zArr))
	copy(newZArr, zArr)

	ixBeg, ixEnd := ChooseDots(newXArr, x, xN+1)
	iyBeg, iyEnd := ChooseDots(newYArr, y, yN+1)
	izBeg, izEnd := ChooseDots(newZArr, z, zN+1)

	newXArr = newXArr[ixBeg:ixEnd]
	newYArr = newYArr[iyBeg:iyEnd]
	newZArr = newZArr[izBeg:izEnd]

	newMatrix = newMatrix[izBeg:izEnd]

	for i := range newZArr {
		newMatrix[i] = newMatrix[i][iyBeg:iyEnd]
	}

	for i := range newZArr {
		for j := range newYArr {
			newMatrix[i][j] = newMatrix[i][j][ixBeg:ixEnd]
		}
	}

	results2D := make([][]float64, zN+1)
	for i := range results2D {
		results2D[i] = make([]float64, yN+1)
	}

	for i := range results2D {
		for j := range results2D[i] {
			results2D[i][j] = NewtonsInterpolation([][]float64{newXArr, newMatrix[i][j]}, x, xN)
		}
	}

	results := make([]float64, zN+1)
	for i := range results {
		results[i] = NewtonsInterpolation([][]float64{newYArr, results2D[i]}, y, yN)
	}

	return NewtonsInterpolation([][]float64{newZArr, results}, z, zN)
}

func MultiInterpolationZYX(matrix [][][]float64, xArr, yArr, zArr []float64, x, y, z float64, xN, yN, zN int) float64 {
	newMatrix := CopyMatrix(matrix)

	newXArr := make([]float64, len(xArr))
	copy(newXArr, xArr)

	newYArr := make([]float64, len(yArr))
	copy(newYArr, yArr)

	newZArr := make([]float64, len(zArr))
	copy(newZArr, zArr)

	ixBeg, ixEnd := ChooseDots(newXArr, x, xN+1)
	iyBeg, iyEnd := ChooseDots(newYArr, y, yN+1)
	izBeg, izEnd := ChooseDots(newZArr, z, zN+1)

	newXArr = newXArr[ixBeg:ixEnd]
	newYArr = newYArr[iyBeg:iyEnd]
	newZArr = newZArr[izBeg:izEnd]

	newMatrix = newMatrix[izBeg:izEnd]

	for i := range newZArr {
		newMatrix[i] = newMatrix[i][iyBeg:iyEnd]
	}

	for i := range newZArr {
		for j := range newYArr {
			newMatrix[i][j] = newMatrix[i][j][ixBeg:ixEnd]
		}
	}

	results2D := make([][]float64, zN+1)
	for i := range results2D {
		results2D[i] = make([]float64, yN+1)
	}

	for i := range results2D {
		for j := range results2D[i] {
			results2D[i][j] = NewtonsInterpolation([][]float64{newZArr, newMatrix[i][j]}, z, zN)
		}
	}

	results := make([]float64, zN+1)
	for i := range results {
		results[i] = NewtonsInterpolation([][]float64{newYArr, results2D[i]}, y, yN)
	}

	return NewtonsInterpolation([][]float64{newXArr, results}, x, xN)
}
