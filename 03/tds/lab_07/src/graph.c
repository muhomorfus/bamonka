#include <stdlib.h>
#include <limits.h>

#include "graph.h"

graph_t graph_new(size_t size)
{
    graph_t g = { 0 };

    g.n = size;
    g.vertexes = calloc(size, sizeof(graph_relation_t *));

    return g;
}

void free_relation(graph_relation_t *rel)
{
    if (rel)
    {
        graph_relation_t *next = rel;
        for (graph_relation_t *cur = rel; cur; cur = next) {
            next = cur->next;
            free(cur);
        }
    }
}

void graph_free(graph_t *g)
{
    if (g)
    {
        for (size_t i = 0; i < g->n; i++)
            free_relation(g->vertexes[i]);
        free(g);
        g = NULL;
    }
}

graph_t *add_relation(graph_t *g, size_t a, size_t b, int weight)
{
    if (!g)
        return NULL;

    if (a >= g->n || b >= g->n)
        return NULL;

    int added = 0;
    if (!g->vertexes[a])
    {
        g->vertexes[a] = calloc(1, sizeof(graph_relation_t));
        if (!g->vertexes[a])
            return NULL;
        g->vertexes[a]->vertex = b;
        g->vertexes[a]->weight = weight;
        added++;
    }
    if (!g->vertexes[b])
    {
        g->vertexes[b] = calloc(1, sizeof(graph_relation_t));
        if (!g->vertexes[b])
            return NULL;
        g->vertexes[b]->vertex = a;
        g->vertexes[b]->weight = weight;
        added++;
    }
    if (added == 2)
        return g;

    graph_relation_t *last;
    for (last = g->vertexes[a]; last->next; last = last->next)
        if (last->vertex == b || last->next->vertex == b)
            return g;
    last->next = calloc(1, sizeof(graph_relation_t));
    if (!last->next)
        return NULL;
    last->next->vertex = b;
    last->next->weight = weight;
    added++;

    if (added == 2)
        return g;

    for (last = g->vertexes[b]; last->next; last = last->next)
        if (last->vertex == a || last->next->vertex == a)
            return g;
    last->next = calloc(1, sizeof(graph_relation_t));
    if (!last->next)
        return NULL;
    last->next->vertex = a;
    last->next->weight = weight;

    return g;
}

size_t get_min_not_seen(const int *seen, const int *distances, size_t n)
{
    size_t index = -1;
    for (size_t i = 0; i < n; i++)
        if (!seen[i])
            if (index == (size_t) -1 || distances[i] < distances[index])
                index = i;

    return index;
}

int *get_distances(graph_t *g, size_t v)
{
    if (!g)
        return NULL;

    if (v >= g->n)
        return NULL;

    int *distances = calloc(g->n, sizeof(int));
    if (!distances)
        return NULL;

    int *seen = calloc(g->n, sizeof(int));
    if (!seen)
        return NULL;

    for (size_t i = 0; i < g->n; i++)
        distances[i] = INT_MAX;

    distances[v] = 0;
    size_t i = v;
    do {
        for (graph_relation_t *rel = g->vertexes[i]; rel; rel = rel->next)
        {
            if (seen[rel->vertex])
                continue;
            if (distances[i] + rel->weight < distances[rel->vertex])
                distances[rel->vertex] = distances[i] + rel->weight;
        }
        seen[i] = 1;
    } while ((i = get_min_not_seen(seen, distances, g->n)) != (size_t) -1);

    free(seen);
    return distances;
}
