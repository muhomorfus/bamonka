#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <memory.h>

#include "crypto.h"
#include "galois.h"

void subBytes(Byte *state) {
    for (Byte i = 0; i < WordSize; i++) {
        for (Byte j = 0; j < NB; j++) {
            state[NB * i + j] = sTable[state[NB * i + j]];
        }
    }
}

void shiftRows(Byte *state) {
    Byte s, tmp;
    for (Byte i = 1; i < WordSize; i++) {
        s = 0;
        while (s < i) {
            tmp = state[NB * i + 0];
            for (Byte k = 1; k < NB; k++) {
                state[NB * i + k - 1] = state[NB * i + k];
            }
            state[NB * i + NB - 1] = tmp;
            s++;
        }
    }
}

void matrixMul(const Byte *matrix, const Byte *column, Byte *res) {
    res[0] = galoisMul(matrix[0], column[0]) ^ galoisMul(matrix[3], column[1]) ^ galoisMul(matrix[2], column[2]) ^ galoisMul(matrix[1], column[3]);
    res[1] = galoisMul(matrix[1], column[0]) ^ galoisMul(matrix[0], column[1]) ^ galoisMul(matrix[3], column[2]) ^ galoisMul(matrix[2], column[3]);
    res[2] = galoisMul(matrix[2], column[0]) ^ galoisMul(matrix[1], column[1]) ^ galoisMul(matrix[0], column[2]) ^ galoisMul(matrix[3], column[3]);
    res[3] = galoisMul(matrix[3], column[0]) ^ galoisMul(matrix[2], column[1]) ^ galoisMul(matrix[1], column[2]) ^ galoisMul(matrix[0], column[3]);
}

void mixColumns(Byte *state) {
    Byte matrix[] = {0x02, 0x01, 0x01, 0x03};

    Byte column[WordSize], res[WordSize];

    for (Byte j = 0; j < NB; j++) {
        for (Byte i = 0; i < WordSize; i++) {
            column[i] = state[NB * i + j];
        }

        matrixMul(matrix, column, res);

        for (Byte i = 0; i < WordSize; i++) {
            state[NB * i + j] = res[i];
        }
    }
}

void addRoundKey(Byte *state, const Byte *expanded_key, Byte round_num) {
    for (Byte c = 0; c < NB; c++) {
        state[NB * 0 + c] = state[NB * 0 + c] ^ expanded_key[WordSize * NB * round_num + WordSize * c + 0];
        state[NB * 1 + c] = state[NB * 1 + c] ^ expanded_key[WordSize * NB * round_num + WordSize * c + 1];
        state[NB * 2 + c] = state[NB * 2 + c] ^ expanded_key[WordSize * NB * round_num + WordSize * c + 2];
        state[NB * 3 + c] = state[NB * 3 + c] ^ expanded_key[WordSize * NB * round_num + WordSize * c + 3];
    }
}

void subWord(Byte *key) {
    for (Byte i = 0; i < WordSize; i++) {
        key[i] = sTable[key[i]];
    }
}

void rotWord(Byte *key) {
    Byte tmp;
    tmp = key[0];
    for (Byte i = 0; i < 3; i++) {
        key[i] = key[i + 1];
    }
    key[3] = tmp;
}

void workXor(const Byte *a, const Byte *b, Byte *result) {
    result[0] = a[0] ^ b[0];
    result[1] = a[1] ^ b[1];
    result[2] = a[2] ^ b[2];
    result[3] = a[3] ^ b[3];
}

Byte rConByte(Byte i) {
    if (i == 1) {
        return 1;
    }

    if (i > 1) {
        int n = 2;
        i--;
        while (i > 1) {
            n = galoisMul(n, 2);
            i--;
        }
        return n;
    }

    return 0;
}

