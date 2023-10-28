#include <stdbool.h>
#include <stdio.h>

#include "set.h"
#include "test_set.h"

bool equals(set_t a, set_t b)
{
    if (a.size != b.size)
        return false;

    for (size_t i = 0; i < a.size; i++)
        if (a.elements[i] != b.elements[i])
            return false;

    return true;
}

void print_set(set_t set)
{
    printf("[");
    for (size_t i = 0; i < set.size; i++)
    {
        printf("%d", set.elements[i]);
        if (i != set.size - 1)
            printf(" ");
    }
    printf("]");
}

void fill_set(set_t *set, size_t num, int start)
{
    if (num > SET_MAX_SIZE)
        num = SET_MAX_SIZE;

    set->size = num;
    for (size_t i = 0; i < num; i++)
        set->elements[i] = start++;
}

typedef struct
{
    set_t set;
    int element;
    size_t position;
} find_position_test_t;

int test_find_position(void)
{
    int failed = 0;

    find_position_test_t tests[] = {
        {
            .set = { 0, { 0 } },
            .element = 15,
            .position = 0,
        },
        {
            .set = { 3, { 1, 2, 3 } },
            .element = 15,
            .position = 3,
        },
        {
            .set = { 3, { 1, 2, 3 } },
            .element = 3,
            .position = 2,
        },
        {
            .set = { 3, { 1, 2, 3 } },
            .element = 1,
            .position = 0,
        },
        {
            .set = { 3, { 1, 2, 3 } },
            .element = 0,
            .position = 0,
        },
        {
            .set = { 3, { 1, 2, 3 }},
            .element = 2,
            .position = 1,
        },
        {
            .set = { 3, { 1, 3, 4 } },
            .element = 2,
            .position = 1,
        },
        {
            .set = { 6, { 1, 3, 4, 5, 7, 8 } },
            .element = 6,
            .position = 4 },
    };
    size_t n = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < n; i++)
    {
        size_t position = set_find_position(&tests[i].set, tests[i].element);
        if (position != tests[i].position)
        {
            printf("-----------\n");
            printf("TEST FAILED\n");
            printf("ON: (");
            print_set(tests[i].set);
            printf(", %d)\n", tests[i].element);
            printf("WAIT: (%zu)\n", tests[i].position);
            printf("GOT: (%zu)\n", position);
            printf("-----------\n");

            failed++;
        }
    }

    printf("TESTING %s: ", __func__);
    if (failed == 0)
        printf("PASSED\n");
    else
        printf("FAILED (%d tests)\n", failed);

    return failed;
}

typedef struct
{
    set_t set;
    int element;
    bool in;
} in_test_t;

int test_in(void)
{
    int failed = 0;

    in_test_t tests[] = {
        {
            .set = { 0, { 0 } },
            .element = 15,
            .in = false },
        {
            .set = { 3, { 1, 2, 3 } },
            .element = 15,
            .in = false,
        },
        {
            .set = { 3, { 1, 2, 3 } },
            .element = 3,
            .in = true,
        },
        {
            .set = { 3, { 1, 2, 3 } },
            .element = 1,
            .in = true,
        },
        {
            .set = { 3, { 1, 2, 3 } },
            .element = 0,
            .in = false,
        },
        {
            .set = { 3, { 1, 2, 3 } },
            .element = 2,
            .in = true,
        },
    };
    size_t n = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < n; i++)
    {
        bool in = set_in(&tests[i].set, tests[i].element);
        if (in != tests[i].in)
        {
            printf("-----------\n");
            printf("TEST FAILED\n");
            printf("ON: (");
            print_set(tests[i].set);
            printf(", %d)\n", tests[i].element);
            printf("WAIT: (%d)\n", tests[i].in);
            printf("GOT: (%d)\n", in);
            printf("-----------\n");

            failed++;
        }
    }

    printf("TESTING %s: ", __func__);
    if (failed == 0)
        printf("PASSED\n");
    else
        printf("FAILED (%d tests)\n", failed);

    return failed;
}

typedef struct
{
    set_t set;
    int element;
    set_t set_added;
    bool is_null;
} add_test_t;

