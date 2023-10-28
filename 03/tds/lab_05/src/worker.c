#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "worker.h"
#include "error.h"
#include "oa.h"

typedef struct process_t
{
    int processed[2];
    int entered[2];
    int discarded;
    double downtime;
    int exited_from_queue[2];
    double sum_time[2];

    size_t current_sum_len[2];
} process_t;

static double random_double(double min, double max)
{
    double n = rand();
    n /= INT_MAX;
    n *= max - min;
    n += min;
    return n;
}

double min_double(double a, double b)
{
    if (a < b)
        return a;

    return b;
}

double max_double(double a, double b)
{
    return a + b - min_double(a, b);
}

double get_theoretic_time(process_config_t config)
{
    double avg_queue_time = (config.t1_min + config.t1_max) / 2;
    double avg_oa_time = (config.t3_min + config.t3_max) / 2;
    double timer = avg_oa_time;
    if (avg_queue_time > timer)
        timer = avg_queue_time;

    return timer * PROCESSED_STOP;
}

void *get_queue(process_config_t config, int num)
{
    switch(num)
    {
        case primary:
            return config.q1;
        case secondary:
            return config.q2;
        default:
            return NULL;
    }
}

error_t print_current_stats(process_config_t config, process_t stats)
{
    error_t err;
    size_t len1, len2;
    if (is_failure(err = config.len(get_queue(config, primary), &len1)))
        return err;
    if (is_failure(err = config.len(get_queue(config, secondary), &len2)))
        return err;

    printf("------------------------------------------------\n");
    printf("Обработано %d заявок первого типа.\n", stats.processed[primary]);
    printf("\nИнформация об очередях\n");
    printf("ОЧЕРЕДЬ 1:\n");
    printf("  Среднее количество элементов = %.1lf\n", (double) stats.current_sum_len[primary] / PROCESSED_STATS);
    printf("  Текущая длина очереди = %zu\n", len1);

    printf("ОЧЕРЕДЬ 2:\n");
    printf("  Среднее количество элементов = %.1lf\n", (double) stats.current_sum_len[secondary] / PROCESSED_STATS);
    printf("  Текущая длина очереди = %zu\n", len2);
    printf("------------------------------------------------\n\n");

    return NEW_SUCCESS();
}

error_t print_final_stats(process_config_t config, process_t stats, double timer)
{
    error_t err;
    size_t len1, len2;
    if (is_failure(err = config.len(get_queue(config, primary), &len1)))
        return err;
    if (is_failure(err = config.len(get_queue(config, secondary), &len2)))
        return err;

    double th_timer = get_theoretic_time(config);
    double percent = fabs(timer - th_timer) / max_double(timer, th_timer) * 100;

    double avg_time1 = stats.sum_time[primary] / stats.exited_from_queue[primary];
    double avg_time2 = stats.sum_time[secondary] / stats.exited_from_queue[secondary];

    if (stats.exited_from_queue[primary] == 0)
        avg_time1 = 0;
    if (stats.exited_from_queue[secondary] == 0)
        avg_time2 = 0;

    printf("Работа системы завершена.\n");

    printf("Общее время моделирования = %.1lf\n", timer);
    printf("Расчетное время моделирования = %.1lf\n", th_timer);
    printf("Расхождение = %.1lf %%\n", percent);
    printf("\nИнформация об очередях\n");
    printf("ОЧЕРЕДЬ 1:\n");
    printf("  Количество вошедших заявок = %d\n", stats.entered[primary]);
    printf("  Количество вышедших заявок = %d\n", stats.processed[primary]);
    printf("  Среднее время пребывания заявки в очереди = %.1lf\n", avg_time1);

    printf("ОЧЕРЕДЬ 2:\n");
    printf("  Количество вошедших заявок = %d\n", stats.entered[secondary]);
    printf("  Количество вышедших заявок = %d\n", stats.processed[secondary]);
    printf("  Среднее время пребывания заявки в очереди = %.1lf\n", avg_time2);
    printf("  Количество \"выброшенных\" заявок = %d\n", stats.discarded);

    return NEW_SUCCESS();
}

