#include <stdio.h>
#include <stdlib.h>

#include "crypto.h"
#include "io.h"

int main() {
    char source[100];
    char destination[100];
    int mode = 0;
    Block key = 0, iv = 0;

    printf("Mode = 0 => encrypt, else => decrypt.\n");
    printf("Type dividing by newlines (mode, source, destination, key, iv):\n\n");

    if (scanf("%d %s %s %llu %llu", &mode, source, destination, &key, &iv) != 5) {
        printf("scanf()\n");
        return 1;
    }

    unsigned char *buf;
    int size;
    if (ReadFile(source, &buf, &size) != 0) {
        printf("ReadFile()\n");
        return 1;
    }

    int newLen = 0;
    unsigned char *result = NULL;
    if (mode == 0) {
        result = EncryptPCBC(buf, key, iv, size, &newLen);
        if (result == NULL) {
            free(buf);
            printf("EncryptPCBC()\n");
            return 1;
        }
    } else {
        result = DecryptPCBC(buf, iv, key, size, &newLen);
        if (result == NULL) {
            free(buf);
            printf("DecryptPCBC()\n");
            return 1;
        }
    }

    if (WriteFile(destination, result, newLen) != 0) {
        free(buf);
        free(result);
        printf("WriteFile()\n");
        return 1;
    }

    free(buf);
    free(result);

    return 0;
}
