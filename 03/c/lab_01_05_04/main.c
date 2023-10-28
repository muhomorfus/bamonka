#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_prime(int n)
{
    if (n % 2 == 0 && n != 2)
        return false;

    for (int i = 3; i <= n / 2; i += 2)
        if (n % i == 0)
            return false;

    return true;
}

int main(void)
{
    int n;

    printf("Enter n: ");
    if (scanf("%d", &n) != 1 || n <= 0)
    {
        printf("[ERROR] invalid input\n");
        return EXIT_FAILURE;
    }

    if (n == 1)
        return EXIT_SUCCESS;
    int i = 2;
    while (n > 1)
    {
        if (is_prime(i))
        {
            // Выводим простой делитель i столько раз,
            // сколько он встречается в числе
            while (n % i == 0)
            {
                printf("%d ", i);
                n /= i;
            }
        }
        i++;
    }
    printf("\n");

    return EXIT_SUCCESS;
}