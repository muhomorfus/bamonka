#include <stdio.h>

#include "file.h"

int fileSize(char *filename) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("fopen(%s)\n", filename);
        return -1;
    }

    fseek(f, 0, SEEK_END);
    int size = (int) ftell(f);
    fclose(f);

    return size;
}

int CompressRatio(char* inFileName, char* compressedFileName, float *result) {
    int inSize = fileSize(inFileName);
    if (inSize <= 0) {
        printf("fileSize(%s)\n", inFileName);
        return -1;
    }

    int compressedSize = fileSize(compressedFileName);
    if (compressedSize <= 0) {
        printf("fileSize(%s)\n", compressedFileName);
        return -1;
    }

    *result = (float) inSize / (float) compressedSize;

    return 0;
}
