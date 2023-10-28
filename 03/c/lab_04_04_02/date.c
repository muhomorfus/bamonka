#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "date.h"
#include "short_string.h"

const month_info months[NUMBER_OF_MONTHS] =
{
    { "january", 31, 31 },
    { "february", 28, 29 },
    { "march", 31, 31 },
    { "april", 30, 30 },
    { "may", 31, 31 },
    { "june", 30, 30 },
    { "july", 31, 31 },
    { "august", 31, 31 },
    { "september", 30, 30 },
    { "october", 31, 31 },
    { "november", 30, 30 },
    { "december", 31, 31 },
};

bool is_correct_year(char *year_str)
{
    if (!is_correct_positive(year_str))
        return false;

    int year = atoi(year_str);
    return MIN_YEAR <= year && year <= MAX_YEAR;
}

bool is_correct_day(char *day_str)
{
    if (!is_correct_positive(day_str))
        return false;

    int day = atoi(day_str);
    return MIN_DAY <= day && day <= MAX_DAY;
}

bool is_leap(int year)
{
    if (year % 4 == 0)
        if (year % 400 == 0 || year % 100 != 0)
            return true;

    return false;
}

size_t get_month_index(char *month_str)
{
    to_lower(month_str);

    for (size_t i = 0; i < NUMBER_OF_MONTHS; i++)
        if (strcmp(month_str, months[i].name) == 0)
            return i;

    return NUMBER_OF_MONTHS;
}

bool is_correct_date(char *str)
{
    short_strings_t words;

    size_t words_num = split_by_spaces(str, words);
    if (words_num != 3)
        return false;

    if (!is_correct_day(words[0]))
        return false;

    size_t month_index = get_month_index(words[1]);
    if (month_index >= NUMBER_OF_MONTHS)
        return false;

    if (!is_correct_year(words[2]))
        return false;

    int day = atoi(words[0]);
    int year = atoi(words[2]);

    if (is_leap(year))
        return day <= months[month_index].leap_length;

    return day <= months[month_index].length;
}