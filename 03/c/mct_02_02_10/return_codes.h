#ifndef RETURN_CODES_H
#define RETURN_CODES_H

typedef enum {
    exit_success = 0,
    exit_failure = 1,
    exit_invalid_args = 2,
    exit_invalid_input = 3,
    exit_file_open_error = 4,
    exit_empty_solution = 5,
    exit_io_error = 6,
    exit_seek_error = 7,
    exit_tell_error = 8,
    exit_read_error = 9,
    exit_write_error = 10,
    exit_file_error = 11,
    exit_file_overflow = 12,
    exit_empty_file = 13,
} return_code;

#endif //RETURN_CODES_H
