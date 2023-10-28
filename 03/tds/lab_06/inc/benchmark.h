#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdlib.h>

#include "error.h"
#include "bst.h"
#include "avl.h"
#include "ht.h"
#include "file.h"

#define BENCH_ARGS_ERROR 200
#define BENCH_ALLOC_ERROR 201
#define BENCH_UNKNOWN_ERROR 202
#define BENCH_OPEN_ERROR 203

#define BENCH_CHECK_TIME(p,t) { \
uint64_t start = bench_tick(); \
p;\
uint64_t finish = bench_tick();\
t = (long long int) (finish - start);}

/**
 * Количество запусков тестов производительности
 */
#define BENCH_NUM_OF_RUNS 100

/**
 * Возвращает количество тактов процессора.
 */
int64_t bench_tick(void);

/**
 * Измерение скорости удаления из ДДП.
 * n - количество удаляемых элементов;
 * timer - среднее время удаления.
 */
error_t bench_bst_remove(size_t n, long long *timer, size_t *n_cmp);

/**
 * Измерение скорости поиска в ДДП.
 * n - количество элементов;
 * timer - среднее поиска.
 */
error_t bench_bst_search(size_t n, long long *timer, size_t *n_cmp);

/**
 * Получение объема памяти, занимаемого ДДП.
 * n - количество элементов;
 * size - объем памяти.
 */
error_t bench_bst_size(size_t n, size_t *size);

/**
 * Измерение скорости удаления из АВЛ-дерева.
 * n - количество удаляемых элементов;
 * timer - среднее время удаления.
 */
error_t bench_avl_remove(size_t n, long long *timer, size_t *n_cmp);

/**
 * Измерение скорости поиска в АВЛ-дереве.
 * n - количество элементов;
 * timer - среднее поиска.
 */
error_t bench_avl_search(size_t n, long long *timer, size_t *n_cmp);

/**
 * Получение объема памяти, занимаемого АВЛ-деревом.
 * n - количество элементов;
 * size - объем памяти.
 */
error_t bench_avl_size(size_t n, size_t *size);

/**
 * Измерение скорости удаления из хеш-таблицы.
 * n - количество удаляемых элементов;
 * cap - емкость хеш-таблицы;
 * timer - среднее время удаления.
 */
error_t bench_ht_remove(size_t n, size_t cap, long long *timer, size_t *n_cmp);

/**
 * Измерение скорости поиска в хеш-таблице.
 * n - количество элементов;
 * timer - среднее поиска.
 */
error_t bench_ht_search(size_t n, size_t cap, long long *timer, size_t *n_cmp);

/**
 * Получение объема памяти, занимаемого хеш-таблицей.
 * n - количество элементов;
 * size - объем памяти.
 */
error_t bench_ht_size(size_t n, size_t cap, size_t *size);

/**
 * Измерение скорости удаления из текстового файла.
 * n - количество удаляемых элементов;
 * timer - среднее время удаления.
 */
error_t bench_file_remove(size_t n, long long *timer, size_t *n_cmp);

/**
 * Измерение скорости поиска в текстовом файле.
 * n - количество элементов;
 * timer - среднее поиска.
 */
error_t bench_file_search(size_t n, long long *timer, size_t *n_cmp);

/**
 * Получение объема памяти, занимаемого текстовым файлом.
 * n - количество элементов;
 * size - объем памяти.
 */
error_t bench_file_size(size_t n, size_t *size);

#endif // BENCHMARK_H
