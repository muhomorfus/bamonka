#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define UINT32_MAX_BIT (int)(sizeof(uint32_t) * 8)

uint32_t shift_left(uint32_t a, int n)
{
    n = n % UINT32_MAX_BIT;
    return (a << n) | (a >> (UINT32_MAX_BIT - n));
}

void print_bin(uint32_t a)
{
    int digit;

    for (int i = UINT32_MAX_BIT - 1; i >= 0; i--)
    {
        digit = (a & (1 << i)) >> i;
        printf("%1d", digit);
    }
}

int main(void)
{
    uint32_t a;
    int n;

    printf("Enter a: ");
    if (scanf("%"SCNu32, &a) != 1)
    {
        printf("Error: invalid input\n");
        return EXIT_FAILURE;
    }

    printf("Enter n: ");
    if (scanf("%d", &n) != 1 || n < 0)
    {
        printf("Error: invalid input\n");
        return EXIT_FAILURE;
    }

    uint32_t result = shift_left(a, n);
    printf("Result: ");
    print_bin(result);
    printf("\n");
}