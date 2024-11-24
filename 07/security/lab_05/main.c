#include <stdio.h>
#include <string.h>

#include "file.h"
#include "huffman.h"

int compressCmd(int argc, char **argv) {
    if (argc < 3) {
        printf("argc must be 3\n");
        return -1;
    }

    char *filenameIn = argv[2];
    char filenameOut[100];
    sprintf(filenameOut, "%s.huffman", argv[2]);

    if (HuffmanCompress(filenameIn, filenameOut) != 0) {
        printf("HuffmanCompress()\n");
        return -1;
    }

    float ratio;
    if (CompressRatio(filenameIn, filenameOut, &ratio) != 0) {
        printf("CompressRatio()\n");
        return -1;
    }

    printf("Compressed!\n");
    printf("Compress ratio = %.2f.\n", ratio);

    return 0;
}

int decompressCmd(int argc, char **argv) {
    if (argc < 4) {
        printf("argc must be 4\n");
        return -1;
    }

    if (HuffmanDecompress(argv[2], argv[3]) != 0) {
        printf("HuffmanDecompress()\n");
        return -1;
    }

    printf("Decompressed!\n");

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("argc must be at least 2\n");
        return -1;
    }

    if (strcmp(argv[1], "c") == 0) {
        return compressCmd(argc, argv);
    }

    if (strcmp(argv[1], "d") == 0) {
        return decompressCmd(argc, argv);
    }

    printf("invalid subcommand\n");
    return -1;
}
