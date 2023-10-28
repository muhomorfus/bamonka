#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdint.h>

/*
 * Функция тестирования по времени.
 * f - функция, принимающая базу данных и преобразующая
 *     как-либо ее;
 * db - исходная база данных.
 */
long long benchmark(void (*f) (flats_db_t *), flats_db_t *db);

#endif //BENCHMARK_H
