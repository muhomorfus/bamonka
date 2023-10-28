#ifndef UI_H
#define UI_H

#include "error.h"
#include "flats_db.h"

#define INPUT_CONV_ERROR 401

/*
 * Основной цикл меню программы.
 */
error_t process_main(flats_db_t *db);

#endif //UI_H
