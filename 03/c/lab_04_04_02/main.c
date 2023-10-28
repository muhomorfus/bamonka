#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "short_string.h"
#include "exit_codes.h"
#include "date.h"

int main(void)
{
    short_string_t str;
    if (!read_line(str))
        return exit_input_overflow;

    bool correct = is_correct_date(str);
    if (correct)
        printf("YES");
    else
        printf("NO");

    return exit_success;
}