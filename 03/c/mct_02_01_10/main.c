#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "my_string.h"
#include "return_codes.h"

int main(int argc, char **argv)
{
    if (argc < 3)
        return exit_invalid_args;

    if (strlen(argv[1]) >= MY_STRING_LEN)
        return exit_invalid_args;

    if (strlen(argv[2]) >= MY_STRING_LEN)
        return exit_invalid_args;

    my_string_t filename;
    my_string_t s;

    strcpy(filename, argv[1]);
    strcpy(s, argv[2]);

    my_string_t str;

    FILE *f = fopen(filename, "r");
    if (f == NULL)
        return exit_file_open_error;

    int counter = 0;
    int read = 0;

    while (read_line(str, f))
    {
        read++;

        my_string_t str_with_syms;
        copy_with_symbols(str, s, str_with_syms);
        if (is_palindrome(str_with_syms) && strlen(str_with_syms) > 0)
        {
            counter++;
            printf("%s\n", str);
        }
    }

    int code = exit_success;

    if (counter == 0)
        code = exit_empty_solution;

    if (read == 0)
        code = exit_invalid_input;

    fclose(f);
    return code;
}
