#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "test_my_string_functions.h"
#include "my_string_functions.h"

void print_status(int failed, int all)
{
    if (failed > 0)
        printf("FAILED");
    else
        printf("PASSED");

    printf(" (%d tests failed of %d).\n", failed, all);
}

int compare_strpbrk(char *s1, char *s2)
{
    int fail = 0;
    char *result = strpbrk(s1, s2);
    char *my_result = my_strpbrk(s1, s2);
    if (result != my_result)
    {
        printf("TEST FAILED\n");
        printf("s1 = \"%s\"\n", s1);
        printf("s2 = \"%s\"\n", s2);
        printf("WAIT: %p\n", result);
        printf("GOT: %p\n\n", my_result);

        fail = 1;
    }

    return fail;
}

int test_my_strpbrk(void)
{
    int failed = 0;
    int all = 0;

    printf("TESTING my_strpbrk\n");

    // Simple tests
    {
        int failed_local = 0;
        int all_local = 0;

        char strings[][TEST_STRING_SIZE] = {
            "Hello, world!",
            "hi, my name is test2008 ><",
            ".Hey,",
            "Lorem? ipsum/ dolor, dit amet!!!",
            "oh, yes.",
            "no, no",
        };
        size_t strings_size = sizeof(strings) / sizeof(strings[0]);

        char sym_lists[][TEST_STRING_SIZE] = { ",", ".,", ",!.?", "o.a" };
        size_t sym_lists_size = sizeof(sym_lists) / sizeof(sym_lists[0]);
        all_local += sym_lists_size * strings_size;

        for (size_t i = 0; i < strings_size; i++)
            for (size_t j = 0; j < sym_lists_size; j++)
                failed_local += compare_strpbrk(strings[i], sym_lists[j]);

        printf("Test group #1. Strings have at least one of symbols: ");
        print_status(failed_local, all_local);
        all += all_local;
        failed += failed_local;
    }

    // String may have not symbols
    {
        int failed_local = 0;
        int all_local = 0;

        char strings[][TEST_STRING_SIZE] = {
            "Lorem ipsum dolor sit amet",
            ".",
            "blablabla",
            "test1234",
            "hello, world",
            "why?",
        };
        size_t strings_size = sizeof(strings) / sizeof(strings[0]);

        char sym_lists[][TEST_STRING_SIZE] = { ".", "abc", "why?", "90" };
        size_t sym_lists_size = sizeof(sym_lists) / sizeof(sym_lists[0]);
        all_local += sym_lists_size * strings_size;

        for (size_t i = 0; i < strings_size; i++)
            for (size_t j = 0; j < sym_lists_size; j++)
                failed_local += compare_strpbrk(strings[i], sym_lists[j]);

        printf("Test group #2. Strings may not have symbols: ");
        print_status(failed_local, all_local);

        all += all_local;
        failed += failed_local;
    }

    // May be empty strings
    {
        int failed_local = 0;
        int all_local = 0;

        char strings[][TEST_STRING_SIZE] = {
            "test",
            "",
        };
        size_t strings_size = sizeof(strings) / sizeof(strings[0]);

        char sym_lists[][TEST_STRING_SIZE] = { "test", "" };
        size_t sym_lists_size = sizeof(sym_lists) / sizeof(sym_lists[0]);
        all_local += sym_lists_size * strings_size;

        for (size_t i = 0; i < strings_size; i++)
            for (size_t j = 0; j < sym_lists_size; j++)
                failed_local += compare_strpbrk(strings[i], sym_lists[j]);

        printf("Test group #3. Strings may be empty: ");
        print_status(failed_local, all_local);

        all += all_local;
        failed += failed_local;
    }

    printf("TESTING my_strpbrk: ");
    print_status(failed, all);
    printf("\n");

    return failed;
}

int compare_strspn(char *s1, char *s2)
{
    int fail = 0;
    size_t result = strspn(s1, s2);
    size_t my_result = my_strspn(s1, s2);
    if (result != my_result)
    {
        printf("TEST FAILED\n");
        printf("s1 = \"%s\"\n", s1);
        printf("s2 = \"%s\"\n", s2);
        printf("WAIT: %zu\n", result);
        printf("GOT: %zu\n\n", my_result);

        fail = 1;
    }

    return fail;
}

