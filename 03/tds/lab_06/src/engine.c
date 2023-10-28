#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "error.h"
#include "engine.h"
#include "mystring.h"
#include "benchmark.h"
#include "bst.h"
#include "avl.h"
#include "ht.h"

#define PRINT_ERROR(err) printf("Ошибка %d: %s.\n", err.code, err.text);
#define ifok(err) if (!is_failure(err))
#define ifnok(err) if (is_failure(err))

error_t main_menu_dialog(int *answer) {
    if (!answer)
        return NEW_ERROR("передан нулевой указатель", ENG_ARGS_ERROR);

    printf("\n+------------------------------- Меню -------------------------------+\n");
    printf("| 01 - вывод ДДП;                                                    |\n");
    printf("| 02 - поиск элемента в ДДП;                                         |\n");
    printf("| 03 - удаление элемента из ДДП;                                     |\n");
    printf("| 04 - вывод АВЛ-дерева;                                             |\n");
    printf("| 05 - поиск элемента в АВЛ-дереве;                                  |\n");
    printf("| 06 - удаление элемента из АВЛ-дерева;                              |\n");
    printf("| 07 - вывод хеш-таблицы;                                            |\n");
    printf("| 08 - поиск элемента в хеш-таблице;                                 |\n");
    printf("| 09 - удаление элемента из хеш-таблицы;                             |\n");
    printf("| 10 - сравнение скорости;                                           |\n");
    printf("| 11 - сравнение потребления памяти;                                 |\n");
    printf("| 00 - выход.                                                        |\n");
    printf("+--------------------------------------------------------------------+\n");

    error_t err;
    printf("Выберите пункт меню [0-11]: ");
    if (is_failure(err = ms_read_int_line(stdin, answer)))
        return err;

    if (*answer > 11 || *answer < 0)
        return NEW_ERROR("введен неверный пункт меню", ENG_INPUT_ERROR);

    return NEW_SUCCESS();
}

error_t input_word_dialog(mystring_t str, char *message)
{
    if (!str)
        return NEW_ERROR("передан нулевой указатель", ENG_ARGS_ERROR);

    printf("%s", message);
    return ms_read_word(stdin, str);
}

error_t input_int_line_dialog(int *n, char *message)
{
    if (!n)
        return NEW_ERROR("передан нулевой указатель", ENG_ARGS_ERROR);

    printf("%s", message);
    return ms_read_int_line(stdin, n);
}

