#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#include "oa.h"
#include "error.h"

static double random_double(double min, double max)
{
    double n = rand();
    n /= INT_MAX;
    n *= max - min;
    n += min;
    return n;
}

error_t oa_start(oa_t *oa, task_t task, double timer)
{
    if (!oa)
        return NEW_ERROR("передан нулевой указатель", OA_ARGS_ERROR);

    if (!oa->is_working)
    {
        oa->current_task = task;
        oa->is_working = true;
        oa->start_time = timer;
        oa->process_time = random_double(oa->process_min_time[task.type], oa->process_max_time[task.type]);
    }

    return NEW_SUCCESS();
}

error_t oa_finish(oa_t *oa, task_t *exited)
{
    if (!oa)
        return NEW_ERROR("передан нулевой указатель", OA_ARGS_ERROR);

    oa->is_working = false;
    if (exited)
        *exited = oa->current_task;
    return NEW_SUCCESS();
}

bool oa_done(oa_t *oa, double timer)
{
    if (!oa)
        return false;

    return oa->is_working && timer >= oa->start_time + oa->process_time;
}

double oa_end_time(oa_t *oa)
{
    if (!oa)
        return -1;

    return oa->start_time + oa->process_time;
}

task_t task_new(task_type_t type, double timer)
{
    task_t task = {
            .type = type,
            .content = rand() % MAX_TASK_CONTENT,
            .enter_time = timer,
    };

    return task;
}
