#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define EPS 1e-8

bool is_zero(double x)
{
    return fabs(x) < EPS;
}

int main(void)
{
    double r1, r2, r3;
    double r;

    printf("Enter three resistances: ");
    scanf("%lf%lf%lf", &r1, &r2, &r3);

    if (is_zero(r1) && is_zero(r2) && is_zero(r3))
        r = 0.0;
    else
        r = (r1 * r2 * r3) / (r1 * r2 + r1 * r3 + r2 * r3);

    printf("R = %.6lf\n", r);

    return EXIT_SUCCESS;
}