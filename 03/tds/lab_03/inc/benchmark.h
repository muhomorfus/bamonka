#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdlib.h>

#include "usual_matrix.h"
#include "csc_matrix.h"
#include "error.h"

#define BENCH_ARGS_ERROR 201

/*
 * Количество запусков тестов производительности
 */
#define BENCH_NUM_OF_RUNS 50

/*
 * Потребление памяти матрицей в стандартном виде.
 * m - исходная матрица;
 * size - размер матрицы.
 */
error_t bench_um_memory_usage(um_matrix_t *m, size_t *size);

/*
 * Потребление памяти матрицей в расширенном столбцовом формате.
 * cm - исходная матрица;
 * size - размер матрицы.
 */
error_t bench_csc_memory_usage(csc_matrix_t *cm, size_t *size);

/*
 * Суммарное время работы стандартного умножения за BENCH_NUM_OF_RUNS
 * запусков.
 * m - исходная матрица;
 * v - исходный вектор;
 * sum_time - суммарное время.
 */
error_t bench_um_mul(um_matrix_t *m, v_vector_t *v, long long *sum_time);

/*
 * Суммарное время работы разреженного умножения за BENCH_NUM_OF_RUNS
 * запусков.
 * сm - исходная матрица;
 * v - исходный вектор;
 * sum_time - суммарное время.
 */
error_t bench_csc_mul(csc_matrix_t *cm, v_vector_t *v, long long *sum_time);

#endif // BENCHMARK_H
