#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ht.h"

size_t ht_additive_method(mystring_t str, size_t size)
{
    size_t result = 0;
    for (size_t i = 0; str[i]; i++)
        result += (size_t) str[i];
    result %= size;
    return result;
}

ht_t *ht_new(size_t size, size_t (*hash) (mystring_t, size_t))
{
    ht_t *ht = calloc(1, sizeof(ht_t));
    if (!ht)
        return NULL;

    ht->table = calloc(size, sizeof(ht_element_t *));
    if (!ht->table)
    {
        free(ht);
        return NULL;
    }

    ht->size = size;
    ht->hash = hash;

    return ht;
}

static void free_list(ht_element_t *head)
{
    ht_element_t *next;
    for (ht_element_t *cur = head; cur; cur = next)
    {
        next = cur->next;
        free(cur);
    }
}

static error_t add_to_list(ht_element_t **head, mystring_t key)
{
    if (!head || !key)
        return NEW_ERROR("передан нулевой указатель", HT_ARGS_ERROR);

    if (!*head)
    {
        *head = calloc(1, sizeof(ht_element_t));
        if (!*head)
            return NEW_ERROR("ошибка выделения памяти", HT_ALLOC_ERROR);

        strcpy((*head)->key, key);
        return NEW_SUCCESS();
    }

    ht_element_t *current;
    for (current = *head; current->next; current = current->next)
        if (!strcmp(current->next->key, key))
            return NEW_ERROR("элемент уже присутствует в хеш-таблице", HT_ALREADY_EXIST_ERROR);

    current->next = calloc(1, sizeof(ht_element_t));
    if (!current->next)
        return NEW_ERROR("ошибка выделения памяти", HT_ALLOC_ERROR);

    strcpy(current->next->key, key);
    return NEW_SUCCESS();
}

static error_t remove_from_list(ht_element_t **head, mystring_t key, size_t *n_cmp)
{
    if (!head || !key || !n_cmp)
        return NEW_ERROR("передан нулевой указатель", HT_ARGS_ERROR);

    if (!*head)
        return NEW_ERROR("элемент не найден", HT_NOT_FOUND_ERROR);

    *n_cmp = 1;
    if (!strcmp((*head)->key, key))
    {
        ht_element_t *next = (*head)->next;
        free(*head);
        *head = next;
        return NEW_SUCCESS();
    }

    ht_element_t *prev;
    for (prev = *head; prev->next && strcmp(prev->next->key, key); prev = prev->next)
        *n_cmp += 1;
    if (!prev->next)
        return NEW_ERROR("элемент не найден", HT_NOT_FOUND_ERROR);

    ht_element_t *next = prev->next->next;

    free(prev->next);
    prev->next = next;
    return NEW_SUCCESS();
}

static error_t find_in_list(ht_element_t **head, mystring_t key, ht_element_t **found, size_t *n_cmp)
{
    if (!head || !key || !found || !n_cmp)
        return NEW_ERROR("передан нулевой указатель", HT_ARGS_ERROR);

    if (*found)
        return NEW_ERROR("указатель на результат должен быть нулевым", HT_ARGS_ERROR);

    *n_cmp = 1;
    ht_element_t *cur;
    for (cur = *head; cur && strcmp(cur->key, key); cur = cur->next)
        *n_cmp += 1;
    if (!cur)
        return NEW_ERROR("элемент не найден", HT_NOT_FOUND_ERROR);
    *found = cur;

    return NEW_SUCCESS();
}

void ht_free(ht_t **ht)
{
    if (!ht)
        return;

    for (size_t i = 0; i < (*ht)->size; i++)
        free_list((*ht)->table[i]);
    free((*ht)->table);
    free(*ht);
    *ht = NULL;
}

error_t ht_insert(ht_t **ht, mystring_t key)
{
    if (!ht || !key)
        return NEW_ERROR("передан нулевой указатель", HT_ARGS_ERROR);

    size_t index = (*ht)->hash(key, (*ht)->size);
    if (index >= (*ht)->size)
        return NEW_ERROR("элемент выходит за пределы памяти", HT_OVERFLOW_ERROR);

    return add_to_list((*ht)->table + index, key);
}

error_t ht_remove(ht_t **ht, mystring_t key, size_t *n_cmp)
{
    if (!ht || !key || !n_cmp)
        return NEW_ERROR("передан нулевой указатель", HT_ARGS_ERROR);

    size_t index = (*ht)->hash(key, (*ht)->size);
    if (index >= (*ht)->size)
        return NEW_ERROR("элемент выходит за пределы памяти", HT_OVERFLOW_ERROR);

    return remove_from_list((*ht)->table + index, key, n_cmp);
}

error_t ht_search(ht_t **ht, mystring_t key, ht_element_t **found, size_t *n_cmp)
{
    if (!ht || !key || !found || !n_cmp)
        return NEW_ERROR("передан нулевой указатель", HT_ARGS_ERROR);

    if (*found)
        return NEW_ERROR("указатель на результат должен быть нулевым", HT_ARGS_ERROR);

    size_t index = (*ht)->hash(key, (*ht)->size);
    if (index >= (*ht)->size)
        return NEW_ERROR("элемент выходит за пределы памяти", HT_OVERFLOW_ERROR);

    return find_in_list((*ht)->table + index, key, found, n_cmp);
}

error_t print_list(ht_element_t **head, size_t index)
{
    if (!head)
        return NEW_ERROR("передан нулевой указатель", HT_ARGS_ERROR);

    if (!*head)
        printf("%04zu: EMPTY\n", index);

    for (ht_element_t *cur = *head; cur; cur = cur->next)
        printf("%04zu: \"%s\"\n", index, cur->key);

    return NEW_SUCCESS();
}

error_t ht_print(ht_t **ht)
{
    if (!ht)
        return NEW_ERROR("передан нулевой указатель", HT_ARGS_ERROR);

    size_t k = 0;
    for (size_t i = 0; i < (*ht)->size; i++)
    {
        k++;
        error_t err;
        if (is_failure(err = print_list((*ht)->table + i, i)))
            return err;
    }

    if (!k)
        return NEW_ERROR("хеш-таблица пуста", HT_EMPTY_ERROR);

    return NEW_SUCCESS();
}

error_t ht_from_file(ht_t **ht, FILE *f)
{
    if (!ht || !f)
        return NEW_ERROR("передан нулевой указатель", HT_ARGS_ERROR);

    rewind(f);

    mystring_t str;
    size_t read = 0;
    while (fscanf(f, "%s", str) == 1)
    {
        error_t err;
        if (is_failure(err = ht_insert(ht, str)))
            return err;

        read++;
    }

    if (!read || !feof(f))
        return NEW_ERROR("ошибка ввода хеш-таблицы", HT_READ_ERROR);

    return NEW_SUCCESS();
}
