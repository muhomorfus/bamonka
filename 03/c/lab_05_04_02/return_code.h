#ifndef RETURN_CODES_H
#define RETURN_CODES_H

typedef enum
{
    exit_success = 0,
    exit_failure = 1,
    exit_ftell_error = 2,
    exit_fseek_error = 3,
    exit_empty_file = 4,
    exit_not_enough_args = 53,
    exit_fopen_error = 6,
    exit_invalid_file = 7,
    exit_fwrite_error = 8,
    exit_fread_error = 9,
    exit_file_overflow = 10,
    exit_invalid_args = 53,
    exit_dst_not_empty = 12,
    exit_string_overflow = 13,
    exit_invalid_input = 14,
    exit_fgets_error = 15,
    exit_no_solution = 16,
} return_code;

#endif //RETURN_CODES_H
