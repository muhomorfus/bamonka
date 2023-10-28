#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define EPS 1e-8

double f(double x) 
{
    return -exp(-x * x * x) * (1 + x) * (1 - x + x * x) / pow(x, 5);
}

double s(double x, double eps)
{
    double cur = x / 2;
    double sum = cur;
    int i = 1;

    while (fabs(cur) >= eps)
    {
        cur *= -(x * x * x) * (i + 1) / (i * (i + 2));
        sum += cur;
        i++;
    }

    return sum;
}

int main(void)
{
    double x;
    double eps;
    
    printf("Enter x: ");
    if (scanf("%lf", &x) != 1 || fabs(x) < EPS)
    {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }

    printf("Enter eps: ");
    if (scanf("%lf", &eps) != 1 || eps <= 0)
    {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }

    double f_value = f(x);
    double s_value = s(x, eps);

    printf("f(x) = %e\n", f_value);
    printf("s(x, eps) = %g\n", s_value);
    return EXIT_SUCCESS;
}