int test_my_strspn(void)
{
    int failed = 0;
    int all = 0;

    printf("TESTING my_strspn\n");

    // Simple tests
    {
        int failed_local = 0;
        int all_local = 0;

        char strings[][TEST_STRING_SIZE] = {
            "Hello, world!",
            "hi, my name is test2008 ><",
            ".Hey,",
            "Lorem? ipsum/ dolor, dit amet!!!",
            "oh, yes.",
            "no, no",
            "!Hola!",
            "(((test)))",
        };
        size_t strings_size = sizeof(strings) / sizeof(strings[0]);

        char sym_lists[][TEST_STRING_SIZE] = { ",", ".,", ",!.?", "abc", "!!)",
            "(.Lol", ".Hey," };
        size_t sym_lists_size = sizeof(sym_lists) / sizeof(sym_lists[0]);
        all_local += sym_lists_size * strings_size;

        for (size_t i = 0; i < strings_size; i++)
            for (size_t j = 0; j < sym_lists_size; j++)
                failed_local += compare_strspn(strings[i], sym_lists[j]);

        printf("Test group #1. Simple tests: ");
        print_status(failed_local, all_local);
        all += all_local;
        failed += failed_local;
    }

    // May be empty strings
    {
        int failed_local = 0;
        int all_local = 0;

        char strings[][TEST_STRING_SIZE] = {
            "test",
            "",
        };
        size_t strings_size = sizeof(strings) / sizeof(strings[0]);

        char sym_lists[][TEST_STRING_SIZE] = { "test", "" };
        size_t sym_lists_size = sizeof(sym_lists) / sizeof(sym_lists[0]);
        all_local += sym_lists_size * strings_size;

        for (size_t i = 0; i < strings_size; i++)
            for (size_t j = 0; j < sym_lists_size; j++)
                failed_local += compare_strspn(strings[i], sym_lists[j]);

        printf("Test group #2. String may be empty: ");
        print_status(failed_local, all_local);
        all += all_local;
        failed += failed_local;
    }

    printf("TESTING my_strspn: ");
    print_status(failed, all);
    printf("\n");

    return failed;
}

int compare_strcspn(char *s1, char *s2)
{
    int fail = 0;
    size_t result = strcspn(s1, s2);
    size_t my_result = my_strcspn(s1, s2);
    if (result != my_result)
    {
        printf("TEST FAILED\n");
        printf("s1 = \"%s\"\n", s1);
        printf("s2 = \"%s\"\n", s2);
        printf("WAIT: %zu\n", result);
        printf("GOT: %zu\n\n", my_result);

        fail = 1;
    }

    return fail;
}

int test_my_strcspn(void)
{
    int failed = 0;
    int all = 0;

    printf("TESTING my_strcspn\n");

    // Simple tests
    {
        int failed_local = 0;
        int all_local = 0;

        char strings[][TEST_STRING_SIZE] = {
            "Hello, world!",
            "hi, my name is test2008 ><",
            ".Hey,",
            "Lorem? ipsum/ dolor, dit amet!!!",
            "oh, yes.",
            "no, no",
            "!Hola!",
            "(((test)))",
        };
        size_t strings_size = sizeof(strings) / sizeof(strings[0]);

        char sym_lists[][TEST_STRING_SIZE] = { ",", ".,", ",!.?", "abc", "!!)",
            "(.Lol", ".Hey," };
        size_t sym_lists_size = sizeof(sym_lists) / sizeof(sym_lists[0]);
        all_local += sym_lists_size * strings_size;

        for (size_t i = 0; i < strings_size; i++)
            for (size_t j = 0; j < sym_lists_size; j++)
                failed_local += compare_strcspn(strings[i], sym_lists[j]);

        printf("Test group #1. Simple tests: ");
        print_status(failed_local, all_local);
        all += all_local;
        failed += failed_local;
    }

    // May be empty strings
    {
        int failed_local = 0;
        int all_local = 0;

        char strings[][TEST_STRING_SIZE] = {
            "test",
            "",
        };
        size_t strings_size = sizeof(strings) / sizeof(strings[0]);

        char sym_lists[][TEST_STRING_SIZE] = { "test", "" };
        size_t sym_lists_size = sizeof(sym_lists) / sizeof(sym_lists[0]);
        all_local += sym_lists_size * strings_size;

        for (size_t i = 0; i < strings_size; i++)
            for (size_t j = 0; j < sym_lists_size; j++)
                failed_local += compare_strcspn(strings[i], sym_lists[j]);

        printf("Test group #2. String may be empty: ");
        print_status(failed_local, all_local);
        all += all_local;
        failed += failed_local;
    }

    printf("TESTING my_strcspn: ");
    print_status(failed, all);
    printf("\n");

    return failed;
}

int compare_strchr(char *s, char c)
{
    int fail = 0;
    char *result = strchr(s, (int)c);
    char *my_result = my_strchr(s, (int)c);
    if (result != my_result)
    {
        printf("TEST FAILED\n");
        printf("s = \"%s\"\n", s);
        printf("c = \'%c\'\n", c);
        printf("WAIT: %p\n", result);
        printf("GOT: %p\n\n", my_result);

        fail = 1;
    }

    return fail;
}

