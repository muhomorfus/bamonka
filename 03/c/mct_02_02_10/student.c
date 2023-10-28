#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "student.h"
#include "return_codes.h"

return_code count_students(FILE *f, size_t *size) {
    if (fseek(f, 0L, SEEK_END) != 0)
        return exit_seek_error;

    long int offset = ftell(f);
    if (offset == -1L)
        return exit_tell_error;

    if (offset % sizeof(student_t) != 0)
        return exit_file_error;

    *size = offset / sizeof(student_t);
    return exit_success;
}

return_code get_by_pos(FILE *f, size_t pos, student_t *student) {
    if (fseek(f, pos * sizeof(student_t), SEEK_SET) != 0)
        return exit_seek_error;

    if (fread(student, sizeof(student_t), 1, f) != 1)
        return exit_read_error;

    if (fseek(f, 0L, SEEK_SET) != 0)
        return exit_seek_error;

    return exit_success;
}

return_code put_by_pos(FILE *f, size_t pos, student_t student) {
    if (fseek(f, pos * sizeof(student_t), SEEK_SET) != 0)
        return exit_seek_error;

    if (fwrite(&student, sizeof(student_t), 1, f) != 1)
        return exit_read_error;

    if (fseek(f, 0L, SEEK_SET) != 0)
        return exit_seek_error;

    return exit_success;
}

return_code truncate_after_pos(FILE *f, size_t pos) {
    student_t zero_student = { "0000", 0 };
    return put_by_pos(f, pos, zero_student);
}

bool is_between(student_t s, char *first, char *last) {
    return strcmp(s.last_name, first) >= 0 && strcmp(s.last_name, last) <= 0;
}