#ifndef LAB_04_CRYPTO_H
#define LAB_04_CRYPTO_H

#include <stdint.h>

typedef uint8_t Byte;
typedef uint32_t Block;

int Sign(Byte *filename, Byte *keyFileName);
int Check(Byte *filename, Byte *keyFileName);
int MakeKeyPair(Byte *fileName);

#endif //LAB_04_CRYPTO_H
