#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include "flats_db.h"

#define NUM_OF_TESTS 20

long long count_time(void (*f) (flats_db_t *), flats_db_t *db)
{
    flats_db_t db_clean = *db;
    db_read(&db_clean);
    struct timeval tv_start, tv_stop;
    int64_t elapsed_time;
    gettimeofday(&tv_start, NULL);
    (*f) (&db_clean);
    gettimeofday(&tv_stop, NULL);
    elapsed_time = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL +
                   (tv_stop.tv_usec - tv_start.tv_usec);

    return elapsed_time;
}

long long benchmark(void (*f) (flats_db_t *), flats_db_t *db)
{
    long long time = 0;
    for (size_t i = 0; i < NUM_OF_TESTS; i++)
        time += count_time(f, db);

    return time / NUM_OF_TESTS;
}

