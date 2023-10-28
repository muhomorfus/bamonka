#ifndef FILE_H
#define FILE_H

#include "mystring.h"
#include "error.h"

#define FILE_OPEN_ERROR 700
#define FILE_ALLOC_ERROR 701
#define FILE_ARGS_ERROR 702
#define FILE_READ_ERROR 703
#define FILE_WRITE_ERROR 704
#define FILE_EMPTY_ERROR 705
#define FILE_NOT_FOUND_ERROR 706

/**
 * Добавление слова в текстовый файл.
 * filename - имя файла;
 * key - добавляемое слово.
 */
error_t file_insert(char *filename, mystring_t key);

/**
 * Удаление слова из текстового файла.
 * filename - имя файла;
 * key - удаляемое слово.
 */
error_t file_remove(char *filename, mystring_t key, size_t *n_cmp);

/**
 * Поиск слова в текстовом файле.
 * filename - имя файла;
 * key - искомое слово.
 */
error_t file_search(char *filename, mystring_t key, size_t *n_cmp);

#endif //FILE_H
