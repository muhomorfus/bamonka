#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "stack_list.h"
#include "stack_arr.h"
#include "ptr_list.h"
#include "error.h"
#include "engine.h"
#include "mystring.h"
#include "benchmark.h"

#define PRINT_ERROR(err) printf("%d: %s (%s)\n", err.code, err.text, err.func);
#define ifok(err) if (!is_failure(err))
#define ifnok(err) if (is_failure(err))

error_t sl_pop_areas(stack_list_t **sl, mystring_t str, ptr_list_t **areas)
{
    if (!sl || !areas || !str)
        return NEW_ERROR("переданы нулевые указатели", PTR_ARGS_ERROR);
    error_t err;
    void *address = *sl;
    err = sl_pop(sl, str);
    ifok(err)
        err = pl_add(areas, address);

    return err;
}

error_t sl_push_areas(stack_list_t **sl, mystring_t str, ptr_list_t **areas)
{
    if (!sl || !areas || !str)
        return NEW_ERROR("переданы нулевые указатели", PTR_ARGS_ERROR);
    error_t err;
    err = sl_push(sl, str);
    ifok(err)
        err = pl_delete(areas, *sl);

    return err;
}

error_t sl_print_reversed(stack_list_t **sl, ptr_list_t **areas)
{
    if (!sl || !areas)
        return NEW_ERROR("переданы нулевые указатели", PTR_ARGS_ERROR);

    if (sl_empty(sl))
        return NEW_ERROR("стек-список пуст", PTR_ARGS_ERROR);

    mystring_t str;
    error_t err = NEW_SUCCESS();
    while (!is_failure(err = sl_pop_areas(sl, str, areas)))
    {
        reverse(str);
        printf("\"%s\"\n", str);
    }

    if (err.code != SL_EMPTY_ERROR)
        return err;

    return NEW_SUCCESS();
}

error_t sa_print_reversed(stack_array_t *sa)
{
    if (!sa)
        return NEW_ERROR("переданы нулевые указатели", PTR_ARGS_ERROR);

    if (sa_empty(sa))
        return NEW_ERROR("стек-массив пуст", PTR_ARGS_ERROR);

    mystring_t str;
    error_t err = NEW_SUCCESS();
    while (!is_failure(err = sa_pop(sa, str)))
    {
        reverse(str);
        printf("\"%s\"\n", str);
    }

    if (err.code != SA_EMPTY_ERROR)
        return err;

    return NEW_SUCCESS();
}

error_t sa_print_state(stack_array_t *sa)
{
    if (!sa)
        return NEW_ERROR("переданы нулевые указатели", PTR_ARGS_ERROR);

    if (sa_empty(sa))
        return NEW_ERROR("стек-массив пуст", PTR_ARGS_ERROR);

    mystring_t str;
    error_t err = NEW_SUCCESS();
    stack_array_t buf = sa_create();
    while (!is_failure(err))
    {
        err = sa_pop(sa, str);
        ifok(err)
            err = sa_push(&buf, str);
        ifok(err)
            printf("\"%s\"\n", str);
    }

    if (err.code != SA_EMPTY_ERROR)
        return err;

    err = NEW_SUCCESS();
    while (!is_failure(err))
    {
        err = sa_pop(&buf, str);
        ifok(err)
            err = sa_push(sa, str);
    }

    if (err.code != SA_EMPTY_ERROR)
        return err;

    return NEW_SUCCESS();
}

error_t sl_print_state(stack_list_t **sl, ptr_list_t **areas)
{
    if (!sl || !areas)
        return NEW_ERROR("переданы нулевые указатели", PTR_ARGS_ERROR);

    if (sl_empty(sl))
        return NEW_ERROR("стек-список пуст", PTR_ARGS_ERROR);

    mystring_t str;
    error_t err = NEW_SUCCESS();
    stack_list_t *buf = NULL;
    while (!is_failure(err))
    {
        void *address = *sl;
        err = sl_pop_areas(sl, str, areas);
        ifok(err)
            err = sl_push_areas(&buf, str, areas);
        ifok(err)
            printf("%p \"%s\"\n", address, str);
    }

    if (err.code != SL_EMPTY_ERROR)
        return err;

    err = NEW_SUCCESS();
    while (!is_failure(err))
    {
        err = sl_pop_areas(&buf, str, areas);
        ifok(err)
            err = sl_push_areas(sl, str, areas);
    }

    if (err.code != SL_EMPTY_ERROR)
        return err;

    return NEW_SUCCESS();
}

error_t main_menu_dialog(int *answer) {
    if (!answer)
        return NEW_ERROR("передан нулевой указатель", ENG_ARGS_ERROR);

    printf("+------------------------------- Меню -------------------------------+\n");
    printf("| 01 - добавление элемента в стек-массив;                            |\n");
    printf("| 02 - добавление элемента в стек-список;                            |\n");
    printf("| 03 - удаление элемента из стека-массива;                           |\n");
    printf("| 04 - удаление элемента из стека-списка;                            |\n");
    printf("| 05 - вывод перевернутых строк в обратном порядке из стека-массива; |\n");
    printf("| 06 - вывод перевернутых строк в обратном порядке из стека-списка;  |\n");
    printf("| 07 - вывод состояния стека-массива;                                |\n");
    printf("| 08 - вывод состояния стека-списка;                                 |\n");
    printf("| 09 - вывод списка свободных областей для стека-списка;             |\n");
    printf("| 10 - сравнение скорости работы стеков двух типов;                  |\n");
    printf("| 11 - сравнение объема занимаемой памяти стеков двух типов;         |\n");
    printf("| 00 - выход.                                                        |\n");
    printf("+--------------------------------------------------------------------+\n");

    error_t err;
    printf("Выберите пункт меню [0-11]: ");
    if (is_failure(err = read_int(answer)))
        return err;

    if (*answer > 11 || *answer < 0)
        return NEW_ERROR("введен неверный пункт меню", ENG_INPUT_ERROR);

    return NEW_SUCCESS();
}

