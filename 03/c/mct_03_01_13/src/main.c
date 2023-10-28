#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "matrix.h"

#define INPUT_ERROR 10
#define UNKNOWN_ERROR

int main()
{
    size_t n;
    if (scanf("%zu", &n) != 1 || !n)
        return INPUT_ERROR;

    matrix_t *matrices = calloc(n * 2 * sizeof(matrix_t));
    int code;
    for (size_t k = 0; k < n * 2 && !code; k += 2)
    {
        code = m_read_with_size(matrices + k);
        if (code)
        {
            for (size_t q = k; q > 0; q--)
                m_free(matrices + q - 1);
        }

        if (!code)
        {
            code = m_create_duplicate(matrices + k, matrices + k + 1);
            if (code)
            {
                for (size_t q = k; q > 0; q--)
                    m_free(matrices + q - 1);
            }
        }


    if (!code)
    {
        for (size_t k = 0; k < n * 2; k++)
            m_print(matrices + k);}
    }
    for (size_t q = 0; q < n * 2; q++)
        m_free(matrices + q);
    free(matrices);
    return code;
}