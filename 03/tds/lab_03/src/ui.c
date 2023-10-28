#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "usual_matrix.h"
#include "csc_matrix.h"
#include "vector.h"
#include "error.h"
#include "ui.h"
#include "mystring.h"
#include "benchmark.h"

#define PRINT_ERROR(err) printf("%d: %s (%s)\n", err.code, err.text, err.func);
#define ifok(err) if (!is_failure(err))
#define ifnok(err) if (is_failure(err))

error_t get_number(int *res)
{
    if (!res)
        return NEW_ERROR("передан нулевой указатель", UI_ARGS_ERROR);

    error_t err;
    mystring_t str;
    if (is_failure(err = f_read_line(stdin, str)))
        return err;

    return to_integer(str, res);
}

error_t main_menu_dialog(int *answer)
{
    if (!answer)
        return NEW_ERROR("передан нулевой указатель", UI_ARGS_ERROR);

    printf("+----------------------------- Меню -----------------------------+\n");
    printf("| 1 - ввод матрицы;                                              |\n");
    printf("| 2 - вывод матрицы;                                             |\n");
    printf("| 3 - ввод вектора-столбца;                                      |\n");
    printf("| 4 - вывод вектора-столбца;                                     |\n");
    printf("| 5 - умножение матрицы на вектор стандартным методом;           |\n");
    printf("| 6 - умножение матрицы на вектор методом разреженной матрицы;   |\n");
    printf("| 7 - сравнение эффективности по времени методов умножения;      |\n");
    printf("| 8 - сравнение потребления памяти хранения матрицы в            |\n");
    printf("|     стандартной форме и в форме разреженной матрицы;           |\n");
    printf("| 0 - выход.                                                     |\n");
    printf("+----------------------------------------------------------------+\n");

    error_t err;
    printf("Выберите пункт меню [0-8]: ");
    if (is_failure(err = get_number(answer)))
        return err;

    if (*answer > 8 || *answer < 0)
        return NEW_ERROR("введен неверный пункт меню", UI_INPUT_ERROR);

    return NEW_SUCCESS();
}

error_t input_source_dialog(int *answer)
{
    if (!answer)
        return NEW_ERROR("передан нулевой указатель", UI_ARGS_ERROR);

    printf("+----- Выберите источник ввода -----+\n");
    printf("| 1 - ввод из файла;                |\n");
    printf("| 2 - ввод с клавиатуры;            |\n");
    printf("| 3 - случайное заполнение.         |\n");
    printf("+-----------------------------------+\n");

    error_t err;
    printf("Выберите подходящий вариант [1-3]: ");
    if (is_failure(err = get_number(answer)))
        return err;

    if (*answer > 3 || *answer < 1)
        return NEW_ERROR("введен неверный вариант", UI_INPUT_ERROR);

    return NEW_SUCCESS();
}

error_t input_type_dialog(int *answer)
{
    if (!answer)
        return NEW_ERROR("передан нулевой указатель", UI_ARGS_ERROR);

    printf("+------- Выберите тип ввода -------+\n");
    printf("| 1 - обычный ввод;                |\n");
    printf("| 2 - координатный ввод.           |\n");
    printf("+----------------------------------+\n");

    printf("Выберите подходящий вариант [1-2]: ");
    error_t err;
    if (is_failure(err = get_number(answer)))
        return err;

    if (*answer > 2 || *answer < 1)
        return NEW_ERROR("введен неверный вариант", UI_INPUT_ERROR);

    return NEW_SUCCESS();
}

error_t output_matrix_dialog(int *answer)
{
    if (!answer)
        return NEW_ERROR("передан нулевой указатель", UI_ARGS_ERROR);

    printf("+--- Выберите, в каком формате выводить матрицу ---+\n");
    printf("| 1 - обычный формат;                              |\n");
    printf("| 2 - разреженный формат.                          |\n");
    printf("+--------------------------------------------------+\n");

    printf("Выберите подходящий вариант [1-2]: ");
    error_t err;
    if (is_failure(err = get_number(answer)))
        return err;

    if (*answer > 2 || *answer < 1)
        return NEW_ERROR("введен неверный вариант", UI_INPUT_ERROR);

    return NEW_SUCCESS();
}

