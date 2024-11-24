#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    printf("PID: %d\n", getpid());

    char c;

    while((read(0, &c, 1) == 1))
        write(1, &c, 1);

    return 0;
}