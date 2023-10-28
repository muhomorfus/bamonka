/**
 * В файле, имя которого вводится в качестве аргумента, хранится
 * список продуктов: название и цена.
 * Удалить товары с повторяющимися именами, оставив самые дешевые варианты,
 * затем удалить самый дорогой и самые дешевые товары.
 * Вывести отсортированный по цене список товаров на экран.
*/

#include <stdio.h>
#include <stdlib.h>

#include "product.h"
#include "node.h"

#define ARGS_ERROR 1
#define OPEN_ERROR 2
#define READ_ERROR 3
#define CONVERSION_ERROR 4
#define SORT_ERROR 5
#define POP_ERROR 6

int main(int argc, char **argv)
{
    if (argc != 2)
        return ARGS_ERROR;

    FILE *f = fopen(argv[1], "r");
    if (!f)
        return OPEN_ERROR;

    size_t n;
    product_t *products_arr = pds_from_file(f, &n);
    fclose(f);
    if (!products_arr)
        return READ_ERROR;

    node_t *list = pds_list_from_arr(products_arr, n);
    if (!list)
    {
        pds_free(products_arr, n);
        return CONVERSION_ERROR;
    }

    node_t *by_name = sort(list, pd_compare_by_name_then_price);
    nodes_free(&list);
    if (!by_name)
    {
        pds_free(products_arr, n);
        return SORT_ERROR;
    }
    list = by_name;

    remove_duplicates(&by_name, pd_compare_by_name);

    node_t *by_price = sort(list, pd_compare_by_price_then_name);
    nodes_free(&list);
    if (!by_price)
    {
        pds_free(products_arr, n);
        return SORT_ERROR;
    }
    list = by_price;

    if (!pop_front(&list) || !pop_back(&list) || !list)
    {
        nodes_free(&list);
        pds_free(products_arr, n);
        return POP_ERROR;
    }

    for (node_t *cur = list; cur; cur = cur->next)
    {
        pd_print(stdout, cur->data);
        printf("\n");
    }

    nodes_free(&list);
    pds_free(products_arr, n);

    return EXIT_SUCCESS;
}
