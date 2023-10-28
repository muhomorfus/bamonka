#include <stdio.h>
#include <stdlib.h>

#include "bigfloat.h"
#include "error.h"
#include "short_string.h"

#define ERROR(err) {printf("%s (%s).", err.text, err.func); \
return err.code;}
#define CHECK(err) if (is_failure(err)) ERROR(err)

int main()
{
    printf("+---------------------------------------------------------+\n");
    printf("| Программа для расчета деления целого числа длиной до 30 |\n");
    printf("| десятичных цифр на действительное число в виде ±m.nE±K, |\n");
    printf("| где суммарная длина мантиссы - 30 символов, а величина  |\n");
    printf("| порядка K - до пяти цифр.                               |\n");
    printf("|                                                         |\n");
    printf("| Результат выдается в форме ±0.aE±B, где a - до 30       |\n");
    printf("| значащих цифр, а B - до пяти цифр.                      |\n");
    printf("+---------------------------------------------------------+\n");
    printf("\n");

    printf("                               <------- 30  символов ------->\n");
    printf("Введите первое число (целое): ");
    short_string_t str_a;
    error_t err = f_read_line(stdin, str_a);
    CHECK(err);
    printf("\n"); //
    printf("                                      <------- 30  символов ------->\n");
    printf("Введите второе число (вещественное): ");
    short_string_t str_b;
    err = f_read_line(stdin, str_b);
    CHECK(err);

    bigint_t bi_a;
    err = bi_from_str(str_a, &bi_a);
    CHECK(err);

    bigfloat_t a;
    err = bf_from_bigint(bi_a, &a);
    CHECK(err);

    bigfloat_t b;
    err = bf_from_str(str_b, &b);
    CHECK(err)

    err = bf_div(&a, b);
    CHECK(err);

    short_string_t str_result;
    err = bf_to_str(str_result, a);
    CHECK(err);

    printf("\n");
    printf("Результат деления:\n");
    printf("   <------- 30  символов ------->\n");
    printf("%s\n", str_result);

    return EXIT_SUCCESS;
}
