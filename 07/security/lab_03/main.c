#include <stdio.h>
#include <stdlib.h>

#include "crypto.h"
#include "io.h"

int main() {
    char source[100];
    char destination[100];
    int mode = 0;
    uint8_t key[KeySize + 1];
    uint8_t iv[BlockSize + 1];

    printf("Mode = 0 => encrypt, else => decrypt.\n");
    printf("Type dividing by newlines (mode, source, destination, key, iv):\n\n");

    if (scanf("%d %s %s %s %s", &mode, source, destination, &key, &iv) != 5) {
        printf("scanf()\n");
        return 1;
    }

    uint8_t *buf;
    int size;
    if (ReadFile(source, &buf, &size) != 0) {
        printf("ReadFile()\n");
        return 1;
    }

    int newLen = 0;
    if (mode == 0) {
        if (EncryptOFB(&buf, size, key, iv, &newLen) != 0) {
            free(buf);
            printf("EncryptOFB()\n");
            return 1;
        }
    } else {
        if (DecryptOFB(buf, size, key, iv, &newLen) != 0) {
            free(buf);
            printf("DecryptOFB()\n");
            return 1;
        }
    }

    if (WriteFile(destination, buf, newLen) != 0) {
        free(buf);
        printf("WriteFile()\n");
        return 1;
    }

    free(buf);

    return 0;
}
