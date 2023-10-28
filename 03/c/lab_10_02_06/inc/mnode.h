#ifndef MNODE_H
#define MNODE_H

#define MLIST_ARGS_ERROR 100
#define MLIST_FIND_ERROR 101

typedef struct mnode mnode_t;
struct mnode
{
    size_t i;
    size_t j;
    int n;
    mnode_t *next;
};

void mlist_free(mnode_t **head);
mnode_t *mlist_append(mnode_t **head, mnode_t **last, size_t i, size_t j, int n);
mnode_t *mlist_from_file(FILE *f);
void mlist_to_file(FILE *f, mnode_t *head);
mnode_t *mlist_add(mnode_t *a, mnode_t *b);
int mlist_del_row_max(mnode_t **head);
mnode_t *mlist_mul(mnode_t *a, mnode_t *b);

#endif //MNODE_H
