#include <stdio.h>

#include "mystring.h"
#include "error.h"
#include "flat.h"
#include "flats_db.h"
#include "table.h"
#include "ui.h"

#define ERROR(err) printf("%d: %s (%s).", err.code, err.text, err.func)

#define FILE_OPEN_ERROR 2

int main(void)
{
    mystring_t filename;
    printf("Введите имя файла: ");
    error_t err = f_read_line(stdin, filename);
    if (is_failure(err))
    {
        ERROR(err);
        return err.code;
    }
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        err = NEW_ERROR("Ошибка открытия файла", FILE_OPEN_ERROR);
        ERROR(err);
        return err.code;
    }

    flats_db_t db;
    db.f = f;
    err = db_read(&db);
    if (is_failure(err))
    {
        ERROR(err);
        fclose(f);
        return err.code;
    }

    process_main(&db);

    fclose(f);
}
