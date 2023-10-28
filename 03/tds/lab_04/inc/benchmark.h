#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdlib.h>

#include "error.h"

#define BENCH_ARGS_ERROR 201

/*
 * Количество запусков тестов производительности
 */
#define BENCH_NUM_OF_RUNS 1000

/*
 * Получение скорости n операций "push" для стека-массива.
 * n - количество элементов;
 * timer - среднее время.
 */
error_t bench_stack_arr_push(size_t n, long long *timer);

/*
 * Получение скорости n операций "pop" для стека-массива.
 * n - количество элементов;
 * timer - среднее время.
 */
error_t bench_stack_arr_pop(size_t n, long long *timer);

/*
 * Получение скорости n операций "push" для стека-списка.
 * n - количество элементов;
 * timer - среднее время.
 */
error_t bench_stack_list_push(size_t n, long long *timer);

/*
 * Получение скорости n операций "pop" для стека-списка.
 * n - количество элементов;
 * timer - среднее время.
 */
error_t bench_stack_list_pop(size_t n, long long *timer);

#endif // BENCHMARK_H