error_t input_filename_dialog(mystring_t name)
{
    if (!name)
        return NEW_ERROR("передан нулевой указатель", UI_ARGS_ERROR);

    printf("Введите имя файла: ");
    return f_read_line(stdin, name);
}

error_t input_size_dialog(size_t *rows, size_t *cols, FILE *f)
{
    if (!rows || !cols || !f)
        return NEW_ERROR("передан нулевой указатель", UI_ARGS_ERROR);

    if (f == stdin)
        printf("Введите размер матрицы в формате <ROWS>x<COLS>: ");
    if (fscanf(f, "%zux%zu", rows, cols) != 2) {
        fflush(stdin);
        return NEW_ERROR("ошибка чтения размеров матрицы", UM_READ_ERROR);
    }
    fflush(stdin);

    return NEW_SUCCESS();
}

error_t input_len_dialog(size_t *len, FILE *f)
{
    if (!len || !f)
        return NEW_ERROR("передан нулевой указатель", UI_ARGS_ERROR);

    if (f == stdin)
        printf("Введите длину вектора: ");
    if (fscanf(f, "%zu", len) != 1)
    {
        fflush(stdin);
        return NEW_ERROR("ошибка чтения длины вектора", UM_READ_ERROR);
    }
    fflush(stdin);

    return NEW_SUCCESS();
}

error_t input_coordinate_not_zero_dialog(size_t *not_zero, FILE *f)
{
    if (!not_zero || !f)
        return NEW_ERROR("передан нулевой указатель", UI_ARGS_ERROR);

    if (f == stdin)
        printf("Количество ненулевых элементов матрицы: ");
    if (fscanf(f, "%zu", not_zero) != 1)
    {
        fflush(stdin);
        return NEW_ERROR("ошибка чтения количества ненулевых элементов", UM_READ_ERROR);
    }
    fflush(stdin);

    return NEW_SUCCESS();
}

error_t input_non_zero_percent_dialog(int *percent)
{
    if (!percent)
        return NEW_ERROR("передан нулевой указатель", UI_ARGS_ERROR);

    printf("Процент ненулевых элементов матрицы: ");
    if (scanf("%d", percent) != 1)
    {
        fflush(stdin);
        return NEW_ERROR("ошибка чтения процента ненулевых элементов", UI_INPUT_ERROR);
    }
    fflush(stdin);

    if (*percent < 0 || *percent > 100)
        return NEW_ERROR("неверный процент ненулевых элементов", UI_INPUT_ERROR);

    return NEW_SUCCESS();
}

error_t open_file(char *filename, char *mode, FILE **f)
{
    if (!filename || !mode || !f)
        return NEW_ERROR("передан нулевой указатель", UI_ARGS_ERROR);

    *f = fopen(filename, mode);
    if (!*f)
        return NEW_ERROR("ошибка открытия файла", UI_FILE_OPEN_ERROR);

    return NEW_SUCCESS();
}

error_t input_matrix_dialog(um_matrix_t *m, FILE *f)
{
    if (!um_initialized(m) || !f)
        return NEW_ERROR("неверные аргументы", UM_ARGS_ERROR);

    if (f == stdin)
        printf("Введите элементы матрицы:\n");
    return um_read(m, f);
}

error_t input_vector_dialog(v_vector_t *v, FILE *f)
{
    if (!v_initialized(v) || !f)
        return NEW_ERROR("неверные аргументы", UM_ARGS_ERROR);

    if (f == stdin)
        printf("Введите элементы вектора:\n");
    return v_read(v, f);
}

error_t coordinate_read_matrix_dialog(um_matrix_t *m, FILE *f)
{
    if (!um_initialized(m) || !f)
        return NEW_ERROR("неверные аргументы", UM_ARGS_ERROR);

    size_t not_zero;
    error_t err;
    err = input_coordinate_not_zero_dialog(&not_zero, f);
    ifnok(err)
        return err;

    if (f == stdin)
        printf("Введите элементы матрицы в формате <ROW> <COL> <VALUE>: \n");

    return um_read_coordinates(m, f, not_zero);
}