error_t eng_work()
{
    printf("+----------------------------------------------------------------+\n");
    printf("| Программа для работы со стеками                                |\n");
    printf("| Программа осуществяет добавление и удаления строк в            |\n");
    printf("| стек-массив и стек-список, вывод состояний стеков, вывод       |\n");
    printf("| перевернутых строк в обратном порядке.                         |\n");
    printf("|                                                                |\n");
    printf("| Кроме того, производится сравнение производительности двух     |\n");
    printf("| методов хранения стеков, а также есть возможность просмотра    |\n");
    printf("| списка свободных областей для стека-списка.                    |\n");
    printf("+----------------------------------------------------------------+\n\n");
    int variant = -1;
    error_t err;
    stack_array_t sa = sa_create();
    stack_list_t *sl = NULL;
    mystring_t str;
    ptr_list_t *free_areas = NULL;

    while (variant)
    {
        err = NEW_SUCCESS();
        err = main_menu_dialog(&variant);
        ifnok(err)
        {
            PRINT_ERROR(err);
            continue;
        }

        if (variant == 1)
        {
            printf("Введите строку длиной не более 256 символов: ");
            err = f_read_line(stdin, str);
            ifok(err)
                err = sa_push(&sa, str);
            ifok(err)
                printf("Строка успешно добавлена.\n");
        }
        else if (variant == 2)
        {
            printf("Введите строку длиной не более 256 символов: ");
            err = f_read_line(stdin, str);
            ifok(err)
                err = sl_push_areas(&sl, str, &free_areas);
            ifok(err)
                printf("Строка успешно добавлена.\n");
        }
        else if (variant == 3)
        {
            err = sa_pop(&sa, str);
            ifok(err)
                printf("Удаленная строка: \"%s\"\n", str);
        }
        else if (variant == 4)
        {
            err = sl_pop_areas(&sl, str, &free_areas);
            ifok(err)
                printf("Удаленная строка: \"%s\"\n", str);
        }
        else if (variant == 5)
        {
            err = sa_print_reversed(&sa);
        }
        else if (variant == 6)
        {
            err = sl_print_reversed(&sl, &free_areas);
        }
        else if (variant == 7)
        {
            printf("Состояние стека-массива:\n");
            err = sa_print_state(&sa);
        }
        else if (variant == 8)
        {
            printf("Состояние стека-списка:\n");
            err = sl_print_state(&sl, &free_areas);
        }
        else if (variant == 9)
        {
            printf("Список свободных областей для стека-списка:\n");
            pl_print(&free_areas);
        }
        else if (variant == 10)
        {
            int n;
            printf("Введите количество добавляемых/удаляемых элементов: ");
            err = read_int(&n);
            ifok(err)
            {
                if (n < 0)
                    err = NEW_ERROR("неверное количество элементов стека", ENG_INPUT_ERROR);
                if (n > SA_STACK_SIZE)
                    err = NEW_ERROR("слишком большой размер", SA_OVERFLOW_ERROR);
            }
            long long time_arr_push, time_arr_pop;
            long long time_list_push, time_list_pop;

            ifok(err)
                err = bench_stack_arr_push(n, &time_arr_push);

            ifok(err)
                err = bench_stack_arr_pop(n, &time_arr_pop);

            ifok(err)
                err = bench_stack_list_push(n, &time_list_push);

            ifok(err)
                err = bench_stack_list_pop(n, &time_list_pop);

            ifok(err)
            {
                printf("Для %d запусков и %d элементов.\n", BENCH_NUM_OF_RUNS, n);
                printf("Стек-массив\n");
                printf("  PUSH: %lld us\n", time_arr_push);
                printf("  POP: %lld us\n", time_arr_pop);

                printf("Стек-список\n");
                printf("  PUSH: %lld us\n", time_list_push);
                printf("  POP: %lld us\n", time_list_pop);

                printf("\nПреимущество стека-массива над стеком-списком\n");
                printf("  PUSH: %.0lf %%\n", ((double) time_list_push / (double) time_arr_push - 1) * 100);
                printf("  POP: %.0lf %%\n", ((double) time_list_pop / (double) time_arr_pop - 1) * 100);
            }
        }
        else if (variant == 11)
        {
            int n;
            printf("Введите количество элементов: ");
            err = read_int(&n);
            ifok(err)
            {
                if (n < 0)
                    err = NEW_ERROR("неверное количество элементов стека", ENG_INPUT_ERROR);
                if (n > SA_STACK_SIZE)
                    err = NEW_ERROR("слишком большой размер", SA_OVERFLOW_ERROR);
            }

            ifok(err)
            {
                int size_arr = (int) sizeof(stack_array_t);
                int size_list = n * (int) sizeof(stack_list_t);
                printf("Для %d элементов.\n", n);
                printf("Стек-массив\n");
                printf("  Объем занимаемой памяти: %d B\n", size_arr);
                printf("\nСтек-список\n");
                printf("  Объем занимаемой памяти: %d B\n", size_list);

                printf("\nПреимущество стека-списка над стеком-массивом\n");
                printf("  Объем памяти: %.0lf %%\n", ((double) size_arr / (double) size_list - 1) * 100);
            }
        }

        ifnok(err)
            PRINT_ERROR(err);
    }

    pl_free(&free_areas);
    sl_free(&sl);
    return err;
}
