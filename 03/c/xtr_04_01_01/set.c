#include <stdbool.h>
#include <stdio.h>

#include "set.h"

size_t set_find_position(const set_t *set, int element)
{
    if (set->size == 0)
        return 0;

    size_t left = 0;
    size_t right = set->size - 1;
    size_t middle;

    while (right > left)
    {
        middle = (left + right) / 2;
        if (set->elements[middle] < element)
            left = middle + 1;
        else if (set->elements[middle] > element)
            right = middle - 1;
        else
            return middle;
    }

    if (set->elements[left] < element)
        return left + 1;

    return left;
}

bool set_in(const set_t *set, int element)
{
    if (set->size == 0)
        return false;

    size_t left = 0;
    size_t right = set->size - 1;
    size_t middle;

    while (right > left)
    {
        middle = (left + right) / 2;
        if (set->elements[middle] < element)
            left = middle + 1;
        else if (set->elements[middle] > element)
            right = middle - 1;
        else
            return true;
    }

    return set->elements[left] == element;
}

set_t *set_add(set_t *set, int element)
{
    if (set_in(set, element))
        return set;

    if (set->size >= SET_MAX_SIZE)
        return NULL;

    size_t position = set_find_position(set, element);
    for (size_t i = set->size; i > position; i--)
        set->elements[i] = set->elements[i - 1];

    set->elements[position] = element;
    set->size++;

    return set;
}

set_t *set_remove(set_t *set, int element)
{
    if (set->size == 0)
        return NULL;

    if (!set_in(set, element))
        return set;

    size_t position = set_find_position(set, element);
    for (size_t i = position; i < set->size - 1; i++)
        set->elements[i] = set->elements[i + 1];
    set->size--;

    return set;
}

size_t set_print(const set_t *set)
{
    size_t i = 0;
    for (; i < set->size; i++)
        printf("%d ", set->elements[i]);

    if (i != 0)
        printf("\n");
    return i;
}

void set_copy(const set_t *src, set_t *dst)
{
    *dst = *src;
}

set_t set_intersection(const set_t *a, const set_t *b)
{
    set_t result = { 0 };

    size_t i = 0;
    size_t j = 0;
    while (i < a->size && j < b->size)
    {
        if (a->elements[i] < b->elements[j])
            i++;
        else if (a->elements[i] > b->elements[j])
            j++;
        else
        {
            result.elements[result.size++] = a->elements[i++];
            j++;
        }
    }

    return result;
}

set_t set_union(const set_t *a, const set_t *b)
{
    set_t result = { 0 };

    size_t i = 0;
    size_t j = 0;
    while (i < a->size || j < b->size)
    {
        if (result.size >= SET_MAX_SIZE)
            return result;

        if (j >= b->size)
            result.elements[result.size++] = a->elements[i++];
        else if (i >= a->size)
            result.elements[result.size++] = b->elements[j++];
        else if (a->elements[i] < b->elements[j])
            result.elements[result.size++] = a->elements[i++];
        else if (a->elements[i] > b->elements[j])
            result.elements[result.size++] = b->elements[j++];
        else
        {
            result.elements[result.size++] = a->elements[i++];
            j++;
        }
    }

    return result;
}

set_t set_difference(const set_t *a, const set_t *b)
{
    set_t result = { 0 };

    size_t i = 0;
    size_t j = 0;

    while (i < a->size)
    {
        if (j >= b->size || a->elements[i] < b->elements[j])
            result.elements[result.size++] = a->elements[i++];
        else if (a->elements[i] > b->elements[j])
            j++;
        else
        {
            i++;
            j++;
        }
    }

    return result;
}

set_t set_symmetric_difference(const set_t *a, const set_t *b)
{
    set_t result = { 0 };

    size_t i = 0;
    size_t j = 0;
    while (i < a->size || j < b->size)
    {
        if (result.size >= SET_MAX_SIZE)
            return result;

        if (j >= b->size)
            result.elements[result.size++] = a->elements[i++];
        else if (i >= a->size)
            result.elements[result.size++] = b->elements[j++];
        else if (a->elements[i] < b->elements[j])
            result.elements[result.size++] = a->elements[i++];
        else if (a->elements[i] > b->elements[j])
            result.elements[result.size++] = b->elements[j++];
        else
        {
            i++;
            j++;
        }
    }

    return result;
}