void expandKey(const Byte *key, Byte *out) {
    for (Byte i = 0; i < NK; i++) {
        out[WordSize * i + 0] = key[WordSize * i + 0];
        out[WordSize * i + 1] = key[WordSize * i + 1];
        out[WordSize * i + 2] = key[WordSize * i + 2];
        out[WordSize * i + 3] = key[WordSize * i + 3];
    }

    for (int i = NK; i < NB * (NR + 1); i++) {
        Byte tmp[WordSize];

        tmp[0] = out[WordSize * (i - 1) + 0];
        tmp[1] = out[WordSize * (i - 1) + 1];
        tmp[2] = out[WordSize * (i - 1) + 2];
        tmp[3] = out[WordSize * (i - 1) + 3];
        if (i % NK == 0) {
            rotWord(tmp);
            subWord(tmp);

            Byte rCon[4] = {rConByte(i / NK), 0, 0, 0};

            workXor(tmp, rCon, tmp);
        } else if (NK > 6 && i % NK == WordSize) {
            subWord(tmp);
        }

        out[WordSize * i + 0] = out[WordSize * (i - NK) + 0] ^ tmp[0];
        out[WordSize * i + 1] = out[WordSize * (i - NK) + 1] ^ tmp[1];
        out[WordSize * i + 2] = out[WordSize * (i - NK) + 2] ^ tmp[2];
        out[WordSize * i + 3] = out[WordSize * (i - NK) + 3] ^ tmp[3];
    }
}

void toState(const Byte *block, Byte *state) {
    for (Byte i = 0; i < WordSize; i++) {
        for (Byte j = 0; j < NB; j++) {
            state[NB * i + j] = block[i + WordSize * j];
        }
    }
}

void fromState(const Byte *state, Byte *block) {
    for (Byte i = 0; i < WordSize; i++) {
        for (Byte j = 0; j < NB; j++) {
            block[i + WordSize * j] = state[NB * i + j];
        }
    }
}

void aes(const Byte *buf, Byte *result, Byte *key) {
    Byte state[WordSize * NB];
    toState(buf, state);

    Byte expanded_key[WordSize * NB * (NR + 1)];
    expandKey(key, expanded_key);
    addRoundKey(state, expanded_key, 0);

    for (Byte r = 1; r < NR; r++) {
        subBytes(state);
        shiftRows(state);
        mixColumns(state);
        addRoundKey(state, expanded_key, r);
    }

    subBytes(state);
    shiftRows(state);
    addRoundKey(state, expanded_key, NR);

    fromState(state, result);
}

void ofb(Byte *data, int num_blocks, Byte *iv, Byte *key) {
    for (int i = 0; i < num_blocks; i++) {
        aes(iv, iv, key);

        for (int j = 0; j < BlockSize; j++) {
            data[BlockSize * i + j] = data[BlockSize * i + j] ^ iv[j];
        }
    }
}

Byte *sizeUp(Byte *buf, int len, int *blocksCount) {
    int uppedLen = len - len % BlockSize + BlockSize;
    *blocksCount = uppedLen / BlockSize;

    Byte *b = realloc(buf, uppedLen * sizeof(char));
    if (b == NULL) {
        printf("realloc()\n");
        return NULL;
    }

    b[len] = Divider;
    for (int i = len + 1; i < uppedLen; i++) {
        b[i] = 0;
    }

    return b;
}

int EncryptOFB(Byte **buf, int len, Byte *key, Byte *iv, int *newLen) {
    int blocksCount;
    Byte *b = sizeUp(*buf, len, &blocksCount);
    if (b == NULL) {
        printf("sizeUp()\n");
        return 1;
    }

    *newLen = blocksCount * BlockSize;
    ofb(b, blocksCount, iv, key);
    *buf = b;

    return 0;
}

int DecryptOFB(Byte *buf, int len, Byte *key, Byte *iv, int *newLen) {
    ofb(buf, len / BlockSize, iv, key);

    *newLen = len;
    while (buf[*newLen] != Divider) {
        (*newLen)--;
    }
    buf[*newLen] = 0;

    return 0;
}

