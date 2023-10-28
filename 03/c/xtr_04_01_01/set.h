#ifndef SET_H
#define SET_H

#include <stdlib.h>

#define SET_MAX_SIZE 256

typedef struct set
{
    size_t size;
    int elements[SET_MAX_SIZE];
} set_t;

size_t set_find_position(const set_t *set, int element);
bool set_in(const set_t *set, int element);

set_t *set_add(set_t *set, int element);
set_t *set_remove(set_t *set, int element);

size_t set_print(const set_t *set);

void set_copy(const set_t *src, set_t *dst);

set_t set_intersection(const set_t *a, const set_t *b);
set_t set_union(const set_t *a, const set_t *b);
set_t set_difference(const set_t *a, const set_t *b);
set_t set_symmetric_difference(const set_t *a, const set_t *b);

#endif //SET_H