int test_my_strchr(void)
{
    int failed = 0;
    int all = 0;

    printf("TESTING my_strchr\n");

    // Simple tests
    {
        int failed_local = 0;
        int all_local = 0;

        char strings[][TEST_STRING_SIZE] = {
            "Hello, world!",
            "hi, my name is test2008 ><",
            ".Hey,",
            "Lorem? ipsum/ dolor, dit amet!!!",
            "oh, yes.",
            "!Hola!",
            "(((test)))",
            "#sultanov_methods",
        };
        size_t strings_size = sizeof(strings) / sizeof(strings[0]);

        char symbols[] = { '1', '.', 'A', 'h', '#', ')', '!', '\n' };
        size_t symbols_size = sizeof(symbols) / sizeof(symbols[0]);
        all_local += symbols_size * strings_size;

        for (size_t i = 0; i < strings_size; i++)
            for (size_t j = 0; j < symbols_size; j++)
                failed_local += compare_strchr(strings[i], symbols[j]);

        printf("Test group #1. Simple tests: ");
        print_status(failed_local, all_local);
        all += all_local;
        failed += failed_local;
    }

    // Searching null-character
    {
        int failed_local = 0;
        int all_local = 0;

        char strings[][TEST_STRING_SIZE] = {
            "Hello, world!",
            "TestTestTest",
            "",
        };
        size_t strings_size = sizeof(strings) / sizeof(strings[0]);

        char symbols[] = { '\0' };
        size_t symbols_size = sizeof(symbols) / sizeof(symbols[0]);
        all_local += symbols_size * strings_size;

        for (size_t i = 0; i < strings_size; i++)
            for (size_t j = 0; j < symbols_size; j++)
                failed_local += compare_strchr(strings[i], symbols[j]);

        printf("Test group #2. Searching null-character: ");
        print_status(failed_local, all_local);
        all += all_local;
        failed += failed_local;
    }

    printf("TESTING my_strchr: ");
    print_status(failed, all);
    printf("\n");

    return failed;
}

int compare_strrchr(char *s, char c)
{
    int fail = 0;
    char *result = strrchr(s, (int)c);
    char *my_result = my_strrchr(s, (int)c);
    if (result != my_result)
    {
        printf("TEST FAILED\n");
        printf("s = \"%s\"\n", s);
        printf("c = \'%c\'\n", c);
        printf("WAIT: %p\n", result);
        printf("GOT: %p\n\n", my_result);

        fail = 1;
    }

    return fail;
}

int test_my_strrchr(void)
{
    int failed = 0;
    int all = 0;

    printf("TESTING my_strrchr\n");

    // Simple tests
    {
        int failed_local = 0;
        int all_local = 0;

        char strings[][TEST_STRING_SIZE] = {
            "Hello, world!",
            "hi, my name is test2008 ><",
            ".Hey,",
            "Lorem? \n ipsum/ dolor, dit amet!!!",
            "oh, yes.",
            "!Hola!\n I\'m here",
            "(((test)))",
            "#sultanov_methods",
        };
        size_t strings_size = sizeof(strings) / sizeof(strings[0]);

        char symbols[] = { '1', '.', 'A', 'h', '#', ')', '(', '!', '\n' };
        size_t symbols_size = sizeof(symbols) / sizeof(symbols[0]);
        all_local += symbols_size * strings_size;

        for (size_t i = 0; i < strings_size; i++)
            for (size_t j = 0; j < symbols_size; j++)
                failed_local += compare_strrchr(strings[i], symbols[j]);

        printf("Test group #1. Simple tests: ");
        print_status(failed_local, all_local);
        all += all_local;
        failed += failed_local;
    }

    // Searching null-character
    {
        int failed_local = 0;
        int all_local = 0;

        char strings[][TEST_STRING_SIZE] = {
            "Hello, world!",
            "TestTestTest",
            "",
        };
        size_t strings_size = sizeof(strings) / sizeof(strings[0]);

        char symbols[] = { '\0' };
        size_t symbols_size = sizeof(symbols) / sizeof(symbols[0]);
        all_local += symbols_size * strings_size;

        for (size_t i = 0; i < strings_size; i++)
            for (size_t j = 0; j < symbols_size; j++)
                failed_local += compare_strrchr(strings[i], symbols[j]);

        printf("Test group #2. Searching null-character: ");
        print_status(failed_local, all_local);
        all += all_local;
        failed += failed_local;
    }

    printf("TESTING my_strrchr: ");
    print_status(failed, all);
    printf("\n");

    return failed;
}