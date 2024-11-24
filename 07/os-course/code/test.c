#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main()
{

    char test[16] = { 0 };
    printf("PID: %d\n", getpid());
    read(1, test, 2);

    return 0;
}