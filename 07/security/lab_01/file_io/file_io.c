#include "file_io.h"

int f_read(char* filename, unsigned char** str, int* size) {
    FILE* f;

    f = fopen(filename, "rb");
    if (f == NULL)
    {
        printf("fopen() failed\n");
        return EXIT_FAILURE;
    }

    fseek(f, 0L, SEEK_END);
    *size = ftell(f);
    rewind(f);

    *str = malloc(*size + 1);

    char c;
    for (size_t i = 0; i < *size; i++) {
        c = fgetc(f);
        (*str)[i] = c;
    }

    fclose(f);

    return EXIT_SUCCESS;
}

int f_write(char* filename, unsigned char* str, int size) {
    FILE* f;

    f = fopen(filename, "wb");
    if (f == NULL)
    {
        printf("fopen() failure\n");
        return EXIT_FAILURE;
    }

    if (fwrite(str, sizeof(char), size, f) != size) {
        printf("fwrite() failure\n");
        fclose(f);
        return EXIT_FAILURE;
    }

    fclose(f);

    return EXIT_SUCCESS;
}
