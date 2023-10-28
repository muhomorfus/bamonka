#ifndef OA_H
#define OA_H

#include <stdbool.h>

#include "error.h"

#define OA_ARGS_ERROR 300

/**
 * Максимальное значение содержимого заявки.
 */
#define MAX_TASK_CONTENT 100

/**
 * Тип заявки.
 * primary - первичная, из первой очереди;
 * secondary - вторичная, из второй очереди.
 */
typedef enum task_type_t
{
    primary,
    secondary,
} task_type_t;

/**
 * Заявка.
 * content - содержимое заявки;
 * type - типа заявки;
 * enter_time - время приходя заявки в очередь.
 */
typedef struct task_t
{
    int content;
    task_type_t type;
    double enter_time;
} task_t;

/**
 * Обрабатывающий автомат.
 * current_task - текущая заявка;
 * is_working - работает ли автомат?
 * start_time - время начала обработки текущей заявки;
 * process_min_time - минимальное время обработки заявок каждого типа;
 * process_min_time - максимальное время обработки заявок каждого типа.
 */
typedef struct oa_t
{
    task_t current_task;
    bool is_working;
    double start_time;
    double process_time;
    double process_min_time[2], process_max_time[2];
} oa_t;

/**
 * Добавление заявки в автомат.
 * oa - обрабатывающий автомат;
 * task - заявка;
 * timer - текущее время.
 */
error_t oa_start(oa_t *oa, task_t task, double timer);

/**
 * Окончание работы автомата над текущей заявкой.
 * oa - обрабатывающий автомат;
 * exited - вышедшая заявка.
 */
error_t oa_finish(oa_t *oa, task_t *exited);

/**
 * Пришло ли время аппарату оканчивать работу над заявкой?
 * oa - обрабатывающий автомат;
 * timer - текущее время.
 */
bool oa_done(oa_t *oa, double timer);

/**
 * Время, когда автомат закончит работать над текущей заявкой.
 * oa - обрабатывающий автомат.
 */
double oa_end_time(oa_t *oa);

/**
 * Новая заявка.
 * type - тип заявки;
 * timer - текущее время.
 */
task_t task_new(task_type_t type, double timer);

#endif //OA_H
