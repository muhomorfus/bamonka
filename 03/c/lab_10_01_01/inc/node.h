#ifndef NODE_H
#define NODE_H

typedef struct node node_t;
struct node
{
    void *data;
    node_t *next;
};

void nodes_free(node_t **head);

void *pop_front(node_t **head);
void *pop_back(node_t **head);
void remove_duplicates(node_t **head, int (*comparator)(const void*, const void*));
node_t *sort(node_t *head, int (*comparator)(const void *, const void *));
void sorted_insert(node_t **head, node_t *element, int (*comparator)(const void *, const void *));

#endif //NODE_H