error_t matrix_fill_dialog(um_matrix_t *m)
{
    if (!um_initialized(m))
        return NEW_ERROR("неверные аргументы", UI_ARGS_ERROR);

    int percent;
    error_t err;
    err = input_non_zero_percent_dialog(&percent);
    ifok(err)
        err = um_generate(m, percent);

    return err;
}

error_t vector_fill_dialog(v_vector_t *v)
{
    if (!v_initialized(v))
        return NEW_ERROR("неверные аргументы", UI_ARGS_ERROR);

    return v_generate(v);
}

error_t input_matrix_scenario(um_matrix_t *m, csc_matrix_t *cm)
{
    if (um_allocated(m) || csc_allocated(cm))
        return NEW_ERROR("матрицы не должны быть выделены", UI_ARGS_ERROR);

    int source_num;
    error_t err;
    err = input_source_dialog(&source_num);

    FILE *source = NULL;
    if (source_num == 1)
    {
        mystring_t name;
        ifok (err)
            err = input_filename_dialog(name);
        ifok (err)
            err = open_file(name, "r", &source);
    }
    else if (source_num == 2)
        source = stdin;
    else
    {
        size_t rows, cols;
        ifok(err)
            err = input_size_dialog(&rows, &cols, stdin);

        ifok(err)
            err = um_init(m, rows, cols);

        ifok(err)
            err = matrix_fill_dialog(m);

        ifok(err)
            err = csc_from_um(cm, m);

        ifok(err)
            printf("Матрица %zux%zu заполнена успешно.\n", m->rows, m->cols);

        return err;
    }

    int type;
    ifok(err)
        err = input_type_dialog(&type);

    size_t rows, cols;
    ifok(err)
        err = input_size_dialog(&rows, &cols, source);

    ifok(err)
        err = um_init(m, rows, cols);

    ifok(err)
    {
        if (type == 1)
            err = input_matrix_dialog(m, source);
        else
            err = coordinate_read_matrix_dialog(m, source);
    }
    fflush(source);

    if (source_num == 1 && source)
        fclose(source);

    ifok(err)
        err = csc_from_um(cm, m);

    ifok(err)
        printf("Матрица %zux%zu введена успешно.\n", m->rows, m->cols);

    return err;
}

error_t output_matrix_scenario(um_matrix_t *m, csc_matrix_t *cm)
{
    error_t err;
    int type;
    err = output_matrix_dialog(&type);

    ifok(err)
    {
        if (type == 1)
            err = um_write(m, stdout);
        else
            err = csc_write(cm, stdout);
    }

    return err;
}

error_t input_vector_scenario(v_vector_t *v)
{
    if (!v || v_allocated(v))
        return NEW_ERROR("неверный вектор", UI_ARGS_ERROR);

    int source_num;
    error_t err;
    err = input_source_dialog(&source_num);

    FILE *source = NULL;
    if (source_num == 1)
    {
        mystring_t name;
        ifok (err)
            err = input_filename_dialog(name);
        ifok (err)
            err = open_file(name, "r", &source);
    }
    else if (source_num == 2)
        source = stdin;
    else
    {
        size_t len;
        ifok(err)
            err = input_len_dialog(&len, stdin);

        ifok(err)
            err = v_init(v, len);

        ifok(err)
            err = vector_fill_dialog(v);

        ifok(err)
            printf("Вектор заполнен успешно.\n");

        return err;
    }

    size_t len;
    ifok(err)
        err = input_len_dialog(&len, source);

    ifok(err)
        err = v_init(v, len);

    ifok(err)
        err = input_vector_dialog(v, source);
    fflush(source);

    if (source_num == 1 && source)
        fclose(source);

    ifok(err)
        printf("Вектор введен успешно.\n");

    return err;
}

error_t output_vector_scenario(v_vector_t *v)
{
    if (!v_initialized(v))
        return NEW_ERROR("вектор не инициализирован", UI_ARGS_ERROR);

    return v_write(v, stdout);
}

error_t standard_mul_scenario(um_matrix_t *m, v_vector_t *v)
{
    um_matrix_t result = um_um();
    error_t err = um_multiply_with_v(&result, m, v);
    ifok(err)
        um_write(&result, stdout);
    um_free(&result);
    return err;
}

