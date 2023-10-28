#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdlib.h>

#include "error.h"

#define BENCH_ARGS_ERROR 201

/**
 * Количество запусков тестов производительности
 */
#define BENCH_NUM_OF_RUNS 1000

/**
 * Замер времени операции "push" для очереди-массива.
 * n - количество элементов очереди;
 * timer - среднее время выполнения операции.
 */
error_t bench_queue_arr_push(size_t n, long long *timer);

/**
 * Замер времени операции "pop" для очереди-массива.
 * n - количество элементов очереди;
 * timer - среднее время выполнения операции.
 */
error_t bench_queue_arr_pop(size_t n, long long *timer);

/**
 * Замер времени операции "push" для очереди-списка.
 * n - количество элементов очереди;
 * timer - среднее время выполнения операции.
 */
error_t bench_queue_list_push(size_t n, long long *timer);

/**
 * Замер времени операции "pop" для очереди-списка.
 * n - количество элементов очереди;
 * timer - среднее время выполнения операции.
 */
error_t bench_queue_list_pop(size_t n, long long *timer);

#endif // BENCHMARK_H
