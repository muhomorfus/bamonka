#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "product.h"
#include "node.h"
#include "error.h"
#include "my_string.h"

int pd_input(FILE *f, product_t *product)
{
    if (!f)
        return PD_INPUT_ERROR;
    if (!product)
        return PD_ARGS_ERROR;

    int err;
    product->name = ms_new();
    if ((err = ms_read_line(f, product->name)))
        return err;

    my_string_t *buffer = ms_new();
    if ((err = ms_read_line(f, buffer)))
    {
        ms_free(buffer);
        return err;
    }

    if ((err = to_integer(buffer->ptr, &product->price)))
    {
        ms_free(buffer);
        return err;
    }

    ms_free(buffer);
    if (product->price <= 0)
        return PD_INVALID_PRICE_ERROR;

    return EXIT_SUCCESS;
}

int pd_print(FILE *f, product_t *product)
{
    if (!f)
        return PD_INPUT_ERROR;
    if (!product)
        return PD_ARGS_ERROR;

    if (fprintf(f, "%s\n", product->name->ptr) < 0)
        return PD_PRINT_ERROR;
    if (fprintf(f, "%d", product->price) < 0)
        return PD_PRINT_ERROR;

    return EXIT_SUCCESS;
}

void pd_free_content(product_t *product)
{
    if (product)
    {
        ms_free(product->name);
        product->name = NULL;
    }
}

void pds_free(product_t *products, size_t len)
{
    for (size_t i = 0; i < len; i++)
        pd_free_content(&products[i]);
    free(products);
}

product_t *pds_new(size_t len)
{
    return calloc(len, sizeof(product_t));
}

product_t *pds_from_file(FILE *f, size_t *len)
{
    if (!f)
        return NULL;

    my_string_t *buffer = ms_new();
    if (ms_read_line(f, buffer))
    {
        ms_free(buffer);
        return NULL;
    }
    int n;
    if (to_integer(buffer->ptr, &n))
    {
        ms_free(buffer);
        return NULL;
    }
    ms_free(buffer);

    if (n <= 0)
        return NULL;
    *len = (size_t) n;

    product_t *products = pds_new(*len);
    if (!products)
    {
        fclose(f);
        return NULL;
    }

    for (size_t i = 0; i < *len; i++)
        if (pd_input(f, &products[i]))
        {
            pds_free(products, *len);
            return NULL;
        }

    return products;
}

node_t *pds_list_from_arr(product_t *products, size_t len)
{
    if (!products || !len)
        return NULL;

    node_t *head = calloc(1, sizeof(node_t));
    if (!head)
        return NULL;

    head->data = products;
    node_t *last = head;
    for (size_t i = 1; i < len; i++)
    {
        last->next = calloc(1, sizeof(node_t));
        if (!last->next)
        {
            nodes_free(&head);
            return NULL;
        }
        last->next->data = products + i;
        last = last->next;
    }

    return head;
}

int pd_compare_by_name_then_price(const void *a, const void *b)
{
    const product_t *pa = a;
    const product_t *pb = b;
    int cmp = strcmp(pa->name->ptr, pb->name->ptr);
    if (!cmp)
        cmp = pa->price - pb->price;

    return cmp;
}

int pd_compare_by_name(const void *a, const void *b)
{
    const product_t *pa = a;
    const product_t *pb = b;
    int cmp = strcmp(pa->name->ptr, pb->name->ptr);

    return cmp;
}

int pd_compare_by_price_then_name(const void *a, const void *b)
{
    const product_t *pa = a;
    const product_t *pb = b;
    int cmp = pa->price - pb->price;
    if (!cmp)
        cmp = strcmp(pa->name->ptr, pb->name->ptr);

    return cmp;
}
