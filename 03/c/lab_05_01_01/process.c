#include <stdio.h>

#include "process.h"

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int process(FILE *f, int *max1, int *max2)
{
    if (fscanf(f, "%d", max1) != 1)
        return EXIT_EMPTY_INPUT;

    if (fscanf(f, "%d", max2) != 1)
        return EXIT_NOT_TOO_FEW_NUMBERS;

    if (*max1 < *max2)
        swap(max1, max2);

    int num;
    while (fscanf(f, "%d", &num) == 1)
        if (num > *max1)
        {
            *max2 = *max1;
            *max1 = num;
        }
        else if (num > *max2)
            *max2 = num;

    return EXIT_SUCCESS;
}
