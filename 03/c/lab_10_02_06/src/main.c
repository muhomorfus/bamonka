#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mnode.h"

#define INPUT_ERROR 1
#define SUM_ERROR 2
#define PARAM_ERROR 3

int main(void)
{
    char action[10];
    if (scanf("%9s", action) != 1)
        return INPUT_ERROR;

    if (!strcmp(action, "out"))
    {
        mnode_t *list = mlist_from_file(stdin);
        if (!list)
            return INPUT_ERROR;

        mlist_to_file(stdout, list);
        mlist_free(&list);
    }
    else if (!strcmp(action, "lin"))
    {
        mnode_t *list = mlist_from_file(stdin);
        if (!list)
            return INPUT_ERROR;

        int err = mlist_del_row_max(&list);
        if (err)
        {
            mlist_free(&list);
            return err;
        }
        mlist_to_file(stdout, list);
        mlist_free(&list);
    }
    else if (!strcmp(action, "add"))
    {
        mnode_t *a = mlist_from_file(stdin);
        if (!a)
            return INPUT_ERROR;

        mnode_t *b = mlist_from_file(stdin);
        if (!b)
        {
            mlist_free(&a);
            return INPUT_ERROR;
        }

        mnode_t *sum = mlist_add(a, b);
        mlist_free(&a);
        mlist_free(&b);
        if (!sum)
            return SUM_ERROR;

        mlist_to_file(stdout, sum);
        mlist_free(&sum);
    }
    else if (!strcmp(action, "mul"))
    {
        mnode_t *a = mlist_from_file(stdin);
        if (!a)
            return INPUT_ERROR;

        mnode_t *b = mlist_from_file(stdin);
        if (!b)
        {
            mlist_free(&a);
            return INPUT_ERROR;
        }

        mnode_t *sum = mlist_mul(a, b);
        mlist_free(&a);
        mlist_free(&b);
        if (!sum)
            return SUM_ERROR;

        mlist_to_file(stdout, sum);
        mlist_free(&sum);
    }
    else
        return PARAM_ERROR;

    return EXIT_SUCCESS;
}
