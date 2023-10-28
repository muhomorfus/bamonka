#ifndef NUMBERS_H
#define NUMBERS_H

#include <stdio.h>
#include <stdlib.h>

#include "return_code.h"

return_code count_numbers(FILE *f, size_t *n);
return_code get_number_by_pos(FILE *f, size_t pos, int *number);
return_code put_number_by_pos(FILE *f, size_t pos, int number);

#endif //NUMBERS_H
