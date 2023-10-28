#include <stdlib.h>
#include <stdio.h>

#include "crypto.h"

static int ipTable[] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7,
};

static int ipReversedTable[] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25,
};

static int gTable[] = {
    57, 49, 41, 33, 25, 17, 9, 1,
    58, 50, 42, 34, 26, 18, 10, 2,
    59, 51, 43, 35, 27, 19, 11, 3,
    60, 52, 44, 36, 63, 55, 47, 39,
    31, 23, 15, 7, 62, 54, 46, 38,
    30, 22, 14, 6, 61, 53, 45, 37,
    29, 21, 13, 5, 28, 20, 12, 4,
};

static int hTable[] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32,
};

static int eTable[] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1,
};

static int pTable[] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25,
};

Block mask(int n) {
    return ((Block) 1 << n) - 1;
}

Block bit(Block b, int n) {
    return (b >> n) & 1;
}

Block getDummy(Block b, int n, int size) {
    return (b >> (size - n)) & 1;
}

Block setDummy(Block b, Block bit, int n, int size) {
    return b | (bit << (size - n));
}

static int shifts[] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1,
};

static Block sTable[][4][16] = {
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},
    },
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},
    },
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},
    },
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},
    },
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},
    },
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13},
    },
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},
    },
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11},
    },
};

Block IP(Block block) {
    Block result = 0;

    for (int i = 1; i <= 64; i++) {
        result = setDummy(result, getDummy(block, ipTable[i - 1], 64), i, 64);
    }

    return result;
}

Block IPReversed(Block block) {
    Block result = 0;

    for (int i = 1; i <= 64; i++) {
        result = setDummy(result, getDummy(block, ipReversedTable[i - 1], 64), i, 64);
    }

    return result;
}

Block G(Block key) {
    Block result = 0;

    for (int i = 1; i <= 56; i++) {
        result = setDummy(result, getDummy(key, gTable[i - 1], 64), i, 56);
    }

    return result;
}

Block H(Block key) {
    Block result = 0;

    for (int i = 1; i <= 48; i++) {
        result = setDummy(result, getDummy(key, hTable[i - 1], 56), i, 48);
    }

    return result;
}

Block P(Block r) {
    Block result = 0;

    for (int i = 1; i <= 32; i++) {
        result = setDummy(result, getDummy(r, pTable[i - 1], 32), i, 32);
    }

    return result;
}

Block E(Block r) {
    Block result = 0;

    for (int i = 1; i <= 48; i++) {
        result = setDummy(result, getDummy(r, eTable[i - 1], 32), i, 48);
    }

    return result;
}

Block f(Block r, Block k) {
    r = E(r) ^ k;

    Block b[8];
    for (int i = 8; i > 0; i--) {
        b[i - 1] = r & mask(6);
        r >>= 6;
    }

    for (int i = 8; i > 0; i--) {
        b[i - 1] = sTable[i - 1][(bit(b[i - 1], 5) << 1 | bit(b[i - 1], 0))][((b[i - 1] >> 1) & mask(4))];
    }

    r = 0;
    for (int i = 1; i <= 8; i++) {
        r <<= 4;
        r |= b[i - 1];
    }

    return P(r);
}

Block shift(Block block, int size, int shift) {
    shift %= size;

    Block k = block >> (size - shift);
    block <<= shift;

    block &= mask(size);
    block |= k;

    return block;
}

void generateKeys(Block k[17], Block key) {
    k[0] = G(key);

    Block c[17], d[17];
    c[0] = (k[0] >> 28) & mask(28);
    d[0] = k[0] & mask(28);

    for (int i = 1; i <= 16; i++) {
        c[i] = shift(c[i - 1], 28, shifts[i - 1]);
        d[i] = shift(d[i - 1], 28, shifts[i - 1]);

        k[i] = H(d[i] | (c[i] << 28));
    }
}

Block encryptDES(Block t, Block key) {
    Block t0 = IP(t);

    Block r[17], l[17];
    r[0] = t0 & mask(32);
    l[0] = (t0 >> 32) & mask(32);

    Block k[17];
    generateKeys(k, key);

    for (int i = 1; i <= 16; i++) {
        l[i] = r[i-1];
        r[i] = l[i-1] ^ f(r[i-1], k[i]);
    }

    Block t16 = l[16] | (r[16] << 32);

    return IPReversed(t16);
}