int test_add(void)
{
    int failed = 0;

    add_test_t tests[] = {
        {
            .set = { 0, { 0 } },
            .element = 15,
            .set_added = { 1, { 15 } },
            .is_null = false
        },
        {
            .set = { SET_MAX_SIZE, { 0 } },
            .element = 4,
            .set_added = { SET_MAX_SIZE, { 0 } },
            .is_null = true,
        },
        {
            .set = { 3, { 1, 2, 3 } },
            .element = 4,
            .set_added = { 4, { 1, 2, 3, 4 } },
            .is_null = false,
        },
        {
            .set = { 3, { 1, 2, 3 } },
            .element = 0,
            .set_added = { 4, { 0, 1, 2, 3 } },
            .is_null = false,
        },
        {
            .set = { 3, { 1, 3, 4 } },
            .element = 2,
            .set_added = { 4, { 1, 2, 3, 4 } },
            .is_null = false,
            },
        {
            .set = { 3, { 1, 2, 3 } },
            .element = 2,
            .set_added = { 3, { 1, 2, 3 } },
            .is_null = false,
        },
    };
    size_t n = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < n; i++)
    {
        set_t copy = tests[i].set;
        set_t *sp = set_add(&copy, tests[i].element);
        if (!sp != tests[i].is_null || !equals(copy, tests[i].set_added))
        {
            printf("-----------\n");
            printf("TEST FAILED\n");
            printf("ON: (");
            print_set(tests[i].set);
            printf(", %d)\n", tests[i].element);
            printf("WAIT: (");
            print_set(tests[i].set_added);
            printf(", %d)\n", tests[i].is_null);
            printf("GOT: (");
            print_set(copy);
            printf(", %d)\n", !sp);
            printf("-----------\n");

            failed++;
        }
    }

    printf("TESTING %s: ", __func__);
    if (failed == 0)
        printf("PASSED\n");
    else
        printf("FAILED (%d tests)\n", failed);

    return failed;
}

typedef struct
{
    set_t set;
    int element;
    set_t set_added;
    bool is_null;
} remove_test_t;

int test_remove(void)
{
    int failed = 0;

    remove_test_t tests[] = {
        {
            .set = { 0, { 0 } },
            .element = 15,
            .set_added = { 0, { 0 } },
            .is_null = true,
        },
        {
            .set = { 3, { 1, 2, 3 } },
            .element = 4,
            .set_added = { 3, { 1, 2, 3 } },
            .is_null = false,
        },
        {
            .set = { 3, { 1, 2, 3 }},
            .element = 0,
            .set_added = { 3, { 1, 2, 3 }},
            .is_null = false,
        },
        {
            .set = { 3, { 1, 2, 3 } },
            .element = 2,
            .set_added = { 2, { 1, 3 } },
            .is_null = false
        },
        {
            .set = { 3, { 1, 2, 3 } },
            .element = 1,
            .set_added = { 2, { 2, 3 } },
            .is_null = false,
        },
    };
    size_t n = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < n; i++)
    {
        set_t copy = tests[i].set;
        set_t *sp = set_remove(&copy, tests[i].element);
        if (!sp != tests[i].is_null || !equals(copy, tests[i].set_added))
        {
            printf("-----------\n");
            printf("TEST FAILED\n");
            printf("ON: (");
            print_set(tests[i].set);
            printf(", %d)\n", tests[i].element);
            printf("WAIT: (");
            print_set(tests[i].set_added);
            printf(", %d)\n", tests[i].is_null);
            printf("GOT: (");
            print_set(copy);
            printf(", %d)\n", !sp);
            printf("-----------\n");

            failed++;
        }
    }

    printf("TESTING %s: ", __func__);
    if (failed == 0)
        printf("PASSED\n");
    else
        printf("FAILED (%d tests)\n", failed);

    return failed;
}

typedef struct
{
    set_t set;
} copy_test_t;

int test_copy(void)
{
    int failed = 0;

    copy_test_t tests[] = {
        {
            .set = { 0, { 0 } },
        },
        {
            .set = { 3, { 1, 2, 3 } },
        },
    };
    size_t n = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < n; i++)
    {
        set_t copy = tests[i].set;
        set_copy(&tests[i].set, &copy);
        if (!equals(copy, tests[i].set))
        {
            printf("-----------\n");
            printf("TEST FAILED\n");
            printf("ON: (");
            print_set(tests[i].set);
            printf(")\n");
            printf("WAIT: (");
            print_set(tests[i].set);
            printf(")\n");
            printf("GOT: (");
            print_set(copy);
            printf(")\n");
            printf("-----------\n");

            failed++;
        }
    }

    printf("TESTING %s: ", __func__);
    if (failed == 0)
        printf("PASSED\n");
    else
        printf("FAILED (%d tests)\n", failed);

    return failed;
}

typedef struct
{
    set_t a;
    set_t b;
    set_t result;
} intersection_test_t;

