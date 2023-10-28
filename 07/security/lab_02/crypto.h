#ifndef LAB_02_CRYPTO_H
#define LAB_02_CRYPTO_H

#include <stdint.h>

typedef uint64_t Block;

unsigned char* EncryptPCBC(unsigned char *buf, Block key, Block iv, int len, int *newLen);
unsigned char* DecryptPCBC(unsigned char *buf, Block iv, Block key, int len, int *newLen);

#endif //LAB_02_CRYPTO_H
