#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "queue_list.h"
#include "queue_arr.h"
#include "error.h"
#include "engine.h"
#include "mystring.h"
#include "benchmark.h"
#include "worker.h"

#define PRINT_ERROR(err) printf("%d: %s (%s)\n", err.code, err.text, err.func);
#define ifok(err) if (!is_failure(err))
#define ifnok(err) if (is_failure(err))

error_t main_menu_dialog(int *answer) {
    if (!answer)
        return NEW_ERROR("передан нулевой указатель", ENG_ARGS_ERROR);

    printf("+------------------------------- Меню -------------------------------+\n");
    printf("| 01 - моделирование процесса с использованием очереди-массива;      |\n");
    printf("| 02 - моделирование процесса с использованием очереди-списка;       |\n");
    printf("| 03 - сравнение скорости работы различных реализаций очереди;       |\n");
    printf("| 04 - сравнение потребления памяти различных реализаций очереди;    |\n");
    printf("| 00 - выход.                                                        |\n");
    printf("+--------------------------------------------------------------------+\n");

    error_t err;
    printf("Выберите пункт меню [0-4]: ");
    if (is_failure(err = read_int(answer)))
        return err;

    if (*answer > 4 || *answer < 0)
        return NEW_ERROR("введен неверный пункт меню", ENG_INPUT_ERROR);

    return NEW_SUCCESS();
}

error_t read_config_times_dialog(process_config_t *config)
{
    if (!config)
        return NEW_ERROR("передан нулевой указатель", ENG_ARGS_ERROR);

    printf("Введите диапазон T1: ");
    if (scanf("%lf%lf", &config->t1_min, &config->t1_max) != 2 || config->t1_min >= config->t1_max)
        return NEW_ERROR("неверный диапазон T1", ENG_INPUT_ERROR);

    printf("Введите диапазон T2: ");
    if (scanf("%lf%lf", &config->t2_min, &config->t2_max) != 2 || config->t2_min >= config->t2_max)
        return NEW_ERROR("неверный диапазон T2", ENG_INPUT_ERROR);

    printf("Введите диапазон T3: ");
    if (scanf("%lf%lf", &config->t3_min, &config->t3_max) != 2  || config->t3_min >= config->t3_max)
        return NEW_ERROR("неверный диапазон T3", ENG_INPUT_ERROR);

    printf("Введите диапазон T4: ");
    if (scanf("%lf%lf", &config->t4_min, &config->t4_max) != 2 || config->t4_min >= config->t4_max)
        return NEW_ERROR("неверный диапазон T4", ENG_INPUT_ERROR);

    fflush(stdin);

    return NEW_SUCCESS();
}

error_t ql_push_address(void *q, task_t el)
{
    queue_list_t *ql = q;

    if (!ql)
        return NEW_ERROR("передан нулевой указатель", QL_ARGS_ERROR);

    queue_node_t *node = calloc(1, sizeof(queue_node_t));
    if (!node)
        return NEW_ERROR("ошибка выделения памяти", QL_ALLOC_ERROR);

    node->content = el;
    if (ql_empty(ql))
    {
        ql->tail = node;
        ql->head = node;
    }
    else
    {
        ql->tail->next = node;
        ql->tail = ql->tail->next;
    }

    printf("PUSH: %p\n", (void *) ql->tail);
    ql->len++;

    return NEW_SUCCESS();
}

error_t ql_pop_address(void *q, task_t *el)
{
    queue_list_t *ql = q;

    if (!ql)
        return NEW_ERROR("передан нулевой указатель", QL_ARGS_ERROR);

    if (ql_empty(ql))
        return NEW_ERROR("очередь пуста", QL_EMPTY_ERROR);

    *el = ql->head->content;
    queue_node_t *next = ql->head->next;
    printf("POP: %p\n", (void *) ql->head);
    free(ql->head);
    ql->head = next;
    ql->len--;
    if (ql_empty(ql))
        ql->tail = NULL;

    return NEW_SUCCESS();
}

