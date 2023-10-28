#include <stdio.h>

#include "student.h"

int main(int argc, char **argv) {
    if (argc < 2)
        return exit_invalid_args;

    FILE *f;
    f = fopen(argv[1], "rb+");
    if (f == NULL)
        return exit_file_open_error;

    size_t n;
    return_code code;
    code = count_students(f, &n);
    if (code != exit_success)
    {
        fclose(f);
        return code;
    }

    if (n == 0)
    {
        fclose(f);
        return exit_empty_file;
    }

    size_t j = 0;
    for (size_t i = 0; i < n; i++)
    {
        student_t student;
        code = get_by_pos(f, i, &student);
        if (code != exit_success)
        {
            fclose(f);
            return code;
        }

        if (!is_between(student, FIRST, LAST))
        {
            code = put_by_pos(f, j++, student);
            if (code != exit_success)
            {
                fclose(f);
                return code;
            }
        }
    }
    code = truncate_after_pos(f, j);
    fclose(f);
    return code;
}