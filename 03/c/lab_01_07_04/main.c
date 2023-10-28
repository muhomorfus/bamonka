#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double f(double x)
{
    return 1 / ((1 + x) * (1 + x) * (1 + x));
}

double s(double x, double eps)
{
    double element = 1.0;
    double sum = 1.0;
    int i = 1;

    while (fabs(element) > eps)
    {
        element *= (-1) * (i + 2) * x / i;
        i++;
        sum += element;
    }

    return sum;
}

int main(void)
{
    double x;
    double eps;

    printf("Enter x: ");
    if (scanf("%lf", &x) != 1 || fabs(x) >= 1)
    {
        printf("[ERROR] invalid input\n");
        return EXIT_FAILURE;
    }

    printf("Enter eps: ");
    if (scanf("%lf", &eps) != 1 || eps <= 0 || eps > 1)
    {
        printf("[ERROR] invalid input\n");
        return EXIT_FAILURE;
    }

    double computed_value = s(x, eps);
    double func_value = f(x);
    double absolute_error = fabs(func_value - computed_value);
    double relative_error = absolute_error / fabs(func_value);

    printf("s(x) = %.6lf\n", computed_value);
    printf("f(x) = %.6lf\n", func_value);
    printf("Delta = %.6lf\n", absolute_error);
    printf("delta = %.6lf\n", relative_error);

    return EXIT_SUCCESS;
}