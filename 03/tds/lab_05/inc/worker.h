#ifndef WORKER_H
#define WORKER_H

#include <stdbool.h>

#include "error.h"
#include "oa.h"

#define WRK_ARGS_ERROR 300

/**
 * Количество обработанных заявок первого типа для выхода.
 */
#define PROCESSED_STOP 1000

/**
 * Количество обработанных заявок первого типа для вывода статистики.
 */
#define PROCESSED_STATS 100

/**
 * Параметр моделирования.
 * push - функция "push" для очереди;
 * pop - функция "pop" для очереди;
 * len - функция "len" для очереди;
 * empty - проверка очереди на пустоту;
 * q1 - первая очередь;
 * q2 - вторая очередь;
 * t1_min, t1_max - диапазон времени на T1;
 * t2_min, t2_max - диапазон времени на T2;
 * t3_min, t3_max - диапазон времени на T3;
 * t4_min, t4_max - диапазон времени на T4.
 */
typedef struct process_config_t
{
    error_t (*push) (void *, task_t);
    error_t (*pop) (void *, task_t *);
    error_t (*len) (void *, size_t *);
    bool (*empty) (void *);

    void *q1;
    void *q2;

    double t1_min, t1_max;
    double t2_min, t2_max;
    double t3_min, t3_max;
    double t4_min, t4_max;
} process_config_t;

/**
 * Основной цикл моделирования процесса.
 * config - настройки моделирования.
 */
error_t process(process_config_t config);

#endif //WORKER_H
