#include <stdio.h>

#include "file_processing.h"
#include "return_code.h"

int main(int argc, char **argv)
{
    if (argc < 2)
        return exit_not_enough_args;

    FILE *f = fopen(argv[1], "r");
    if (f == NULL)
        return exit_fopen_error;

    double nearest;
    return_code code = find_nearest_to_average(f, &nearest);
    if (code == exit_success)
        printf("%.6lf\n", nearest);

    fclose(f);
    return code;
}