#include <stdio.h>
#include <math.h>

#include "file_processing.h"
#include "return_code.h"

return_code find_average(FILE *f, double *average)
{
    *average = 0;

    long int offset = ftell(f);
    if (offset == -1L)
        return exit_ftell_error;

    if (fseek(f, 0L, SEEK_SET) != 0)
        return exit_fseek_error;

    int read = 0;
    double current;

    while (fscanf(f, "%lf", &current) == 1)
    {
        read++;
        *average += current;
    }

    if (read == 0)
        return exit_empty_file;

    *average /= read;

    if (fseek(f, offset, SEEK_SET) != 0)
        return exit_fseek_error;

    return exit_success;
}

return_code find_nearest_to_average(FILE *f, double *nearest)
{
    *nearest = 0;

    double average;
    return_code code = find_average(f, &average);

    if (code != exit_success)
        return code;

    long int offset = ftell(f);
    if (offset == -1L)
        return exit_ftell_error;

    if (fseek(f, 0L, SEEK_SET) != 0)
        return exit_fseek_error;

    if (fscanf(f, "%lf", nearest) != 1)
        return exit_empty_file;

    double current;
    while (fscanf(f, "%lf", &current) == 1)
        if (fabs(current - average) < fabs(*nearest - average))
            *nearest = current;

    if (fseek(f, offset, SEEK_SET) != 0)
        return exit_fseek_error;

    return exit_success;
}
