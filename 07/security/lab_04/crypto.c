#include <stdio.h>
#include <memory.h>

#include "crypto.h"
#include "bignum.h"
#include "io.h"
#include "log.h"

#define rotateLeft(bits, word) (((word) << (bits)) | ((word) >> (32-(bits))))

Block padding(Block len) {
    if (len % 64 == 56) {
        len++;
    }
    while ((len % 64) != 56) {
        len++;
    }
    return len * 8;
}

int sha1(Byte *filename, Byte *result) {
    int size;
    Byte *data;
    if (ReadFile((char *) filename, &data, &size)) {
        Errorln("cant ReadFile()");
        return -1;
    }

    Block digest[5];
    Block w[80];
    Block a, b, c, d, e, f;

    unsigned int i, j;
    Byte *buffer;
    Block bits;
    Block temp, k;
    Block lb = size * 8;

    bits = padding(size);
    buffer = (Byte *) malloc((bits / 8) + 8);
    memcpy(buffer, data, size);

    *(buffer + size) = 0x80;
    for (i = size + 1; i < (bits / 8); i++) {
        *(buffer + i) = 0x00;
    }

    *(buffer + (bits / 8) + 4 + 0) = (lb >> 24) & 0xFF;
    *(buffer + (bits / 8) + 4 + 1) = (lb >> 16) & 0xFF;
    *(buffer + (bits / 8) + 4 + 2) = (lb >> 8) & 0xFF;
    *(buffer + (bits / 8) + 4 + 3) = (lb >> 0) & 0xFF;

    digest[0] = 0x67452301;
    digest[1] = 0xEFCDAB89;
    digest[2] = 0x98BADCFE;
    digest[3] = 0x10325476;
    digest[4] = 0xC3D2E1F0;

    for (i = 0; i < ((bits + 64) / 512); i++) {
        for (j = 0; j < 80; j++) {
            w[j] = 0x00;
        }

        for (j = 0; j < 16; j++) {
            w[j] = buffer[j * 4 + 0];
            w[j] = w[j] << 8;
            w[j] |= buffer[j * 4 + 1];
            w[j] = w[j] << 8;
            w[j] |= buffer[j * 4 + 2];
            w[j] = w[j] << 8;
            w[j] |= buffer[j * 4 + 3];
        }

        for (j = 16; j < 80; j++) {
            w[j] = (rotateLeft(1, (w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16])));
        }

        a = digest[0];
        b = digest[1];
        c = digest[2];
        d = digest[3];
        e = digest[4];

        for (j = 0; j < 80; j++) {
            if (j < 20) {
                f = ((b) & (c)) | ((~(b)) & (d));
                k = 0x5A827999;

            } else if (j < 40) {
                f = (b) ^ (c) ^ (d);
                k = 0x6ED9EBA1;
            } else if (j < 60) {
                f = ((b) & (c)) | ((b) & (d)) | ((c) & (d));
                k = 0x8F1BBCDC;
            } else {
                f = (b) ^ (c) ^ (d);
                k = 0xCA62C1D6;
            }

            temp = rotateLeft(5, a) + f + e + k + w[j];
            e = d;
            d = c;
            c = rotateLeft(30, b);
            b = a;
            a = temp;
        }

        digest[0] += a;
        digest[1] += b;
        digest[2] += c;
        digest[3] += d;
        digest[4] += e;

        buffer = buffer + 64;
    }

    memcpy(result, digest, 20);
    return 0;
}

void InfoSHA1ln(char *msg, Byte *p) {
    Info(msg);
    printf(": ");\
    Block *ptr = p;
    for (unsigned int i = 0; i < 5; ++i) {
        printf("%x", ptr[i]);
    }
    printf(".\n");
}

typedef struct {
    bignum *exp;
    bignum *mod;
} rsaKey;

void gcd(bignum *first, bignum *second, bignum *result) {
    bignum *a = bignum_alloc();
    bignum *b = bignum_alloc();
    bignum_copy(first, a);
    bignum_copy(second, b);

    bignum *tmp = bignum_alloc();

    while (!bignum_iszero(b)) {
        bignum_copy(b, tmp);
        bignum_imod(a, b);
        bignum_copy(a, b);
        bignum_copy(tmp, a);
    }

    bignum_copy(a, result);
}

void getD(bignum *a, bignum *mod, bignum *result) {
    bignum *u1 = bignum_alloc();
    bignum *u3 = bignum_alloc();
    bignum *v1 = bignum_alloc();
    bignum *v3 = bignum_alloc();
    bignum *t1 = bignum_alloc();
    bignum *t3 = bignum_alloc();
    bignum *q = bignum_alloc();

    int iter = 1;
    bignum_fromint(u1, 1);
    bignum_copy(a, u3);
    bignum_copy(mod, v3);

    while (!bignum_iszero(v3)) {
        bignum_divide(q, t3, u3, v3);
        bignum_imultiply(q, v1);
        bignum_add(t1, u1, q);

        bignum_copy(v1, u1);
        bignum_copy(t1, v1);
        bignum_copy(v3, u3);
        bignum_copy(t3, v3);

        iter = -iter;
    }

    if (iter < 0)
        bignum_subtract(result, mod, u1);
    else
        bignum_copy(u1, result);
}

void getE(bignum *phi, bignum *result) {
    bignum *x = bignum_alloc();
    Block e = 3;

    while (1) {
        bignum_fromint(result, e);
        gcd(result, phi, x);
        if (x->length == 1 && x->data[0] == 1) { // GCD == 1
            break;
        }

        e++;
    }
}

