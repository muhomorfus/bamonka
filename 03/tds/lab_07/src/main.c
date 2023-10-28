#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "mystring.h"

#define READ_ERROR 1
#define FIRST_READ_ERROR 2
#define ARG_ERROR 3
#define ALLOC_ERROR 4
#define ADD_ERROR 5
#define OPEN_ERROR 6

size_t get_set_unique_string(mystring_t **arr, mystring_t str, size_t *n)
{
    if (!n || !arr)
        return -1;

    for (size_t i = 0; i < *n; i++)
        if (!strcmp(str, (*arr)[i]))
            return i;

    mystring_t *buf = realloc(*arr, ++(*n));
    if (!buf)
        return -1;

    *arr = buf;
    strcpy((*arr)[(*n) - 1], str);
    return (*n) - 1;
}

int input_relation(mystring_t a, mystring_t b, int *distance, FILE *f)
{
    if (fscanf(f, "%s%s%d", a, b, distance) != 3)
        return READ_ERROR;

    return EXIT_SUCCESS;
}

int input_names(mystring_t **names, size_t *n, FILE *f)
{
    rewind(f);
    mystring_t a, b;
    int distance;
    while (!input_relation(a, b, &distance, f))
    {
        if (get_set_unique_string(names, a, n) == (size_t) -1)
            return READ_ERROR;

        if (get_set_unique_string(names, b, n) == (size_t) -1)
            return READ_ERROR;
    }

    rewind(f);
    return EXIT_SUCCESS;
}

int graph_from_file(graph_t *g, mystring_t **names, FILE *f)
{
    if (!g || !names || !f)
        return ARG_ERROR;

    size_t n = 0;
    int err = input_names(names, &n, f);
    if (err)
        return err;

    *g = graph_new(n);
    if (!g->vertexes)
        return ALLOC_ERROR;

    mystring_t a, b;
    int distance;
    while (!input_relation(a, b, &distance, f))
    {
        size_t a_i, b_i;
        if ((a_i = get_set_unique_string(names, a, &n)) == (size_t) -1)
            return READ_ERROR;

        if ((b_i = get_set_unique_string(names, b, &n)) == (size_t) -1)
            return READ_ERROR;

        if (!add_relation(g, a_i, b_i, distance))
            return ADD_ERROR;
    }

    return EXIT_SUCCESS;
}

int print_graph(graph_t *g, mystring_t *names, int *distances, int t)
{
    if (!g || !names || !distances)
        return ARG_ERROR;

    FILE *f = fopen("graph.gv", "w");
    if (!f)
        return OPEN_ERROR;

    fprintf(f, "graph cities {\n");
    fprintf(f, "node [style=rounded, shape=diamond];\n");
    for (size_t i = 0; i < g->n; i++)
    {
        if (distances[i] > t)
            fprintf(f, "\"%s\" [style=\"filled,rounded\", fillcolor=yellow];\n", names[i]);
        else
            fprintf(f, "\"%s\";\n", names[i]);

        for (graph_relation_t *rel = g->vertexes[i]; rel; rel = rel->next) {
            if (rel->vertex < i)
                continue;
            fprintf(f, "\"%s\" -- \"%s\" [label=\"%d\"];\n", names[i], names[rel->vertex], rel->weight);
        }
    }

    fprintf(f, "}");
    fclose(f);
    system("/opt/homebrew/bin/dot -Tpng graph.gv -o graph.png");
    system("open graph.png");

    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    if (argc != 4)
        return ARG_ERROR;

    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        printf("Ошибка открытия файла.\n");
        return OPEN_ERROR;
    }

    int t;
    if (sscanf(argv[3], "%d", &t) != 1)
    {
        printf("Расстояние должно быть целым числом.\n");
        fclose(f);
        return ARG_ERROR;
    }

    graph_t g;
    mystring_t *names = NULL;
    int err = graph_from_file(&g, &names, f);
    if (err)
    {
        printf("Ошибка чтения графа.\n");
        fclose(f);
        free(names);
        graph_free(&g);
        return err;
    }
    char *a1 = names[0];
    char *a2 = names[1];
    char *a3 = names[2];
    printf("%s %s %s\n", a1, a2, a3);
    size_t size = g.n;
    size_t searched = get_set_unique_string(&names, argv[2], &size);
    if (searched == (size_t) -1)
    {
        printf("Ошибка чтения города.\n");
        fclose(f);
        free(names);
        graph_free(&g);
        return err;
    }
    fclose(f);
    for (size_t i = 0; i < g.n; i++)
        printf("%s\n", names[i]);
    int *distances = get_distances(&g, searched);
    for (size_t i = 0; i < g.n; i++)
        printf("%s\n", names[i]);
    if (!distances)
    {
        printf("Ошибка расчета путей.\n");
        fclose(f);
        free(names);
        graph_free(&g);
        return err;
    }

    for (size_t i = 0; i < g.n; i++)
        printf("%s\n", names[i]);

    print_graph(&g, names, distances, t);
    return EXIT_SUCCESS;
}
