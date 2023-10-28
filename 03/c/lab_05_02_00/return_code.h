#ifndef RETURN_CODES_H
#define RETURN_CODES_H

typedef enum
{
    exit_success = 0,
    exit_failure = 1,
    exit_ftell_error = 2,
    exit_fseek_error = 3,
    exit_empty_file = 4,
    exit_not_enough_args = 5,
    exit_fopen_error = 6,
} return_code;

#endif //RETURN_CODES_H
