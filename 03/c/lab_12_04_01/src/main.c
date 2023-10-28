#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"

#define ERROR_INVALID_ARGS 2
#define ERROR_OPEN_FILE 3

int main(int argc, char **argv)
{
    if (argc != 3 && argc != 4)
        return ERROR_INVALID_ARGS;

    FILE *f_in = fopen(argv[1], "r");
    if (!f_in)
        return ERROR_OPEN_FILE;

    size_t n;
    int code = f_count_ints(f_in, &n);
    if (code)
    {
        fclose(f_in);
        return code;
    }
    int *pb, *pe;
    code = new_ints_array(n, &pb);
    if (code)
    {
        fclose(f_in);
        return code;
    }
    pe = pb + n;

    code = f_read_ints_array(f_in, pb, pe);
    fclose(f_in);
    if (code)
    {
        free_ints_array(&pb);
        return code;
    }
    if (argc == 4)
    {
        if (!strcmp(argv[3], "f"))
        {
            int *pb_filtered, *pe_filtered;
            code = new_ints_array(n, &pb_filtered);
            if (code)
            {
                free_ints_array(&pb);
                return code;
            }
            pe_filtered = pb_filtered + n;

            code = key(pb, pe, &pb_filtered, &pe_filtered);
            free_ints_array(&pb);
            if (code)
            {
                free_ints_array(&pb_filtered);
                return code;
            }
            pb = pb_filtered;
            pe = pe_filtered;
        }
        else
        {
            free_ints_array(&pb);
            return ERROR_INVALID_ARGS;
        }
    }
    mysort(pb, pe - pb, sizeof(int), compare_ints);

    FILE *f_out = fopen(argv[2], "w");
    if (!f_out)
    {
        free_ints_array(&pb);
        return ERROR_OPEN_FILE;
    }

    code = f_print_ints_array(f_out, pb, pe);
    free_ints_array(&pb);
    fclose(f_out);

    return code;
}
