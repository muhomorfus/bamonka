#include "enigma.h"

void e_free(enigma_t* e) {
    if (e->rotors != NULL && e->rotors_count > 0) {
        for (int i = 0; i < e->rotors_count; i++) {
            free(e->rotors[i]);
        }
    }

    if (e->reverse_rotors != NULL && e->rotors_count > 0) {
        for (int i = 0; i < e->rotors_count; i++) {
            free(e->reverse_rotors[i]);
        }
    }

    free(e->rotors);
    free(e->reverse_rotors);
    free(e->reflector);
    free(e->panel);

    e->rotors = NULL;
    e->reverse_rotors = NULL;
    e->reflector = NULL;
    e->panel = NULL;
    e->alphabet_count = 0;
    e->rotors_count = 0;
    e->counter = 0;
}

void e_shift_right(enigma_t* e, int rotor) {
    e->shifts[rotor] = (e->shifts[rotor] + 1) % e->alphabet_count;
}

void e_shift_rotors(enigma_t* e) {
    e_shift_right(e, 0);
    for (int j = 1; j < e->rotors_count; j++) {
        if (e->counter % (e->alphabet_count * j) == 0) {
            e_shift_right(e, j);
        }
    }
}

int e_apply_shift(enigma_t* e, int value, int rotor) {
    value -= e->shifts[rotor];
    if (value < 0) {
        value += e->alphabet_count;
    }

    return value;
}

int e_apply_shift_rev(enigma_t* e, int value, int rotor) {
    value += e->shifts[rotor];
    if (value >= e->alphabet_count) {
        value -= e->alphabet_count;
    }

    return value;
}

unsigned char* e_encode(enigma_t* e, unsigned char* data, size_t size) {
    unsigned char* res = calloc(size, sizeof(unsigned char));

    e->counter = 1;
    for (int i = 0; i < size; i++, e->counter++) {
        int code = data[i];

        code = e->panel[code];

        for (int j = 0; j < e->rotors_count; j++) {
            code = e_apply_shift_rev(e, e->rotors[j][e_apply_shift(e, code, j)], j);
        }

        code = e->reflector[code];

        for (int j = e->rotors_count - 1; j >= 0; j--) {
            code = e_apply_shift_rev(e, e->reverse_rotors[j][e_apply_shift(e, code, j)], j);
        }

        code = e->panel[code];
        res[i] = code;

        e_shift_rotors(e);
    }

    return res;
}

void e_print(enigma_t* e) {
    printf("Counter: %d\n", e->counter);
    printf("Alphabet count: %d\n", e->alphabet_count);
    printf("Rotors count: %d\n", e->rotors_count);

    printf("\nRotors:\n");
    for (int i = 1; i <= e->rotors_count; i++) {
        printf("%7d   ", i);
    }
    printf("\n");

    for (int i = 0; i < e->alphabet_count; i++) {
        for (int j = 0; j < e->rotors_count; j++) {
            printf("%03d:%03d   ", i, e->rotors[j][i]);
        }
        printf("\n");
    }

    printf("\nReflector:\n");
    for (int i = 0; i < e->alphabet_count; i++) {
        printf("%03d:%03d\n", i, e->reflector[i]);
    }
    printf("\n");

    printf("\nPanel:\n");
    for (int i = 0; i < e->alphabet_count; i++) {
        printf("%03d:%03d\n", i, e->panel[i]);
    }
    printf("\n");
}

int e_read_alphabet_count(enigma_t *e) {
    FILE* f;

    f = fopen("configs/alphabet_count.txt", "r");
    if (f == NULL)
    {
        printf("fopen() failed\n");
        return EXIT_FAILURE;
    }

    if (fscanf(f, "%d", &e->alphabet_count) <= 0) {
        printf("fscanf() failed()\n");
        fclose(f);
        return EXIT_FAILURE;
    }

    fclose(f);
    return EXIT_SUCCESS;
}

int e_read_rotors_count(enigma_t *e) {
    FILE* f;

    f = fopen("configs/rotors_count.txt", "r");
    if (f == NULL)
    {
        printf("fopen() failed\n");
        return EXIT_FAILURE;
    }

    if (fscanf(f, "%d", &e->rotors_count) <= 0) {
        printf("fscanf() failed()\n");
        fclose(f);
        return EXIT_FAILURE;
    }

    fclose(f);
    return EXIT_SUCCESS;
}

