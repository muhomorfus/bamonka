#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "inc/array.h"

int create_arr(int **pb_dst, int **pe_dst, size_t len)
{
    *pb_dst = malloc(len * sizeof(int));

    if (!*pb_dst)
        return EXIT_FAILURE;

    *pe_dst = *pb_dst + len;
    int *p_arr = *pb_dst;
    while (p_arr < *pe_dst)
    {
        *p_arr = rand() % 1000;
        p_arr++;
    }

    return EXIT_SUCCESS;
}

int main(void)
{
    FILE *f;
    f = fopen("mysort.txt", "w");
    if (f == NULL)
        return EXIT_FAILURE;

    for (size_t len = 1; len < 101; len += 1)
    {
        struct timeval tv_start, tv_stop;
        long long sorting_time;
        long long sum = 0;

        for (size_t i = 0; i < 100; i++)
        {
            int *pb_dst = NULL;
            int *pe_dst = NULL;
            if (create_arr(&pb_dst, &pe_dst, len))
                return EXIT_FAILURE;

            gettimeofday(&tv_start, NULL);
            mysort(pb_dst, len, sizeof(int), compare_ints);
            gettimeofday(&tv_stop, NULL);
            sorting_time = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL +
                           (tv_stop.tv_usec - tv_start.tv_usec);

            sum += sorting_time;

            free(pb_dst);
        }

        fprintf(f, "%zu %lld\n", len, sum / 100);
    }

    fclose(f);
    f = fopen("qsort.txt", "w");
    if (f == NULL)
        return EXIT_FAILURE;

    for (size_t len = 1; len < 101; len += 1)
    {
        struct timeval tv_start, tv_stop;
        long long sorting_time;
        long long sum = 0;

        for (size_t i = 0; i < 100; i++)
        {
            int *pb_dst = NULL;
            int *pe_dst = NULL;
            if (create_arr(&pb_dst, &pe_dst, len))
                return EXIT_FAILURE;

            gettimeofday(&tv_start, NULL);
            qsort(pb_dst, len, sizeof(int), compare_ints);
            gettimeofday(&tv_stop, NULL);
            sorting_time = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL +
                           (tv_stop.tv_usec - tv_start.tv_usec);

            sum += sorting_time;

            free(pb_dst);
        }

        fprintf(f, "%zu %lld\n", len, sum / 100);
    }
    fclose(f);

    return EXIT_SUCCESS;
}