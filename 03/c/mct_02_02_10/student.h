#ifndef STUDENT_H
#define STUDENT_H

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "return_codes.h"

#define LAST_NAME_LEN 126
#define FIRST "Ivanov"
#define LAST "Petrov"

#pragma pack(push, 1)
typedef struct {
    char last_name[LAST_NAME_LEN];
    int64_t number;
} student_t;
#pragma pack(pop)

return_code count_students(FILE *f, size_t *size);
return_code get_by_pos(FILE *f, size_t pos, student_t *student);
return_code put_by_pos(FILE *f, size_t pos, student_t student);
return_code truncate_after_pos(FILE *f, size_t pos);
bool is_between(student_t s, char *first, char *last);

#endif //STUDENT_H
