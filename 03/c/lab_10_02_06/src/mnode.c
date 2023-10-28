#include <stdlib.h>
#include <stdio.h>

#include "mnode.h"

void mlist_free(mnode_t **head)
{
    if (!head || !*head)
        return;

    mnode_t *next;
    for (mnode_t *cur = *head; cur; cur = next)
    {
        next = cur->next;
        free(cur);
    }
}

mnode_t *mlist_append(mnode_t **head, mnode_t **last, size_t i, size_t j, int n)
{
    if (!head || !last)
        return NULL;

    mnode_t *buf = calloc(1, sizeof(mnode_t));
    if (!buf)
        return NULL;

    buf->n = n;
    buf->i = i;
    buf->j = j;

    if (!*last)
    {
        *last = buf;
        *head = buf;
    }
    else
    {
        (*last)->next = buf;
        (*last) = (*last)->next;
    }

    return *last;
}

mnode_t *mlist_from_file(FILE *f)
{
    if (!f)
        return NULL;

    size_t rows, cols;

    if (fscanf(f, "%zu%zu", &rows, &cols) != 2)
        return NULL;

    mnode_t *head = NULL, *cur = NULL;
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
        {
            int element;
            if (fscanf(f, "%d", &element) != 1)
            {
                mlist_free(&head);
                return NULL;
            }

            if (element)
                if (!mlist_append(&head, &cur, i, j, element))
                {
                    mlist_free(&head);
                    return NULL;
                }
        }

    return head;
}

void mlist_to_file(FILE *f, mnode_t *head)
{
    if (!f || !head)
        return;

    for (mnode_t *cur = head; cur; cur = cur->next)
        printf("%zu %zu %d ", cur->i, cur->j, cur->n);
}

mnode_t *mlist_add(mnode_t *a, mnode_t *b)
{
    mnode_t *head = NULL;
    mnode_t *cur = NULL;

    while (a || b)
    {
        if (!a || (b && ((a->i == b->i && a->j > b->j) || a->i > b->i)))
        {
            if (!mlist_append(&head, &cur, b->i, b->j, b->n))
            {
                mlist_free(&head);
                return NULL;
            }
            b = b->next;
        }
        else if (!b || (a && ((a->i == b->i && a->j < b->j) || a->i < b->i)))
        {
            if (!mlist_append(&head, &cur, a->i, a->j, a->n))
            {
                mlist_free(&head);
                return NULL;
            }
            a = a->next;
        }
        else
        {
            if (b->n + a->n)
                if (!mlist_append(&head, &cur, a->i, a->j, b->n + a->n))
                {
                    mlist_free(&head);
                    return NULL;
                }

            b = b->next;
            a = a->next;
        }
    }

    return head;
}

mnode_t *max_node(mnode_t *head)
{
    mnode_t *max = head;
    for (mnode_t *cur = head; cur; cur = cur->next)
        if (cur->n > max->n)
            max = cur;

    return max;
}

int mlist_del_row_max(mnode_t **head)
{
    if (!head || !*head)
        return MLIST_ARGS_ERROR;

    mnode_t *max = max_node(*head);
    if (!max)
        return MLIST_FIND_ERROR;

    size_t row = max->i;
    if ((*head)->i == row)
    {
        while (*head && (*head)->i == row)
        {
            mnode_t *next = (*head)->next;
            free(*head);
            *head = next;
        }
    }
    else
    {
        mnode_t *cur;
        for (cur = *head; cur && cur->next->i != row; cur = cur->next);
        if (!cur)
            return MLIST_FIND_ERROR;

        while (cur->next && cur->next->i == row)
        {
            mnode_t *next = cur->next->next;
            free(cur->next);
            cur->next = next;
        }
    }

    for (mnode_t *cur = *head; cur; cur = cur->next)
        if (cur->i > row)
            cur->i--;

    return EXIT_SUCCESS;
}

mnode_t *min_coordinates_node(mnode_t *head)
{
    mnode_t *min = head;
    for (mnode_t *cur = head; cur; cur = cur->next)
        if (min->i > cur->i || (min->i == cur->i && min->j > cur->j))
            min = cur;

    return min;
}

mnode_t *filter_coordinates(mnode_t *head, size_t i, size_t j)
{
    mnode_t *head_r = NULL;
    mnode_t *cur_r = NULL;

    for (mnode_t *cur = head; cur; cur = cur->next)
        if (cur->i != i || cur->j != j)
            if (!mlist_append(&head_r, &cur_r, cur->i, cur->j, cur->n))
            {
                mlist_free(&head_r);
                return NULL;
            }

    return head_r;
}

mnode_t *mlist_mul(mnode_t *a, mnode_t *b)
{
    mnode_t *head = NULL;
    mnode_t *cur = NULL;

    for (mnode_t *cur_a = a; cur_a; cur_a = cur_a->next)
        for (mnode_t *cur_b = b; cur_b; cur_b = cur_b->next)
        {
            if (cur_a->j == cur_b->i)
                if (!mlist_append(&head, &cur, cur_a->i, cur_b->j, cur_a->n * cur_b->n))
                {
                    mlist_free(&head);
                    return NULL;
                }
        }

    mnode_t *head_result = NULL;
    mnode_t *cur_result = NULL;

    mnode_t *min = min_coordinates_node(head);
    while (min)
    {
        int sum = 0;
        for (mnode_t *c = head; c; c = c->next)
            if (c->i == min->i && c->j == min->j)
                sum += c->n;

        if (!mlist_append(&head_result, &cur_result, min->i, min->j, sum))
        {
            mlist_free(&head);
            mlist_free(&head_result);
            return NULL;
        }

        mnode_t *buf = filter_coordinates(head, min->i, min->j);
        mlist_free(&head);
        head = buf;
        min = min_coordinates_node(head);
    }

    return head_result;
}
