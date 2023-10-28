#ifndef csc_MATRIX_H
#define csc_MATRIX_H

#include <stdio.h>
#include <stdlib.h>

#include "size_t_list.h"
#include "vector.h"
#include "error.h"
#include "usual_matrix.h"

#define csc_ARGS_ERROR 141
#define csc_ALLOC_ERROR 142
#define csc_READ_ERROR 143
#define csc_WRITE_ERROR 144

/*
 * Матрица в разреженном виде.
 * n - количество ненулевых элементов;
 * num_of_cols - количество столбцов матрицы;
 * num_of_rows - количество строк матрицы;
 * allocated_n - количество выделенной памяти под ненулевые
 *               элементы;
 * allocated_cols - количество выделенной памяти под столбцы;
 * values - массив с ненулевыми значениями;
 * rows - массив с номерами строк, в которых содержатся
 *        соответствующие значения;
 * col_list - односвязный список с номерами элементов в values,
 *            с которых нчинается новая строка.
 */
typedef struct csc_matrix_t
{
    size_t n;
    size_t num_of_cols;
    size_t num_of_rows;

    size_t allocated_n;
    size_t allocated_cols;

    double *values;
    size_t *rows;

    stl_list_t *col_list;
} csc_matrix_t;

/*
 * Выделена ли под матрицу память.
 * m - исходная матрица.
 */
bool csc_allocated(csc_matrix_t *m);

/*
 * Инициализирована ли матрица.
 * m - исходная матрица.
 */
bool csc_initialized(csc_matrix_t *m);

/*
 * Создание пустого неаллоцированного "объекта" матрицы.
 */
csc_matrix_t csc_csc();

/*
 * Освобождение матрицы.
 */
void csc_free(csc_matrix_t *m);

/*
 * Выделение памяти под матрицу.
 * m - исходная матрица;
 * n - количество ненулевых элементов;
 * cols - количество столбцов матрицы.
 */
error_t csc_allocate(csc_matrix_t *m, size_t n, size_t cols);

/*
 * Инициалиация матрицы.
 * m - исходная матрица;
 * n - количество ненулевых элементов;
 * rows - количество столбцов матрицы;
 * cols - количество столбцов матрицы.
 */
error_t csc_init(csc_matrix_t *m, size_t n, size_t rows, size_t cols);

/*
 * Перевод матрицы в обычном виде в разреженную матрицу.
 * m - исходная матрица;
 * src - источник.
 */
error_t csc_from_um(csc_matrix_t *m, um_matrix_t *src);

/*
 * Запись матрицы в разреженном виде в файл.
 * m - исходная матрица;
 * f - файл.
 */
error_t csc_write(csc_matrix_t *m, FILE *f);

/*
 * Умножение матрицы в разреженном виде на вектор.
 * res - результат;
 * a - первый операнд;
 * b - второй операнд.
 */
error_t csc_multiply_with_v(csc_matrix_t *res, csc_matrix_t *a, v_vector_t *b);

#endif //csc_MATRIX_H
