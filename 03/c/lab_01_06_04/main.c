#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define EPS 1e-8

bool equal(double a, double b)
{
    return fabs(a - b) < EPS;
}

bool points_equal(double x1, double y1, double x2, double y2)
{
    return equal(x1, x2) && equal(y1, y2);
}

int in_segment(double xp, double yp, double xq, double yq, double xr, double yr)
{
    double tolerance = 0;
    double mul = xp * yq - xp * yr - xr * yq - xq * yp + xq * yr + xr * yp;
    tolerance += fabs(mul);

    double tolerance_x = fabs(fabs(xp - xr) + fabs(xp - xq) -
        fabs(xr - xq));
    double tolerance_y = fabs(fabs(yp - yr) + fabs(yp - yq) -
        fabs(yr - yq));
    tolerance += tolerance_x + tolerance_y;

    return tolerance <= EPS;
}

int main(void)
{
    double xp, yp;
    double xq, yq;
    double xr, yr;

    printf("Enter x and y of point q: ");
    if (scanf("%lf%lf", &xq, &yq) != 2)
    {
        printf("[ERROR] invalid input\n");
        return EXIT_FAILURE;
    }

    printf("Enter x and y of point r: ");
    if (scanf("%lf%lf", &xr, &yr) != 2)
    {
        printf("[ERROR] invalid input\n");
        return EXIT_FAILURE;
    }

    printf("Enter x and y of point p: ");
    if (scanf("%lf%lf", &xp, &yp) != 2)
    {
        printf("[ERROR] invalid input\n");
        return EXIT_FAILURE;
    }

    if (points_equal(xq, yq, xr, yr))
    {
        printf("[ERROR] rq must be section, not point\n");
        return EXIT_FAILURE;
    }

    printf("%d\n", in_segment(xp, yp, xq, yq, xr, yr));

    return EXIT_SUCCESS;
}