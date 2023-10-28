#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void)
{
    double x_a, y_a;
    double x_b, y_b;
    double x_c, y_c;

    double ab, ac, bc;
    double p;

    printf("Enter x and y of point A: ");
    scanf("%lf%lf", &x_a, &y_a);

    printf("Enter x and y of point B: ");
    scanf("%lf%lf", &x_b, &y_b);

    printf("Enter x and y of point C: ");
    scanf("%lf%lf", &x_c, &y_c);

    ab = sqrt((x_a - x_b) * (x_a - x_b) + (y_a - y_b) * (y_a - y_b));
    ac = sqrt((x_a - x_c) * (x_a - x_c) + (y_a - y_c) * (y_a - y_c));
    bc = sqrt((x_b - x_c) * (x_b - x_c) + (y_b - y_c) * (y_b - y_c));

    p = ab + bc + ac;

    printf("P = %.6lf\n", p);

    return EXIT_SUCCESS;
}