error_t eng_work()
{
    printf("+----------------------------------------------------------------+\n");
    printf("| Программа для работы со очередями                              |\n");
    printf("| Программа осуществяет моделирования процесса и использованием  |\n");
    printf("| очередей на основе массива и списка.                           |\n");
    printf("|                                                                |\n");
    printf("| Кроме того, производится сравнение производительности двух     |\n");
    printf("| методов хранения очередей, а также есть возможность просмотра  |\n");
    printf("| адресов добавляемых и удаляемых элементов из очереди-списка.   |\n");
    printf("+----------------------------------------------------------------+\n\n");
    int variant = -1;
    error_t err;

    while (variant)
    {
        err = NEW_SUCCESS();
        err = main_menu_dialog(&variant);
        ifnok(err)
        {
            PRINT_ERROR(err);
            continue;
        }

        if (variant == 2)
        {
            queue_list_t ql1 = ql_create();
            queue_list_t ql2 = ql_create();

            error_t (*push_f) (void *, task_t) = ql_push;
            error_t (*pop_f) (void *, task_t *) = ql_pop;

            printf("+-------------------------------------------------------+\n");
            printf("| Выводить адреса добавляемых и удаляемых элементов?    |\n");
            printf("| 1 - да;                                               |\n");
            printf("| 2 - нет.                                              |\n");
            printf("+-------------------------------------------------------+\n");
            printf("Выберите вариант ответа [1-2]: ");
            int answer;
            err = read_int(&answer);
            ifok(err)
                if (answer != 1 && answer != 2)
                    err = NEW_ERROR("неверный вариант ответа", ENG_INPUT_ERROR);

            ifok(err)
                if (answer == 1)
                {
                    push_f = ql_push_address;
                    pop_f = ql_pop_address;
                }

            process_config_t config = {
                    .push = push_f,
                    .pop = pop_f,
                    .len = ql_len,
                    .empty = ql_empty,

                    .q1 = &ql1,
                    .q2 = &ql2,
            };

            ifok(err)
                err = read_config_times_dialog(&config);
            ifok(err)
                err = process(config);

            ql_free(&ql1);
            ql_free(&ql2);
        }
        else if (variant == 1)
        {
            queue_array_t qa1 = qa_create();
            queue_array_t qa2 = qa_create();

            process_config_t config = {
                    .push = qa_push,
                    .pop = qa_pop,
                    .len = qa_len,
                    .empty = qa_empty,

                    .q1 = &qa1,
                    .q2 = &qa2,
            };

            ifok(err)
                err = read_config_times_dialog(&config);
            ifok(err)
                err = process(config);
        }
        else if (variant == 3)
        {
            int n;
            printf("Введите количество добавляемых/удаляемых элементов: ");
            err = read_int(&n);
            ifok(err)
            {
                if (n < 0)
                    err = NEW_ERROR("неверное количество элементов очереди", ENG_INPUT_ERROR);
                if (n > QA_QUEUE_SIZE)
                    err = NEW_ERROR("слишком большой размер", QA_OVERFLOW_ERROR);
            }
            long long time_arr_push, time_arr_pop;
            long long time_list_push, time_list_pop;

            ifok(err)
                err = bench_queue_arr_push(n, &time_arr_push);

            ifok(err)
                err = bench_queue_arr_pop(n, &time_arr_pop);

            ifok(err)
                err = bench_queue_list_push(n, &time_list_push);

            ifok(err)
                err = bench_queue_list_pop(n, &time_list_pop);

            ifok(err)
            {
                printf("Для %d запусков и %d элементов.\n", BENCH_NUM_OF_RUNS, n);
                printf("Очередь-массив\n");
                printf("  PUSH: %lld us\n", time_arr_push);
                printf("  POP: %lld us\n", time_arr_pop);

                printf("Очередь-список\n");
                printf("  PUSH: %lld us\n", time_list_push);
                printf("  POP: %lld us\n", time_list_pop);

                printf("\nПреимущество очереди-массива над очередью-списком\n");
                printf("  PUSH: %.0lf %%\n", ((double) time_list_push / (double) time_arr_push - 1) * 100);
                printf("  POP: %.0lf %%\n", ((double) time_list_pop / (double) time_arr_pop - 1) * 100);
            }
        }
        else if (variant == 4)
        {
            int n;
            printf("Введите количество элементов: ");
            err = read_int(&n);
            ifok(err)
            {
                if (n < 0)
                    err = NEW_ERROR("неверное количество элементов очереди", ENG_INPUT_ERROR);
                if (n > QA_QUEUE_SIZE)
                    err = NEW_ERROR("слишком большой размер", QA_OVERFLOW_ERROR);
            }

            ifok(err)
            {
                int size_arr = (int) sizeof(queue_array_t);
                int size_list = n * (int) sizeof(queue_node_t) + (int) sizeof(queue_list_t);
                printf("Для %d элементов.\n", n);
                printf("Очередь-массив\n");
                printf("  Объем занимаемой памяти: %d B\n", size_arr);
                printf("\nОчередь-список\n");
                printf("  Объем занимаемой памяти: %d B\n", size_list);

                printf("\nПреимущество очереди-списка над очередью-массивом\n");
                printf("  Объем памяти: %.0lf %%\n", ((double) size_arr / (double) size_list - 1) * 100);
            }
        }

        ifnok(err)
            PRINT_ERROR(err);
    }

    return err;
}
