#include <stdio.h>
#include "my_string_functions.h"
#include "test_my_string_functions.h"
#include <string.h>

int main(void)
{
    int failed = 0;
    failed += test_my_strpbrk();
    failed += test_my_strspn();
    failed += test_my_strcspn();
    failed += test_my_strchr();
    failed += test_my_strrchr();

    return failed;
}