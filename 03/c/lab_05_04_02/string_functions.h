#ifndef STRING_FUNCTIONS_H
#define STRING_FUNCTIONS_H

#include "return_code.h"

#define STR_MAX_SIZE 32

return_code read_line(char *str, size_t size);
bool has_suffix(char *str, char *suffix);

#endif //STRING_FUNCTIONS_H
