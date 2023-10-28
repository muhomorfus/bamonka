import numpy as np
import math


def full_input_matrix():
    rows, cols = [int(x) for x in input().split()]

    py_matrix = []
    for i in range(rows):
        row = [float(x) for x in input().split()]
        if len(row) != cols:
            raise Exception('Invalid number of columns')
        py_matrix.append(row)

    return np.array(py_matrix)


def delete_column(matrix, col):
    new_matrix = []
    for i in range(len(matrix)):
        row = []
        for j in range(len(matrix[i])):
            if j != col:
                row.append(matrix[i][j])
        new_matrix.append(row)

    return np.array(new_matrix)


def delete_row(matrix, row):
    for i in range(row, len(matrix) - 1):
        matrix[i] = matrix[i+1]
    matrix = matrix[:-1]
    return matrix


def find_pos_of_max(matrix):
    pos = (0, 0)
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            if matrix[i][j] > matrix[pos[0]][pos[1]]:
                pos = (i, j)

    return pos


def make_squared(matrix):
    rows = len(matrix)
    cols = len(matrix[0])
    if rows > cols:
        for i in range(rows - cols):
            pos = find_pos_of_max(matrix)
            matrix = delete_row(matrix, pos[0])
    else:
        for i in range(cols - rows):
            pos = find_pos_of_max(matrix)
            matrix = delete_column(matrix, pos[1])

    return matrix


def print_matrix(matrix):
    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            print(matrix[i][j], end=' ')
        print()


def count_row_element(matrix, j):
    avg = 0
    for i in range(len(matrix)):
        avg += matrix[i][j]
    avg /= len(matrix)
    return math.floor(avg)


def count_col_element(matrix, i):
    el_max = 0
    for j in range(len(matrix[i])):
        if matrix[i][j] > el_max:
            el_max = matrix[i][j]
    return el_max


def to_py(matrix):
    m = []
    for i in range(len(matrix)):
        row = []
        for j in range(len(matrix[i])):
            row.append(matrix[i][j])
        m.append(row)
    return m


def add_row(matrix):
    row = []
    m = to_py(matrix)
    for j in range(len(m[0])):
        row.append(count_row_element(m, j))
    m.append(row)
    return np.array(m)


def add_col(matrix):
    m = to_py(matrix)
    for i in range(len(m)):
        m[i].append(count_col_element(m, i))
    return np.array(m)


def make_one_size(matrix1, matrix2):
    size1 = len(matrix1)
    size2 = len(matrix2)
    if size1 > size2:
        for i in range(size1 - size2):
            matrix2 = add_row(matrix2)
        for i in range(size1 - size2):
            matrix2 = add_col(matrix2)
    else:
        for i in range(size2 - size1):
            matrix1 = add_row(matrix1)
        for i in range(size2 - size1):
            matrix1 = add_col(matrix1)
    return matrix1, matrix2


def create_ones_matrix(n):
    m = []
    for i in range(n):
        row = [0] * n
        row[i] = 1
        m.append(row)

    return np.array(m)

a = full_input_matrix()

d = np.linalg.det(a)

print()
print('{:.6f}'.format(d))
