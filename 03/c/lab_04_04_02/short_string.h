#ifndef SHORT_STRING_H
#define SHORT_STRING_H

#include <stdlib.h>
#include <stdbool.h>

#define SHORT_STRING_SIZE 257
#define NUMBER_OF_SHORT_STRINGS 128
#define WORD_SIZE 16

typedef char short_string_t[SHORT_STRING_SIZE];
typedef short_string_t short_strings_t[NUMBER_OF_SHORT_STRINGS];

char *read_line(short_string_t str);

bool is_correct_positive(char *str);

size_t split(char *str, char *seps, short_string_t *strs);
size_t split_by_spaces(char *str, short_string_t *strs);
size_t strtok_split(char *str, char *seps, short_string_t *strs);

bool includes(short_string_t *strs, size_t n, short_string_t str);
size_t join(short_string_t *strs, size_t n, char *sep, short_string_t str);
void swap(short_string_t s1, short_string_t s2);
void reverse(short_string_t *strs, size_t n);

void to_lower(char *str);
size_t delete_duplicates_of_first(char *str);
size_t delete_char(char *str, size_t index);

size_t delete_str(short_string_t *strs, size_t n, size_t index);
size_t delete_occurrences(short_string_t *strs, size_t n, short_string_t str);
size_t delete_duplicates(short_string_t *strs, size_t n);
size_t delete_duplicates(short_string_t *strs, size_t n);

#endif // SHORT_STRING_H
