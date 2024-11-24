#ifndef LAB_05_TREE_H
#define LAB_05_TREE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MaxCode 32

typedef struct Node {
    uint8_t data;
    int freq;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    int size;
    int capacity;
    Node **nodes;
} Tree;

typedef struct {
    uint8_t carr[MaxCode];
    int size;
} HuffmanCode;

void initNode(Node *n);

Node *newNode(uint8_t c, int f);

Tree *makeTree(uint8_t *character, int *freq, int size);

void deleteTree(Tree *t);

Node *getMin(Tree *t);

void swap(Node **a, Node **b);

void sort(Tree *t, int i);

void insertNode(Tree *t, Node *n);

Tree *buildHuffmanTree(uint8_t *character, int *freq, int size);

int getBit(const uint8_t *c, int n);

#endif //LAB_05_TREE_H
