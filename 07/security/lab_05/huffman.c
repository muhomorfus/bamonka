#include <stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

#include "huffman.h"
#include "tree.h"

#define maxCodeValue 256

void saveBit(uint8_t *c, int n, int value) {
    uint8_t mask = 0x80;
    mask = mask >> n;

    if (value) {
        *c = *c | mask;
    } else {
        *c = *c & ~mask;
    }
}

int getBit(const uint8_t *c, int n) {
    uint8_t mask = 0x80;
    mask = mask >> n;
    return *c & mask;
}

void getCodeArr(int arr[], HuffmanCode *codes, int n) {
    for (int i = 0; i < n; ++i) {
        int ndx = i / 8;
        int nBit = i % 8;

        saveBit(&(codes->carr[ndx]), nBit, arr[i]);
        codes->size++;
    }
}

void getCodes(Node *root, int arr[], HuffmanCode codes[], int top) {
    if (root->left) {
        arr[top] = 0;
        getCodes(root->left, arr, codes, top + 1);
    }

    if (root->right) {
        arr[top] = 1;
        getCodes(root->right, arr, codes, top + 1);
    }

    if (root->left == root->right) {
        getCodeArr(arr, &codes[root->data], top);
    }
}

void saveLastByteBits(FILE *f, uint8_t value) {
    fwrite(&value, 1, 1, f);
}

int saveCodes(char *filename, HuffmanCode codes[], int freq[], int maxsize) {
    FILE *f = fopen(filename, "wb");
    if (f == NULL) {
        return -1;
    }

    saveLastByteBits(f, 8);

    for (int i = 0; i < maxsize; i++) {
        if (codes[i].size != 0) {
            fwrite(&i, 1, 1, f);
            fwrite(&(freq[i]), sizeof(int), 1, f);
        }
    }

    fwrite("", 1, 1, f);
    int i = 0;
    fwrite(&i, sizeof(int), 1, f);
    fclose(f);
    return 0;
}

void encodeData(char *infile, char *outfile, HuffmanCode codes[], int maxsize) {
    FILE *fIn = fopen(infile, "rb");
    FILE *fOut = fopen(outfile, "ab+");

    int wBitCnt = 0;
    uint8_t wTmp = 0;

    while (!feof(fIn)) {
        uint8_t rTmp = 0;
        if (fread(&rTmp, 1, 1, fIn)) {
            int byteCnt = 0;
            int rBitCnt = 0;
            for (int i = 0; i < codes[rTmp].size; i++) {
                int value = getBit(&(codes[rTmp].carr[byteCnt]), rBitCnt++);
                if (rBitCnt == 8) {
                    rBitCnt = 0;
                    byteCnt++;
                }
                saveBit(&wTmp, wBitCnt++, value);
                if (wBitCnt == 8) {
                    wBitCnt = 0;
                    fwrite(&wTmp, 1, 1, fOut);
                }
            }
        }
    }
    if (wBitCnt != 0) {
        fwrite(&wTmp, 1, 1, fOut);
    }

    fclose(fIn);
    fclose(fOut);

    FILE *fp = fopen(outfile, "rb+");
    if (fp != NULL) {
        saveLastByteBits(fp, wBitCnt);
        fclose(fp);
    }
}

int HuffmanCompress(char *filenameIn, char *filenameOut) {
    fclose(fopen(filenameOut, "wb"));

    FILE *f = fopen(filenameIn, "rb");
    if (f == NULL) {
        return -1;
    }
    uint8_t character[maxCodeValue] = {0};
    int freq[maxCodeValue] = {0};
    uint8_t temp = '\0';
    int empty = 1;
    while (!feof(f)) {
        if (fread(&temp, 1, 1, f)) {
            character[temp] = temp;
            freq[temp]++;
            empty = 0;
        }
    }

    if (empty) {
        fclose(f);
        return 0;
    }

    Tree *t = buildHuffmanTree(character, freq, maxCodeValue);

    int arr[MaxCode] = {0};
    int top = 0;
    HuffmanCode codes[maxCodeValue] = {0};
    getCodes(t->nodes[0], arr, codes, top);

    saveCodes(filenameOut, codes, freq, maxCodeValue);
    encodeData(filenameIn, filenameOut, codes, maxCodeValue);

    deleteTree(t);

    fclose(f);

    return 0;
}

int readCodes(FILE *fp, HuffmanCode codes[], int freq[], int maxsize, int *lastBitUsed) {
    uint8_t character = '\0';
    fread(lastBitUsed, 1, 1, fp);

    while (!feof(fp)) {
        if (fread(&character, 1, 1, fp)) {
            int f = 0;
            if (fread(&f, sizeof(int), 1, fp)) {
                if (character == 0 && f == 0) break;
                freq[character] = f;
            } else {
                fclose(fp);
                return -1;
            }
        }
    }
    return 0;
}

void truncateFile(char *filename, int toReduce) {
    FILE *fp = fopen(filename, "r");
    fseek(fp, 0, SEEK_END);
    int size = (int) ftell(fp);
    fclose(fp);

    size = size - toReduce;
    truncate(filename, size);
}

void decodeData(FILE *fIn, char *filenameOut, Tree *t, HuffmanCode codes[], int maxsize, int lastUsedBits) {
    FILE *fOut = fopen(filenameOut, "wb+");

    Node *root = t->nodes[0];
    Node *currentNode = root;
    int possibleExtraByte = 0;
    int numExtraBytesWritten = 0;

    while (!feof(fIn)) {
        uint8_t rTmp = 0;

        if (fread(&rTmp, 1, 1, fIn)) {
            possibleExtraByte = 0;
            numExtraBytesWritten = 0;
            for (int i = 0; i < 8; i++) {
                int value = getBit(&rTmp, i);
                if (value != 0) {
                    currentNode = currentNode->right;
                } else {
                    currentNode = currentNode->left;
                }
                if (currentNode->left == NULL && currentNode->right == NULL) {
                    fwrite(&currentNode->data, 1, 1, fOut);
                    if (i == (lastUsedBits - 1)) {
                        possibleExtraByte = 1;
                    }
                    if (possibleExtraByte) {
                        numExtraBytesWritten++;
                    }

                    currentNode = root;
                }
            }
        }
    }

    fclose(fIn);
    fclose(fOut);

    if (numExtraBytesWritten != 0)
        truncateFile(filenameOut, numExtraBytesWritten - 1);
}

int HuffmanDecompress(char *filenameIn, char *filenameOut) {
    FILE *f;
    f = fopen(filenameIn, "rb");
    if (f == NULL) {
        printf("fopen()\n");
        return -1;
    }
    HuffmanCode codes[maxCodeValue] = {0};
    int freq[maxCodeValue] = {0};
    int lastUsedBits = 0;

    if (readCodes(f, codes, freq, maxCodeValue, &lastUsedBits) != 0) {
        printf("readCodes()\n");
        return -1;
    }

    uint8_t character[maxCodeValue] = {0};
    for (int i = 0; i < maxCodeValue; i++) {
        character[i] = i;
    }
    Tree *t = buildHuffmanTree(character, freq, maxCodeValue);

    decodeData(f, filenameOut, t, codes, maxCodeValue, lastUsedBits);

    deleteTree(t);
    fclose(f);

    return 0;
}
