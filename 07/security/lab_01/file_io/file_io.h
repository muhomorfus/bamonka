#ifndef FILE_IO_H
#define FILE_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int f_read(char* filename, unsigned char** str, int* size);
int f_write(char* filename, unsigned char* str, int size);

#endif //FILE_IO_H