int isPrime(bignum *n, int repeat) {
    bignum *n_1 = bignum_alloc();
    bignum *a = bignum_alloc();
    bignum *q = bignum_alloc();
    bignum *one = bignum_alloc();
    bignum *x = bignum_alloc();
    Block k = 0;
    Block i;
    int result = 1;

    one->length = 1;
    one->data[0] = 1;

    bignum_subtract(n_1, n, one);
    bignum_copy(n_1, q);
    while (!bignum_isodd(q)) {
        bignum_idivide_2(q);
        k++;
    }

    while (repeat--) {
        bignum_random(10 * n_1->length, a);
        bignum_imod(a, n_1);
        bignum_pow_mod(x, a, q, n);
        if (bignum_isequal(x, one))
            continue;   //a is not well chosen
        i = 0;
        while (!bignum_isequal(x, n_1)) {
            // printf("i=%u\n", i);
            if (i == (k - 1)) {
                return 0;
            } else {
                i++;
                bignum_imultiply(x, x);
                bignum_imod(x, n);
            }
        }
    }

    bignum_free(a);
    bignum_free(q);
    bignum_free(n_1);
    bignum_free(x);
    bignum_free(one);
    return result;
}

void randPrime(int bytes, bignum *result) {
    bignum_random(bytes, result);
    if (!bignum_isodd(result)) {
        result->data[0] += 1;
    }

    for (;;) {
        if (isPrime(result, 100))
            break;
        bignum_iadd_2(result);
    }
}

int readKey(Byte *filename, rsaKey *key) {
    FILE *f;
    if (!(f = fopen(filename, "r"))) {
        return -1;
    }

    key->exp = bignum_alloc();
    key->mod = bignum_alloc();
    if (bignum_read(f, key->mod) != 0) {
        return -1;
    }

    if (bignum_read(f, key->exp) != 0) {
        return -1;
    }

    return 0;
}

int writeRSAKey(Byte *filename, rsaKey *key) {
    FILE *f;
    if (!(f = fopen(filename, "w"))) {
        return -1;
    }

    if (bignum_write(f, key->mod) != 0) {
        return -1;
    }

    if (bignum_write(f, key->exp) != 0) {
        return -1;
    }

    return 0;
}

int MakeKeyPair(Byte *fileName) {
    Byte *privateFileName = fileName;
    Byte publicFileName[100];
    sprintf((char *) publicFileName, "%s.pub", fileName);

    bignum *p = bignum_alloc();
    bignum *q = bignum_alloc();
    bignum *n = bignum_alloc();
    bignum *d = bignum_alloc();
    bignum *e = bignum_alloc();
    bignum *phi = bignum_alloc();

    randPrime(32, p);
    randPrime(32, q);

    bignum_multiply(n, p, q);
    bignum_isubtract(p, &NUMS[1]);
    bignum_isubtract(q, &NUMS[1]);
    bignum_multiply(phi, p, q);

    getE(phi, e);
    getD(e, phi, d);

    rsaKey private = {
        .exp = e,
        .mod = n,
    };

    rsaKey public = {
        .exp = d,
        .mod = n,
    };

    if (writeRSAKey(privateFileName, &private)) {
        return -1;
    }

    if (writeRSAKey(publicFileName, &public)) {
        return -1;
    }

    return 0;
}

int rsaInFile(const Byte *buf, int size, rsaKey *key, Byte *result) {
    bignum *res, *plain;
    int resSize;

    plain = bignum_from_buf(buf, size);
    res = bignum_alloc();
    bignum_pow_mod(res, plain, key->exp, key->mod);

    resSize = res->length * sizeof(Block);

    for (int i = 0; i < resSize; i++)
        result[i] = ((Byte *) res->data)[i];

    return resSize;
}

int rsa(const Byte *buf, int bytes, Byte *keyFileName, Byte *result) {
    rsaKey key;
    if (readKey(keyFileName, &key)) {
        Errorln("cant readKey()");
        return -1;
    }

    return rsaInFile(buf, bytes, &key, result);
}

int Sign(Byte *filename, Byte *keyFileName) {
    char signFileName[512];
    sprintf(signFileName, "%s.signed", filename);

    Byte hash[512] = {0};
    if (sha1(filename, hash)) {
        Errorln("cant sha1()");
        return -1;
    }

    Byte sign_content[512] = {0};
    int size;
    if ((size = rsa(hash, 20, keyFileName, sign_content)) < 0) {
        Errorln("cant rsa()");
        return -1;
    }

    if (WriteFile(signFileName, sign_content, size)) {
        Errorln("cant WriteFile()");
        return -1;
    }

    return 0;
}

int Check(Byte *filename, Byte *keyFileName) {
    char signFileName[512];
    sprintf(signFileName, "%s.signed", filename);

    Byte hash[512] = {0};
    if (sha1(filename, hash) != 0) {
        Errorln("cant sha1()");
        return -1;
    }
    InfoSHA1ln("SHA1 of file", hash);

    Byte *signData;
    int signSize;
    if (ReadFile(signFileName, &signData, &signSize)) {
        Errorln("cant ReadFile()");
        return -1;
    }

    Byte toCompare[512] = {0};
    if (rsa(signData, signSize, keyFileName, toCompare) < 0) {
        Errorln("cant rsa()");
        return -1;
    }
    InfoSHA1ln("SHA1 from signature", toCompare);

    for (int i = 0; i < 20; i++) {
        if (toCompare[i] != hash[i]) {
            Errorln("file is broken");
            return -1;
        }
    }

    Infoln("file is valid");
    return 0;
}
