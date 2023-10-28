#ifndef USUAL_MATRIX_H
#define USUAL_MATRIX_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "error.h"
#include "vector.h"

#define UM_ALLOC_ERROR 100
#define UM_ARGS_ERROR 101
#define UM_OUT_OF_RANGE_ERROR 102
#define UM_POINTER_ERROR 103
#define UM_READ_ERROR 104
#define UM_PRINT_ERROR 105
#define UM_ARITHMETIC_ERROR 106
#define UM_REPEAT_ERROR 107

/*
 * Верхняя граница значения случайного элемента.
 */
#define UM_RANDOM_ELEMENT_BORDER 10

/*
 * Матрица в стандартном виде.
 * ptr - указатель на матрицу;
 * cols - количество столбцов;
 * rows - количество строк;
 * allocated_cols - размер выделенной памяти под столбцы;
 * allocated_rows - размер выделенной памяти под строки.
 */
typedef struct um_matrix_t
{
    double **ptr;
    size_t cols;
    size_t rows;
    size_t allocated_cols;
    size_t allocated_rows;
} um_matrix_t;

/*
 * Выделена ли под матрицу память.
 * m - исходная матрица.
 */
bool um_allocated(um_matrix_t *m);

/*
 * Инициализирована ли матрица.
 * m - исходная матрица.
 */
bool um_initialized(um_matrix_t *m);

/*
 * Создание пустого неаллоцированного "объекта" матрицы.
 */
um_matrix_t um_um();

/*
 * Выделение памяти под матрицу.
 * m - исходная матрица;
 * rows - количество строк матрицы;
 * cols - количество столбцов матрицы.
 */
error_t um_allocate(um_matrix_t *m, size_t rows, size_t cols);

/*
 * Инициалиация матрицы.
 * m - исходная матрица;
 * rows - количество столбцов матрицы;
 * cols - количество столбцов матрицы.
 */
error_t um_init(um_matrix_t *m, size_t rows, size_t cols);

/*
 * Освобождение матрицы.
 */
void um_free(um_matrix_t *m);

/*
 * Чтение размеров матрицы в формате ROWSxCOLS.
 * rows - количество строк;
 * cols - количество столбцов;
 * f - файл.
 */
error_t um_read_size(size_t *rows, size_t *cols, FILE *f);

/*
 * Считывание матрицы из файла.
 * m - исходная матрица;
 * f - файл.
 */
error_t um_read(um_matrix_t *m, FILE *f);

/*
 * Считывание матрицы в координатном виде.
 * m - исходная матрица;
 * f - файл;
 * not_zero - количество ненулевых элементов.
 */
error_t um_read_coordinates(um_matrix_t *m, FILE *f, size_t not_zero);

/*
 * Генерация случайно матрицы.
 * m - исходная матрица;
 * percent - процент заполненности матрицы.
 */
error_t um_generate(um_matrix_t *m, int percent);

/*
 * Получение матрицы из вектора.
 * m - матрица;
 * v - преобразуемый вектор.
 */
error_t um_from_v(um_matrix_t *m, v_vector_t *v);

/*
 * Запись матрицы в файл.
 * m - исходная матрица;
 * f - файл.
 */
error_t um_write(um_matrix_t *m, FILE *f);

/*
 * Умножение матрциы на вектор.
 * res - матрица-результат;
 * a - первый операнд;
 * b - второй операнд.
 */
error_t um_multiply_with_v(um_matrix_t *res, um_matrix_t *a, v_vector_t *b);

/*
 * Подсчет количества ненулевых элементов в матрице.
 * m - исходная матрица;
 * non_zeros - количество ненулевых элементов.
 */
error_t um_count_non_zeros(um_matrix_t *m, size_t *non_zeros);

#endif //USUAL_MATRIX_H