error_t sparse_mul_scenario(csc_matrix_t *cm, v_vector_t *v)
{
    csc_matrix_t result = csc_csc();
    error_t err = csc_multiply_with_v(&result, cm, v);
    ifok(err)
        csc_write(&result, stdout);
    csc_free(&result);
    return err;
}

error_t memory_bench_scenario(um_matrix_t *m, csc_matrix_t *cm)
{
    size_t um_size;
    size_t csc_size;
    error_t err;
    if (is_failure(err = bench_um_memory_usage(m, &um_size)))
        return err;

    if (is_failure(err = bench_csc_memory_usage(cm, &csc_size)))
        return err;

    printf("Размер матрицы в обычном виде: %zu Б\n", um_size);
    printf("Размер матрицы в разреженном виде: %zu Б\n", csc_size);
    printf("Размер матрицы в разреженном виде меньше на %.0lf %%\n", ((double) um_size / (double) csc_size - 1) * 100);
    return NEW_SUCCESS();
}

error_t time_bench_scenario(um_matrix_t *m, csc_matrix_t *cm, v_vector_t *v)
{
    long long sum_um_time;
    long long sum_csc_time;
    error_t err;
    if (is_failure(err = bench_um_mul(m, v, &sum_um_time)))
        return err;

    if (is_failure(err = bench_csc_mul(cm, v, &sum_csc_time)))
        return err;

    printf("Суммарная статистика по времени для матрицы %zux%zu для %d запусков:\n", m->rows, m->cols, BENCH_NUM_OF_RUNS);
    printf("Умножение обычной матрицы: %lld us\n", sum_um_time);
    printf("Умножение разреженной матрицы: %lld us\n", sum_csc_time);
    printf("Умножение разреженной матрицы быстрее стандартного на %.0lf %%\n", ((double) sum_um_time / (double) sum_csc_time - 1) * 100);

    return NEW_SUCCESS();
}

error_t main_scenario()
{
    printf("+----------------------------------------------------------------+\n");
    printf("| Программа для работы с матрицами                               |\n");
    printf("| Программа осуществляет ввод-вывод матриц в обычном и           |\n");
    printf("| разреженных видах, координатный ввод матриц, случайное         |\n");
    printf("| заполнение матрицы и вектора.                                  |\n");
    printf("|                                                                |\n");
    printf("| Реализовано умножение матриц стандартным алгоритмом и          |\n");
    printf("| алгоритмом разреженной матрицы, а также произведено сравнение  |\n");
    printf("| их производительности по скорости и занимаемой памяти.         |\n");
    printf("+----------------------------------------------------------------+\n\n");
    int variant = -1;
    error_t err;
    um_matrix_t matrix = um_um();
    csc_matrix_t cmatrix = csc_csc();
    v_vector_t vector = v_v();

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
            um_free(&matrix);
            csc_free(&cmatrix);

            ifok(err)
                err = input_matrix_scenario(&matrix, &cmatrix);

            ifnok(err)
            {
                um_free(&matrix);
                csc_free(&cmatrix);
            }
        }
        else if (variant == 2)
        {
            ifok(err)
                err = output_matrix_scenario(&matrix, &cmatrix);
        }
        else if (variant == 3)
        {
            v_free(&vector);

            ifok(err)
                err = input_vector_scenario(&vector);
        }
        else if (variant == 4)
        {
            ifok(err)
                err = output_vector_scenario(&vector);
        }
        else if (variant == 5)
        {
            ifok(err)
                err = standard_mul_scenario(&matrix, &vector);
        }
        else if (variant == 6)
        {
            ifok(err)
                err = sparse_mul_scenario(&cmatrix, &vector);
        }
        else if (variant == 7)
        {
            ifok(err)
                err = time_bench_scenario(&matrix, &cmatrix, &vector);
        }
        else if (variant == 8)
        {
            ifok(err)
                err = memory_bench_scenario(&matrix, &cmatrix);
        }

        ifnok(err)
            PRINT_ERROR(err);
    }

    return err;
}
