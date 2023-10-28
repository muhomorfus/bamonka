#include <stdio.h>

#include "process.h"

int main(void)
{
    int code;
    int max1, max2;

    code = process(stdin, &max1, &max2);

    if (code == EXIT_SUCCESS)
        printf("%d %d\n", max1, max2);

    return code;
}