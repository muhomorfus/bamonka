#ifndef ENIGMA_H
#define ENIGMA_H

#include <stdlib.h>
#include <string.h>

#include "file_io.h"

typedef struct {
    int counter;
    int alphabet_count;
    int rotors_count;
    int *panel;
    int *reflector;
    int **rotors;
    int **reverse_rotors;
    int *shifts;
} enigma_t;

void e_print(enigma_t* e);
unsigned char* e_encode(enigma_t* e, unsigned char* data, size_t size);
int e_read(enigma_t *e);
void e_free(enigma_t* e);

#endif //ENIGMA_H
