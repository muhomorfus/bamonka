#ifndef DATE_H
#define DATE_H

#include <stdbool.h>
#include "short_string.h"

#define NUMBER_OF_MONTHS 12
#define MAX_YEAR 9999
#define MIN_YEAR 0
#define MIN_DAY 1
#define MAX_DAY 31

typedef struct
{
    short_string_t name;
    int length;
    int leap_length;
} month_info;

bool is_correct_date(char *str);

#endif // DATE_H
