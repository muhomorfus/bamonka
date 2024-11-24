#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    printf("PID: %d\n", getpid());

    char c;

    int fd_in = open("alphabet.txt", O_RDONLY);
    int fd_out = open("out.txt", O_CREAT | O_EXCL | O_RDWR);

    while((read(fd_in, &c, 1) == 1))
        write(fd_out, &c, 1);

    return 0;
}