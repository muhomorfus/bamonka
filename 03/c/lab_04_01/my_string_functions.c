#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "my_string_functions.h"

char *my_strpbrk(const char *s1, const char *s2)
{
    for (const char *sp1 = s1; *sp1 != '\0'; sp1++)
        for (const char *sp2 = s2; *sp2 != '\0'; sp2++)
            if (*sp1 == *sp2)
                return (char *)sp1;

    return NULL;
}

size_t my_strspn(const char *s1, const char *s2)
{
    size_t len = 0;
    bool in_s2 = true;

    for (const char *sp1 = s1; *sp1 != '\0' && in_s2; sp1++)
    {
        in_s2 = false;
        for (const char *sp2 = s2; *sp2 != '\0' && !in_s2; sp2++)
            if (*sp1 == *sp2)
                in_s2 = true;

        if (in_s2)
            len++;
    }

    return len;
}

size_t my_strcspn(const char *s1, const char *s2)
{
    size_t len = 0;
    bool in_s2 = false;

    for (const char *sp1 = s1; *sp1 != '\0' && !in_s2; sp1++)
    {
        in_s2 = false;
        for (const char *sp2 = s2; *sp2 != '\0' && !in_s2; sp2++)
            if (*sp1 == *sp2)
                in_s2 = true;

        if (!in_s2)
            len++;
    }

    return len;
}

char *my_strchr(const char *s, int c)
{
    const char *sp;
    for (sp = s; *sp != (char)c; sp++)
        if (*sp == '\0')
            return NULL;

    return (char *)sp;
}

char *my_strrchr(const char *s, int c)
{
    const char *last = NULL;

    const char *sp;
    for (sp = s; *sp != '\0'; sp++)
        if (*sp == (char)c)
            last = sp;

    if ((char)c == '\0')
        last = sp;

    return (char *)last;
}