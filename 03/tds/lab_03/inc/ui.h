#ifndef UI_H
#define UI_H

#include "error.h"

#define UI_INPUT_ERROR 161
#define UI_OUTPUT_ERROR 162
#define UI_ARGS_ERROR 163
#define UI_FILE_OPEN_ERROR 164
#define UI_CONVERSION_ERROR 165

/*
 * Запуск основного диалога программы.
 */
error_t main_scenario();

#endif //UI_H
