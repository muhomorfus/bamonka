#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#define ERROR_INVALID_ARGS 2
#define ERROR_OPEN_FILE 3
#define ERROR_OPEN_LIBRARY 4
#define ERROR_GET_LIBRARY_FUNCTION 5

int main(int argc, char **argv)
{
    if (argc != 3 && argc != 4)
        return ERROR_INVALID_ARGS;

    void *hlib;
    hlib = dlopen("libarray.so", RTLD_NOW);
    if (!hlib)
        return ERROR_OPEN_LIBRARY;

    int (*f_count_ints)(FILE *, size_t *);
    int (*f_read_ints_array)(FILE *, int *, int *);
    int (*f_print_ints_array)(FILE *, int *, int *);
    int (*new_ints_array)(size_t, int **);
    void (*free_ints_array)(int **);
    int (*key)(const int *, const int *, int **, int **);
    int (*compare_ints)(const void *, const void *);
    void (*mysort)(void *, size_t, size_t, int (*)(const void *, const void *));

    f_count_ints = dlsym(hlib, "f_count_ints");
    f_read_ints_array = dlsym(hlib, "f_read_ints_array");
    f_print_ints_array = dlsym(hlib, "f_print_ints_array");
    new_ints_array = dlsym(hlib, "new_ints_array");
    free_ints_array = dlsym(hlib, "free_ints_array");
    key = dlsym(hlib, "key");
    compare_ints = dlsym(hlib, "compare_ints");
    mysort = dlsym(hlib, "mysort");

    if (!f_count_ints || !f_read_ints_array || !f_print_ints_array ||
        !new_ints_array || !free_ints_array || !key ||
        !compare_ints || !mysort)
    {
        dlclose(hlib);
        return ERROR_GET_LIBRARY_FUNCTION;
    }

    FILE *f_in = fopen(argv[1], "r");
    if (!f_in)
    {
        dlclose(hlib);
        return ERROR_OPEN_FILE;
    }

    size_t n;
    int code = f_count_ints(f_in, &n);
    if (code)
    {
        fclose(f_in);
        dlclose(hlib);
        return code;
    }
    int *pb, *pe;
    code = new_ints_array(n, &pb);
    if (code)
    {
        fclose(f_in);
        dlclose(hlib);
        return code;
    }
    pe = pb + n;

    code = f_read_ints_array(f_in, pb, pe);
    fclose(f_in);
    if (code)
    {
        free_ints_array(&pb);
        dlclose(hlib);
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
                dlclose(hlib);
                return code;
            }
            pb = pb_filtered;
            pe = pe_filtered;
        }
        else
        {
            free_ints_array(&pb);
            dlclose(hlib);
            return ERROR_INVALID_ARGS;
        }
    }
    mysort(pb, pe - pb, sizeof(int), compare_ints);

    FILE *f_out = fopen(argv[2], "w");
    if (!f_out)
    {
        free_ints_array(&pb);
        dlclose(hlib);
        return ERROR_OPEN_FILE;
    }

    code = f_print_ints_array(f_out, pb, pe);
    free_ints_array(&pb);
    fclose(f_out);
    dlclose(hlib);

    return code;
}
