#ifndef ENGINE_H
#define ENGINE_H

#include "error.h"

#define ENG_INPUT_ERROR 161
#define ENG_OUTPUT_ERROR 162
#define ENG_ARGS_ERROR 163
#define ENG_FILE_OPEN_ERROR 164
#define ENG_CONVERSION_ERROR 165

/**
 * Запуск основного диалога программы.
 */
error_t eng_work();

#endif //ENGINE_H
