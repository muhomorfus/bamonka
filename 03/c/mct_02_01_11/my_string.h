#ifndef MY_STRING_H
#define MY_STRING_H

#include <stdio.h>
#include <stdbool.h>

#define MY_STRING_LEN 129

typedef char my_string_t[MY_STRING_LEN];

char *read_line(my_string_t str, FILE *f);
void copy_without_symbols(my_string_t str, char *symbols, my_string_t res);
bool is_palindrome(char *str);

#endif //MY_STRING_H