error_t process(process_config_t config)
{
    srand(time(NULL));

    if (!config.empty || !config.pop || !config.push)
        return NEW_ERROR("передан нулевой указатель на функцию", WRK_ARGS_ERROR);

    if (!config.q1 || !config.q2)
        return NEW_ERROR("передан нулевой указатель на очередь", WRK_ARGS_ERROR);

    double current_time = 0.0;
    double add_to_queue_time[2] = {0.0};

    oa_t oa = {
        .process_min_time = {config.t3_min, config.t4_min},
        .process_max_time = {config.t3_max, config.t4_max},
    };
    process_t stats = { 0 };

    add_to_queue_time[primary] = random_double(config.t1_min, config.t1_max);
    add_to_queue_time[secondary] = random_double(config.t2_min, config.t2_max);

    current_time = min_double(add_to_queue_time[primary], add_to_queue_time[secondary]);

    error_t err = NEW_SUCCESS();

    while (stats.processed[primary] < PROCESSED_STOP)
    {
//        printf("TIME: %lf\n", current_time);
//        printf("IN OA: %d, TYPE: %d, WORKING: %d\n", oa.current_task.content, oa.current_task.type, oa.is_working);
//        if (oa.is_working)
//            printf("OA WILL DONE WORK: %lf\n", oa.start_time + oa.process_time);
        if (oa_done(&oa, current_time))
        {
//            printf("OA DONE\n");
            task_t exited;
            if (is_failure(err = oa_finish(&oa, &exited)))
                return err;

            stats.processed[exited.type]++;

            if (exited.type == primary)
                for (int i = 0; i < 2; i++)
                {
                    size_t len;
                    if (is_failure(err = config.len(get_queue(config, i), &len)))
                        return err;
                    stats.current_sum_len[i] += len;
                }

            if (exited.type == primary && stats.processed[primary] % PROCESSED_STATS == 0)
            {
                if (is_failure(err = print_current_stats(config, stats)))
                    return err;
                stats.current_sum_len[primary] = 0;
                stats.current_sum_len[secondary] = 0;
            }
        }
        if (current_time >= add_to_queue_time[primary])
        {
//            printf("PRIMARY ADD\n");
            if (is_failure(err = config.push(get_queue(config, primary), task_new(primary, current_time))))
                return err;

            stats.entered[primary]++;
            add_to_queue_time[primary] = current_time + random_double(config.t1_min, config.t1_max);
//            printf("NEXT PRIMARY ADD: %lf\n", add_to_queue_time[primary]);
        }
        if (current_time >= add_to_queue_time[secondary])
        {
//            printf("SECONDARY ADD\n");
            if (is_failure(err = config.push(get_queue(config, secondary), task_new(secondary, current_time))))
                return err;

            stats.entered[secondary]++;
            add_to_queue_time[secondary] = current_time + random_double(config.t2_min, config.t2_max);
//            printf("NEXT SECONDARY ADD: %lf\n", add_to_queue_time[secondary]);
        }

        if (oa.is_working && oa.current_task.type == secondary && !config.empty(get_queue(config, primary)))
        {
//            printf("DISCARDING\n");
            task_t discarded = oa.current_task;
            discarded.enter_time = current_time;
            if (is_failure(err = config.push(get_queue(config, secondary), discarded)))
                return err;
            stats.discarded++;
            oa.is_working = false;
        }

        if (!oa.is_working)
        {
            double last_exited_time = oa.process_time + oa.start_time;

            for (int i = 0; i < 2; i++)
            {
                if (!config.empty(get_queue(config, i)))
                {
//                    printf("ADD NEW TASK\n");
                    void *q = get_queue(config, i);
                    task_t task;
                    if (is_failure(err = config.pop(q, &task)))
                        return err;

                    stats.exited_from_queue[i]++;
                    stats.sum_time[i] = current_time - task.enter_time;

                    if (is_failure(err = oa_start(&oa, task, current_time)))
                        return err;

//                    printf("OA WILL DONE NEW WORK: %lf\n", oa.start_time + oa.process_time);
                    stats.downtime += current_time - last_exited_time;
                    break;
                }
            }
        }

        current_time = min_double(add_to_queue_time[primary], add_to_queue_time[secondary]);
        if (oa.is_working)
            current_time = min_double(current_time, oa_end_time(&oa));
    }

    if (is_failure(err = print_final_stats(config, stats, current_time)))
        return err;

    return NEW_SUCCESS();
}