int test_intersection(void)
{
    int failed = 0;

    intersection_test_t tests[] = {
        {
            .a = { 0, { 0 } },
            .b = { 0, { 0 } },
            .result = { 0, { 0 } },
        },
        {
            .a = { 0, { 0 } },
            .b = { 3, { 1, 2, 3 } },
            .result = { 0, { 0 } },
        },
        {
            .a = { 2, { 1, 2 } },
            .b = { 2, { 3, 4 } },
            .result = { 0, { 0 } },
        },
        {
            .a = { 2, { 3, 4 } },
            .b = { 2, { 2, 1 } },
            .result = { 0, { 0 } },
        },
        {
            .a = { 3, { 1, 2, 3 } },
            .b = { 2, { 3, 4 } },
            .result = { 1, { 3 } },
        },
        {
            .a = { 3, { 1, 2, 3 } },
            .b = { 3, { 1, 3, 4 } },
            .result = { 2, { 1, 3 } },
        },
        {
            .a = { 3, { 1, 3, 4 } },
            .b = { 3, { 1, 2, 3 } },
            .result = { 2, { 1, 3 } },
        },
        {
            .a = { 3, { 1, 2, 3 } },
            .b = { 3, { 1, 2, 3 } },
            .result = { 3, { 1, 2, 3 } },
        },
    };
    size_t n = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < n; i++)
    {
        set_t result = set_intersection(&tests[i].a, &tests[i].b);
        if (!equals(result, tests[i].result))
        {
            printf("-----------\n");
            printf("TEST FAILED\n");
            printf("ON: (");
            print_set(tests[i].a);
            printf(", ");
            print_set(tests[i].b);
            printf(")\n");
            printf("WAIT: (");
            print_set(tests[i].result);
            printf(")\n");
            printf("GOT: (");
            print_set(result);
            printf(")\n");
            printf("-----------\n");

            failed++;
        }
    }

    printf("TESTING %s: ", __func__);
    if (failed == 0)
        printf("PASSED\n");
    else
        printf("FAILED (%d tests)\n", failed);

    return failed;
}

typedef struct
{
    set_t a;
    set_t b;
    set_t result;
} union_test_t;

int test_union(void)
{
    int failed = 0;

    union_test_t tests[] = {
        {
            .a = { 0, { 0 } },
            .b = { 0, { 0 } },
            .result = { 0, { 0 } },
        },
        {
            .a = { 2, { 1, 2 } },
            .b = { 2, { 3, 4 } },
            .result = { 4, { 1, 2, 3, 4 } },
        },
        {
            .a = { 2, { 1, 2 } },
            .b = { 2, { 2, 3 } },
            .result = { 3, { 1, 2, 3 } },
        },
        {
            .a = { 2, { 2, 3 } },
            .b = { 2, { 1, 2 } },
            .result = { 3, { 1, 2, 3 } },
        },
        {
            .a = { 2, { 2, 3 } },
            .b = { 0, { 0 } },
            .result = { 2, { 2, 3 } },
        },
        {
            .a = { SET_MAX_SIZE, { 1 } },
            .b = { 1, { SET_MAX_SIZE + 1 } },
            .result = { SET_MAX_SIZE, { 1 } },
        },
        {
            .a = { SET_MAX_SIZE, { 1 } },
            .b = { 1, { 1 } },
            .result = { SET_MAX_SIZE, { 1 } },
        },
    };
    fill_set(&tests[5].a, SET_MAX_SIZE, 1);
    fill_set(&tests[5].result, SET_MAX_SIZE, 1);
    fill_set(&tests[6].a, SET_MAX_SIZE, 1);
    fill_set(&tests[6].result, SET_MAX_SIZE, 1);

    size_t n = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < n; i++)
    {
        set_t result = set_union(&tests[i].a, &tests[i].b);
        if (!equals(result, tests[i].result))
        {
            printf("-----------\n");
            printf("TEST FAILED\n");
            printf("ON: (");
            print_set(tests[i].a);
            printf(", ");
            print_set(tests[i].b);
            printf(")\n");
            printf("WAIT: (");
            print_set(tests[i].result);
            printf(")\n");
            printf("GOT: (");
            print_set(result);
            printf(")\n");
            printf("-----------\n");

            failed++;
        }
    }

    printf("TESTING %s: ", __func__);
    if (failed == 0)
        printf("PASSED\n");
    else
        printf("FAILED (%d tests)\n", failed);

    return failed;
}

typedef struct
{
    set_t a;
    set_t b;
    set_t result;
} difference_test_t;

