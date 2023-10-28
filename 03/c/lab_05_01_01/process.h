#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>

#define EXIT_SUCCESS 0
#define EXIT_EMPTY_INPUT -1
#define EXIT_NOT_TOO_FEW_NUMBERS -2

int process(FILE *f, int *max1, int *max2);

#endif //PROCESS_H
