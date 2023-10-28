#ifndef RETURN_CODES_H
#define RETURN_CODES_H

typedef enum {
    exit_success = 0,
    exit_failure = 1,
    exit_invalid_args = 2,
    exit_invalid_input = 3,
    exit_file_open_error = 4,
    exit_empty_solution = 5,
} return_code;

#endif //RETURN_CODES_H
