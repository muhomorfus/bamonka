#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "short_string.h"
#include "exit_codes.h"

int main(void)
{
    short_string_t str;
    char *separators = " ,;:-.!?";
    if (!read_line(str))
        return exit_input_overflow;

    short_strings_t words;
    size_t words_num = strtok_split(str, separators, words);
    if (words_num == 0)
        return exit_empty_result;

    words_num = delete_occurrences(words, words_num, words[words_num - 1]);
    if (words_num == 0)
        return exit_empty_result;

    reverse(words, words_num);

    for (size_t i = 0; i < words_num; i++)
        delete_duplicates_of_first(words[i]);

    short_string_t new_str;
    short_string_t separator = " ";
    join(words, words_num, separator, new_str);

    printf("Result: %s\n", new_str);
    return exit_success;
}