int e_read_rotor(enigma_t *e, int rotor) {
    FILE* f;

    char filename[100] = { 0 };
    sprintf(filename, "configs/rotor_%d.txt", rotor+1);

    f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("fopen() failed\n");
        return EXIT_FAILURE;
    }

    if (fscanf(f, "%d", &e->shifts[rotor]) <= 0) {
        printf("fscanf(shift) failed()\n");
        fclose(f);
        return EXIT_FAILURE;
    }

    e->rotors[rotor] = malloc(e->alphabet_count * sizeof(int));
    if (e->rotors[rotor] == NULL) {
        printf("malloc() failed\n");
        fclose(f);
        return EXIT_FAILURE;
    }

    e->reverse_rotors[rotor] = malloc(e->alphabet_count * sizeof(int));
    if (e->reverse_rotors[rotor] == NULL) {
        printf("malloc() failed\n");
        fclose(f);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < e->alphabet_count; i++) {
        if (fscanf(f, "%d", &e->rotors[rotor][i]) <= 0) {
            printf("fscanf() failed()\n");
            fclose(f);
            return EXIT_FAILURE;
        }

        e->reverse_rotors[rotor][e->rotors[rotor][i]] = i;
    }

    fclose(f);
    return EXIT_SUCCESS;
}

int e_read_panel(enigma_t *e) {
    FILE* f;

    f = fopen("configs/panel.txt", "r");
    if (f == NULL)
    {
        printf("fopen() failed\n");
        return EXIT_FAILURE;
    }

    e->panel = malloc(e->alphabet_count * sizeof(int));
    if (e->panel == NULL) {
        printf("malloc() failed\n");
        fclose(f);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < e->alphabet_count; i++) {
        if (fscanf(f, "%d", &e->panel[i]) <= 0) {
            printf("fscanf() failed()\n");
            fclose(f);
            return EXIT_FAILURE;
        }
    }

    fclose(f);
    return EXIT_SUCCESS;
}

int e_read_reflector(enigma_t *e) {
    FILE* f;

    f = fopen("configs/reflector.txt", "r");
    if (f == NULL)
    {
        printf("fopen() failed\n");
        return EXIT_FAILURE;
    }

    e->reflector = malloc(e->alphabet_count * sizeof(int));
    if (e->reflector == NULL) {
        printf("malloc() failed\n");
        fclose(f);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < e->alphabet_count; i++) {
        if (fscanf(f, "%d", &e->reflector[i]) <= 0) {
            printf("fscanf() failed()\n");
            fclose(f);
            return EXIT_FAILURE;
        }
    }

    fclose(f);
    return EXIT_SUCCESS;
}

int e_read(enigma_t *e) {
    if (e_read_alphabet_count(e)) {
        printf("e_read_alphabet_count() failed\n");
        return EXIT_FAILURE;
    }

    if (e_read_rotors_count(e)) {
        printf("e_read_rotors_count() failed\n");
        return EXIT_FAILURE;
    }

    e->rotors = malloc(e->rotors_count * sizeof(int *));
    if (e->rotors == NULL) {
        printf("malloc() failed\n");
        return EXIT_FAILURE;
    }

    e->reverse_rotors = malloc(e->rotors_count * sizeof(int *));
    if (e->rotors == NULL) {
        printf("malloc() failed\n");
        return EXIT_FAILURE;
    }

    e->shifts = malloc(e->rotors_count * sizeof(int));
    if (e->shifts == NULL) {
        printf("malloc() failed\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < e->rotors_count; i++) {
        if (e_read_rotor(e, i)) {
            printf("e_read_rotor(%d) failed\n", i);
            e_free(e);
            return EXIT_FAILURE;
        }
    }

    if (e_read_panel(e)) {
        printf("e_read_panel() failed\n");
        e_free(e);
        return EXIT_FAILURE;
    }

    if (e_read_reflector(e)) {
        printf("e_read_reflector() failed\n");
        e_free(e);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

