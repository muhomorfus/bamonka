#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>

typedef struct graph_relation_t graph_relation_t;
struct graph_relation_t
{
    size_t vertex;
    int weight;
    graph_relation_t *next;
};

typedef struct graph_t
{
    graph_relation_t **vertexes;
    size_t n;
} graph_t;

graph_t graph_new(size_t size);
void graph_free(graph_t *g);
graph_t *add_relation(graph_t *g, size_t a, size_t b, int weight);
int *get_distances(graph_t *g, size_t v);

#endif //GRAPH_H
