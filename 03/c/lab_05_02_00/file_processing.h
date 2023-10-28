#ifndef FILE_PROCESSING_H
#define FILE_PROCESSING_H

#include <stdio.h>

#include "return_code.h"

return_code find_average(FILE *f, double *average);
return_code find_nearest_to_average(FILE *f, double *nearest);

#endif //FILE_PROCESSING_H