int test_difference(void)
{
    int failed = 0;

    difference_test_t tests[] = {
        {
            .a = { 0, { 0 } },
            .b = { 0, { 0 } },
            .result = { 0, { 0 } },
        },
        {
            .a = { 0, { 0 }},
            .b = { 2, { 1, 2 }},
            .result = { 0, { 0 }},
        },
        {
            .a = { 2, { 1, 2 } },
            .b = { 0, { 0 } },
            .result = { 2, { 1, 2 } },
        },
        {
            .a = { 2, { 1, 2 } },
            .b = { 2, { 1, 2 } },
            .result = { 0, { 0 } },
        },
        {
            .a = { 3, { 1, 2, 3 } },
            .b = { 1, { 1 } },
            .result = { 2, { 2, 3 } },
        },
        {
            .a = { 4, { 1, 2, 3, 4 } },
            .b = { 3, { 0, 1, 3 } },
            .result = { 2, { 2, 4 } },
        },
        {
            .a = { 3, { 0, 1, 3 } },
            .b = { 4, { 1, 2, 3, 4 } },
            .result = { 1, { 0 } },
        },
    };
    size_t n = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < n; i++)
    {
        set_t result = set_difference(&tests[i].a, &tests[i].b);
        if (!equals(result, tests[i].result))
        {
            printf("-----------\n");
            printf("TEST FAILED\n");
            printf("ON: (");
            print_set(tests[i].a);
            printf(", ");
            print_set(tests[i].b);
            printf(")\n");
            printf("WAIT: (");
            print_set(tests[i].result);
            printf(")\n");
            printf("GOT: (");
            print_set(result);
            printf(")\n");
            printf("-----------\n");

            failed++;
        }
    }

    printf("TESTING %s: ", __func__);
    if (failed == 0)
        printf("PASSED\n");
    else
        printf("FAILED (%d tests)\n", failed);

    return failed;
}

typedef struct
{
    set_t a;
    set_t b;
    set_t result;
} symmetric_difference_test_t;

int test_symmetric_difference(void)
{
    int failed = 0;

    symmetric_difference_test_t tests[] = {
        {
            .a = { 0, { 0 } },
            .b = { 0, { 0 } },
            .result = { 0, { 0 } },
        },
        {
            .a = { 0, { 0 } },
            .b = { 2, { 1, 2 } },
            .result = { 2, { 1, 2 } },
        },
        {
            .a = { 2, { 1, 2 } },
            .b = { 2, { 3, 4 } },
            .result = { 4, { 1, 2, 3, 4 } },
        },
        {
            .a = { 2, { 3, 4 } },
            .b = { 2, { 1, 2 } },
            .result = { 4, { 1, 2, 3, 4 } },
        },
        {
            .a = { 2, { 1, 2 } },
            .b = { 2, { 2, 4 } },
            .result = { 2, { 1, 4 } },
        },
        {
            .a = { 3, { 1, 2, 3 } },
            .b = { 3, { 1, 2, 3 } },
            .result = { 0, { 0 } },
        },
        {
            .a = { SET_MAX_SIZE, { 1 } },
            .b = { 1, { SET_MAX_SIZE + 1 } },
            .result = { SET_MAX_SIZE, { 1 } },
        },
        {
            .a = { SET_MAX_SIZE, { 1 } },
            .b = { 1, { 1 } },
            .result = { SET_MAX_SIZE - 1, { 2 } },
        },
    };
    fill_set(&tests[6].a, SET_MAX_SIZE, 1);
    fill_set(&tests[6].result, SET_MAX_SIZE, 1);
    fill_set(&tests[7].a, SET_MAX_SIZE, 1);
    fill_set(&tests[7].result, SET_MAX_SIZE - 1, 2);

    size_t n = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < n; i++)
    {
        set_t result = set_symmetric_difference(&tests[i].a, &tests[i].b);
        if (!equals(result, tests[i].result))
        {
            printf("-----------\n");
            printf("TEST FAILED\n");
            printf("ON: (");
            print_set(tests[i].a);
            printf(", ");
            print_set(tests[i].b);
            printf(")\n");
            printf("WAIT: (");
            print_set(tests[i].result);
            printf(")\n");
            printf("GOT: (");
            print_set(result);
            printf(")\n");
            printf("-----------\n");

            failed++;
        }
    }

    printf("TESTING %s: ", __func__);
    if (failed == 0)
        printf("PASSED\n");
    else
        printf("FAILED (%d tests)\n", failed);

    return failed;
}

typedef struct
{
    set_t set;
} print_test_t;

int test_print(void)
{
    int failed = 0;

    print_test_t tests[] = {
        {
            .set = { 0, { 0 } },
        },
        {
            .set = { 3, { 1, 2, 3 } },
        },
    };
    size_t n = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < n; i++)
    {
        size_t printed = set_print(&tests[i].set);
        if (printed != tests[i].set.size)
        {
            printf("-----------\n");
            printf("TEST FAILED\n");
            printf("ON: (");
            print_set(tests[i].set);
            printf(")\n");
            printf("WAIT: (%zu)\n", tests[i].set.size);
            printf("GOT: (%zu)\n", printed);
            printf("-----------\n");

            failed++;
        }
    }

    printf("TESTING %s: ", __func__);
    if (failed == 0)
        printf("PASSED\n");
    else
        printf("FAILED (%d tests)\n", failed);

    return failed;
}