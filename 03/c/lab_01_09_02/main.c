#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void add_to_sum(double *sum, int n, double x)
{
    *sum += sqrt((double) n + x);
}

int main(void)
{
    double x = 0.0;
    double g = 0.0;
    int count;
    int n = 0;

    while ((count = scanf("%lf", &x)) == 1 && x >= 0)
    {
        n++;
        add_to_sum(&g, n, x);
    }
    if (n != 0)
        g /= n;

    if (count != 1)
    {
        printf("[ERROR] invalid input\n");
        return EXIT_FAILURE;
    }

    printf("g = %.6lf\n", g);
    return EXIT_SUCCESS;
}