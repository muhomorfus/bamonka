#include <stdio.h>
#include <stdlib.h>

#define SECONDS_IN_MINUTE 60
#define SECONDS_IN_HOUR 3600

int main(void)
{
    int total_seconds;
    int hours, minutes, seconds;

    printf("Enter time in seconds: ");
    scanf("%d", &total_seconds);

    hours = total_seconds / SECONDS_IN_HOUR;
    total_seconds -= hours * SECONDS_IN_HOUR;

    minutes = total_seconds / SECONDS_IN_MINUTE;
    total_seconds -= minutes * SECONDS_IN_MINUTE;

    seconds = total_seconds;

    printf("Time in hours, minutes and seconds: ");
    printf("%d %d %d\n", hours, minutes, seconds);

    return EXIT_SUCCESS;
}