Block decryptDES(Block t, Block key) {
    Block t16 = IP(t);

    Block r[17], l[17];
    l[16] = t16 & mask(32);
    r[16] = (t16 >> 32) & mask(32);

    Block k[17];
    generateKeys(k, key);

    for (int i = 16; i > 0; i--) {
        r[i-1] = l[i];
        l[i-1] = r[i] ^ f(l[i], k[i]);
    }

    Block t0 = r[0] | (l[0] << 32);

    return IPReversed(t0);
}

Block* encryptPCBC(Block* blocks, int len, Block key, Block iv) {
    Block* result = calloc(len, sizeof(Block));
    if (result == NULL) {
        printf("calloc()\n");
        return NULL;
    }

    result[0] = encryptDES(blocks[0] ^ iv, key);
    for (int i = 1; i < len; i++) {
        result[i] = encryptDES(result[i - 1] ^ blocks[i - 1] ^ blocks[i], key);
    }

    return result;
}

Block* decryptPCBC(Block* blocks, int len, Block key, Block iv) {
    Block* result = calloc(len, sizeof(Block));
    if (result == NULL) {
        printf("calloc()\n");
        return NULL;
    }

    result[0] = decryptDES(blocks[0], key) ^ iv;
    for (int i = 1; i < len; i++) {
        result[i] = decryptDES(blocks[i], key) ^ result[i - 1] ^ blocks[i - 1];
    }

    return result;
}

void charsToBlocks(Block *blocks, unsigned char* buf, int len) {
    int idx = 0;
    for (int i = 0; i < len; i += 8) {
        Block block = 0;
        for (int j = i; j < i + 8; j++) {
            block <<= 8;
            block |= buf[j];
        }

        blocks[idx] = block;
        idx++;
    }
}

void blocksToChars(Block *blocks, unsigned char* buf, int len) {
    int idx = 0;
    for (int i = 0; i < len; i += 8) {
        Block block = blocks[idx];
        for (int j = i + 8 - 1; j >= i; j--) {
            buf[j] = (char) block;
            block >>= 8;
        }

        idx++;
    }
}

Block* charsToBlocksPadding(unsigned char* buf, int len, int *blocksLen) {
    int newLen = len - len % 8 + 8;

    unsigned char* realloced = realloc(buf, newLen * sizeof(char));
    if (buf == NULL) {
        printf("realloc()\n");
        return NULL;
    }
    buf = realloced;

    buf[len] = 0x80;
    for (int i = len + 1; i < newLen; i++) {
        buf[i] = 0;
    }

    *blocksLen = newLen / 8;
    Block* blocks = calloc(*blocksLen, sizeof(Block));
    if (blocks == NULL) {
        printf("calloc()\n");
        return NULL;
    }

    charsToBlocks(blocks, buf, newLen);

    return blocks;
}

unsigned char* EncryptPCBC(unsigned char *buf, Block key, Block iv, int len, int *newLen) {
    int blocksLen = 0;
    Block* blocks = charsToBlocksPadding(buf, len, &blocksLen);
    if (blocks == NULL) {
        printf("charsToBlocksPadding()\n");
        return NULL;
    }

    Block* encrypted = encryptPCBC(blocks, blocksLen, key, iv);
    if (encrypted == NULL) {
        free(blocks);
        printf("encryptPCBC()\n");
        return NULL;
    }

    *newLen = blocksLen * 8;
    unsigned char* result = calloc(*newLen, sizeof(unsigned char));
    if (result == NULL) {
        free(blocks);
        free(encrypted);
        printf("calloc()\n");
        return NULL;
    }

    blocksToChars(encrypted, result, blocksLen * 8);
    return result;
}

unsigned char* DecryptPCBC(unsigned char *buf, Block iv, Block key, int len, int *newLen) {
    int blocksLen = len / 8;
    Block* blocks = calloc(blocksLen, sizeof(Block));
    if (blocks == NULL) {
        printf("calloc()\n");
        return NULL;
    }

    charsToBlocks(blocks, buf, len);

    Block* decrypted = decryptPCBC(blocks, blocksLen, key, iv);
    if (decrypted == NULL) {
        free(blocks);
        printf("decryptPCBC()\n");
        return NULL;
    }

    *newLen = (blocksLen) * 8;
    unsigned char* result = calloc(*newLen, sizeof(unsigned char));
    if (result == NULL) {
        free(blocks);
        free(decrypted);
        printf("calloc()\n");
        return NULL;
    }

    blocksToChars(decrypted, result, *newLen);

    while (result[*newLen] != 0x80) {
        (*newLen)--;
    }

    result[*newLen] = 0;

    return result;
}