error_t eng_work()
{
    printf("+----------------------------------------------------------------+\n");
    printf("| Программа для работы с двоичными деревьями поиска,             |\n");
    printf("| АВЛ-деревьями и хеш-таблицами.                                 |\n");
    printf("|                                                                |\n");
    printf("| Программа позволяет осуществлять операции удаления и поиска    |\n");
    printf("| в заданных структурах. Ввод в структуры может быть             |\n");
    printf("| осуществлен из файла.                                          |\n");
    printf("|                                                                |\n");
    printf("| Кроме того, производится сравнение производительности двух     |\n");
    printf("| методов хранения деревьев и хеш-таблицы.                       |\n");
    printf("+----------------------------------------------------------------+\n");
    int variant = -1;
    bst_t *bst = NULL;
    avl_t *avl = NULL;
    ht_t *ht = NULL;
    error_t err = NEW_SUCCESS();

    mystring_t filename;
    if (is_failure(err = input_word_dialog(filename, "Введите название файла со словами: ")))
        return err;

    int ht_size_int;
    if (is_failure(err = input_int_line_dialog(&ht_size_int, "Введите размер хеш-таблицы: ")))
        return err;

    if (ht_size_int <= 0)
        return NEW_ERROR("размер хеш-табицы не может быть меньше нуля", ENG_INPUT_ERROR);

    size_t ht_size = (size_t) ht_size_int;

    FILE *f = NULL;
    if (!(f = fopen(filename, "r")))
        return NEW_ERROR("ошибка открытия файла", ENG_FILE_OPEN_ERROR);

    ifok(err)
        err = bst_from_file(&bst, f);

    ifok(err)
        err = avl_from_file(&avl, f);

    ifok(err)
    {
        ht = ht_new(ht_size, ht_additive_method);
        if (!ht)
            err = NEW_ERROR("ошибка создания хеш-таблицы", ENG_ALLOC_ERROR);
    }

    ifok(err)
        err = ht_from_file(&ht, f);

    fclose(f);

    ifok(err)
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
                ifok(err)
                    err = bst_show(&bst);
            }
            else if (variant == 2)
            {
                mystring_t word;
                ifok(err)
                    err = input_word_dialog(word, "Введите искомое слово: ");

                bst_t *found = NULL;
                long long int timer;
                size_t n_cmp = 0;
                ifok(err)
                    BENCH_CHECK_TIME(err = bst_search(&bst, word, &found, &n_cmp), timer);

                ifok(err)
                    printf("Слово \"%s\" найдено в ДДП.\n", found->key);

                if (err.code == BST_NOT_FOUND_ERROR)
                {
                    printf("Элемент не найден.\n");
                    err = NEW_SUCCESS();
                }

                ifok (err)
                    printf("Время поиска: %lld.\n", timer);

                ifok(err)
                    printf("Количество сравнений: %zu.\n", n_cmp);
            }
            else if (variant == 3)
            {
                mystring_t word;
                ifok(err)
                    err = input_word_dialog(word, "Введите удаляемое слово: ");

                long long int timer;
                size_t n_cmp = 0;
                ifok(err)
                    BENCH_CHECK_TIME(err = bst_remove(&bst, word, &n_cmp), timer);

                ifok(err)
                    printf("Слово \"%s\" успешно удалено из ДДП.\n", word);

                if (err.code == BST_NOT_FOUND_ERROR)
                {
                    printf("Элемент не найден.\n");
                    err = NEW_SUCCESS();
                }

                ifok (err)
                    printf("Время удаления: %lld.\n", timer);

                ifok(err)
                    printf("Количество сравнений: %zu.\n", n_cmp);
            }
            else if (variant == 4)
            {
                ifok(err)
                    err = avl_show(&avl);
            }
            else if (variant == 5)
            {
                mystring_t word;
                ifok(err)
                    err = input_word_dialog(word, "Введите искомое слово: ");

                long long int timer;
                avl_t *found = NULL;
                size_t n_cmp = 0;
                ifok(err)
                    BENCH_CHECK_TIME(err = avl_search(&avl, word, &found, &n_cmp), timer);

                ifok(err)
                    printf("Слово \"%s\" найдено в АВЛ-дереве.\n", found->key);

                if (err.code == AVL_NOT_FOUND_ERROR)
                {
                    printf("Элемент не найден.\n");
                    err = NEW_SUCCESS();
                }

                ifok (err)
                    printf("Время поиска: %lld.\n", timer);

                ifok(err)
                    printf("Количество сравнений: %zu.\n", n_cmp);
            }
            else if (variant == 6)
            {
                mystring_t word;
                ifok(err)
                    err = input_word_dialog(word, "Введите удаляемое слово: ");

                long long int timer;
                size_t n_cmp = 0;
                ifok(err)
                    BENCH_CHECK_TIME(err = avl_remove(&avl, word, &n_cmp), timer);

                ifok(err)
                    printf("Слово \"%s\" успешно удалено из АВЛ-дерева.\n", word);

                if (err.code == AVL_NOT_FOUND_ERROR)
                {
                    printf("Элемент не найден.\n");
                    err = NEW_SUCCESS();
                }

                ifok (err)
                    printf("Время удаления: %lld.\n", timer);

                ifok(err)
                    printf("Количество сравнений: %zu.\n", n_cmp);

            }
            else if (variant == 7)
            {
                ifok(err)
                    err = ht_print(&ht);
            }
            else if (variant == 8)
            {
                mystring_t word;
                ifok(err)
                    err = input_word_dialog(word, "Введите искомое слово: ");

                long long int timer;
                ht_element_t *found = NULL;
                size_t n_cmp = 0;
                ifok(err)
                    BENCH_CHECK_TIME(err = ht_search(&ht, word, &found, &n_cmp), timer);

                ifok(err)
                    printf("Слово \"%s\" найдено в хеш-таблице.\n", found->key);

                if (err.code == HT_NOT_FOUND_ERROR)
                {
                    printf("Элемент не найден.\n");
                    err = NEW_SUCCESS();
                }

                ifok (err)
                    printf("Время поиска: %lld.\n", timer);

                ifok(err)
                    printf("Количество сравнений: %zu.\n", n_cmp);
            }
            else if (variant == 9)
            {
                mystring_t word;
                ifok(err)
                    err = input_word_dialog(word, "Введите удаляемое слово: ");

                long long int timer;
                size_t n_cmp = 0;
                ifok(err)
                    BENCH_CHECK_TIME(err = ht_remove(&ht, word, &n_cmp), timer);

                ifok(err)
                    printf("Слово \"%s\" успешно удалено из хеш-таблицы.\n", word);

                if (err.code == HT_NOT_FOUND_ERROR)
                {
                    printf("Элемент не найден.\n");
                    err = NEW_SUCCESS();
                }

                ifok (err)
                    printf("Время удаления: %lld.\n", timer);

                ifok(err)
                    printf("Количество сравнений: %zu.\n", n_cmp);
            }
            else if (variant == 10)
            {
                int num_of_els_int;
                ifok (err)
                    err = input_int_line_dialog(&num_of_els_int, "Введите количество элементов: ");

                size_t num_of_els;
                ifok(err)
                {
                    if (num_of_els_int <= 0)
                        err = NEW_ERROR("размер не может быть меньше нуля", ENG_INPUT_ERROR);
                    num_of_els = (size_t) num_of_els_int;
                }

                int ht_test_size_int;
                ifok (err)
                    err = input_int_line_dialog(&ht_test_size_int, "Введите размер хеш-таблицы: ");

                size_t ht_test_size;
                ifok(err)
                {
                    if (ht_test_size_int <= 0)
                        err = NEW_ERROR("размер хеш-табицы не может быть меньше нуля", ENG_INPUT_ERROR);
                    ht_test_size = (size_t) ht_test_size_int;
                }

                long long bst_remove_time, bst_search_time;
                long long avl_remove_time, avl_search_time;
                long long ht_remove_time, ht_search_time;
                long long file_remove_time, file_search_time;

                size_t bst_remove_cmp = 0, bst_search_cmp = 0;
                size_t avl_remove_cmp = 0, avl_search_cmp = 0;
                size_t ht_remove_cmp = 0, ht_search_cmp = 0;
                size_t file_remove_cmp = 0, file_search_cmp = 0;

                ifok(err)
                    err = bench_bst_remove(num_of_els, &bst_remove_time, &bst_remove_cmp);

                ifok(err)
                    err = bench_bst_search(num_of_els, &bst_search_time, &bst_search_cmp);

                ifok(err)
                    err = bench_avl_remove(num_of_els, &avl_remove_time, &avl_remove_cmp);

                ifok(err)
                    err = bench_avl_search(num_of_els, &avl_search_time, &avl_search_cmp);

                ifok(err)
                    err = bench_ht_remove(num_of_els, ht_test_size, &ht_remove_time, &ht_remove_cmp);

                ifok(err)
                    err = bench_ht_search(num_of_els, ht_test_size, &ht_search_time, &ht_search_cmp);

                ifok(err)
                    err = bench_file_remove(num_of_els, &file_remove_time, &file_remove_cmp);

                ifok(err)
                    err = bench_file_search(num_of_els, &file_search_time, &file_search_cmp);

                ifok(err)
                {
                    printf("ДДП:\n");
                    printf("  Поиск: %lld, количество сравнений: %zu\n", bst_search_time, bst_search_cmp);
                    printf("  Удаление: %lld, количество сравнений: %zu\n", bst_remove_time, bst_remove_cmp);

                    printf("АВЛ-дерево:\n");
                    printf("  Поиск: %lld, количество сравнений: %zu\n", avl_search_time, avl_search_cmp);
                    printf("  Удаление: %lld, количество сравнений: %zu\n", avl_remove_time, avl_remove_cmp);

                    printf("Хеш-таблица:\n");
                    printf("  Поиск: %lld, количество сравнений: %zu\n", ht_search_time, ht_search_cmp);
                    printf("  Удаление: %lld, количество сравнений: %zu\n", ht_remove_time, ht_remove_cmp);

                    printf("Файл:\n");
                    printf("  Поиск: %lld, количество сравнений: %zu\n", file_search_time, file_search_cmp);
                    printf("  Удаление: %lld, количество сравнений: %zu\n", file_remove_time, file_remove_cmp);
                }
            }
            else if (variant == 11)
            {
                int num_of_els_int;
                ifok (err)
                    err = input_int_line_dialog(&num_of_els_int, "Введите количество элементов: ");

                size_t num_of_els;
                ifok(err)
                {
                    if (num_of_els_int <= 0)
                        err = NEW_ERROR("размер не может быть меньше нуля", ENG_INPUT_ERROR);
                    num_of_els = (size_t) num_of_els_int;
                }

                int ht_test_size_int;
                ifok (err)
                    err = input_int_line_dialog(&ht_test_size_int, "Введите размер хеш-таблицы: ");

                size_t ht_test_size;
                ifok(err)
                {
                    if (ht_test_size_int <= 0)
                        err = NEW_ERROR("размер хеш-табицы не может быть меньше нуля", ENG_INPUT_ERROR);
                    ht_test_size = (size_t) ht_test_size_int;
                }

                size_t bst_vol;
                size_t avl_vol;
                size_t ht_vol;
                size_t file_vol;

                ifok(err)
                    err = bench_bst_size(num_of_els, &bst_vol);

                ifok(err)
                    err = bench_avl_size(num_of_els, &avl_vol);

                ifok(err)
                    err = bench_ht_size(num_of_els, ht_test_size, &ht_vol);

                ifok(err)
                    err = bench_file_size(num_of_els, &file_vol);

                ifok(err)
                {
                    printf("ДДП:\n");
                    printf("  Размер: %zu Б\n", bst_vol);

                    printf("АВЛ-дерево:\n");
                    printf("  Размер: %zu Б\n", avl_vol);

                    printf("Хеш-таблица:\n");
                    printf("  Размер: %zu Б\n", ht_vol);

                    printf("Текстовый файл:\n");
                    printf("  Размер: %zu Б\n", file_vol);
                }
            }

            ifnok(err)
                PRINT_ERROR(err);
        }

    bst_free(&bst);
    avl_free(&avl);

    return err;
}
