#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "short_string.h"
#include "exit_codes.h"

int main(void)
{
    short_string_t s1;
    short_string_t s2;
    char *separators = " ,;:-.!?";

    if (!read_line(s1))
        return exit_input_overflow;

    if (!read_line(s2))
        return exit_input_overflow;

    short_strings_t words1;
    short_strings_t words2;

    size_t words1_num = split(s1, separators, words1);
    size_t words2_num = split(s2, separators, words2);

    words1_num = delete_duplicates(words1, words1_num);
    words2_num = delete_duplicates(words2, words2_num);

    if (words1_num == 0 || words2_num == 0)
        return exit_empty_result;

    printf("Result: \n");

    for (size_t i = 0; i < words1_num; i++)
    {
        printf("%s ", words1[i]);

        if (includes(words2, words2_num, words1[i]))
            printf("yes");
        else
            printf("no");
        printf("\n");
    }

    return exit_success;
}