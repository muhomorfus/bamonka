#include "tree.h"

void initNode(Node *n) {
    n->data = '\0';
    n->freq = 0;
    n->left = NULL;
    n->right = NULL;
}

Node *newNode(uint8_t c, int f) {
    Node *temp = (Node *) malloc(sizeof(Node));
    if (temp == NULL) return temp;
    initNode(temp);
    temp->data = c;
    temp->freq = f;
    return temp;
}

Tree *makeTree(uint8_t *character, int *freq, int size) {
    Tree *t = (Tree *) malloc(sizeof(Tree));
    t->size = 0;
    t->capacity = size;

    t->nodes = (Node **) malloc(size * sizeof(Node *));
    for (int ndx = 0; ndx < size; ndx++) {
        if (freq[ndx] != 0) {
            Node *tmp = newNode(character[ndx], freq[ndx]);
            t->nodes[t->size] = tmp;
            t->size++;
        }
    }
    return t;
}

void deleteNode(Node *n) {
    if (n == NULL) {
        return;
    }

    if (n->left != NULL) {
        deleteNode(n->left);
        n->left = NULL;
    }

    if (n->right != NULL) {
        deleteNode(n->right);
        n->right = NULL;
    }

    free(n);
}

void deleteTree(Tree *t) {
    int i = 0;
    while (i < t->size) {
        deleteNode(t->nodes[i]);
        i++;
    }
    free(t->nodes);
    free(t);
}

Node *getMin(Tree *t) {
    sort(t, 0);
    Node *temp = t->nodes[0];
    t->nodes[0] = t->nodes[t->size - 1];
    --(t->size);
    return temp;
}

void sort(Tree *t, int i) {
    int min = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < t->size && t->nodes[left]->freq < t->nodes[min]->freq)
        min = left;

    if (right < t->size && t->nodes[right]->freq < t->nodes[min]->freq)
        min = right;

    if (min != i) {
        swap(&t->nodes[min], &t->nodes[i]);
        sort(t, min);
    }
}

void swap(Node **a, Node **b) {
    Node *t = *a;
    *a = *b;
    *b = t;
}

void insertNode(Tree *t, Node *n) {
    ++(t->size);
    int i = t->size - 1;

    while (i && n->freq < t->nodes[(i - 1) / 2]->freq) {
        t->nodes[i] = t->nodes[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    t->nodes[i] = n;
}

Tree *buildHuffmanTree(uint8_t *character, int *freq, int size) {
    Tree *t = makeTree(character, freq, size);
    Node *left, *right, *top;
    while (t->size != 1) {
        left = getMin(t);
        right = getMin(t);

        top = newNode('\0', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertNode(t, top);
    }

    return t;
}

