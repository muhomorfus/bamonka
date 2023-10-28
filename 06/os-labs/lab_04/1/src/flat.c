#include <stdio.h>
#include <string.h>

#include "flat.h"
#include "mystring.h"
#include "error.h"

#define FATAL_CHECK(expr) err = expr; if (is_failure(err)) return err

error_t convert_to_flat(mystring_t *strs, size_t n, flat_t *flat, flat_converter_t c)
{
    if (n != MIN_NUM_OF_FIELDS && n != MAX_NUM_OF_FIELDS)
        return NEW_ERROR("Неверное количество полей", CONVERSION_ERROR);

    for (size_t i = 0; i < n; i++)
        trim_spaces(strs[i]);

    error_t err;
    size_t k = 0;

    FATAL_CHECK(c.address(strs[k++], flat));
    FATAL_CHECK(c.area(strs[k++], flat));
    FATAL_CHECK(c.number_of_rooms(strs[k++], flat));
    FATAL_CHECK(c.cost_per_square_meter(strs[k++], flat));
    FATAL_CHECK(c.class(strs[k++], flat));

    if (flat->class == primary)
    {
        if (n != MIN_NUM_OF_FIELDS)
            return NEW_ERROR("Неверное количество полей", CONVERSION_ERROR);

        FATAL_CHECK(c.have_finishing(strs[k++], flat));
    }
    else if (flat->class == secondary)
    {
        if (n != MAX_NUM_OF_FIELDS)
            return NEW_ERROR("Неверное количество полей", CONVERSION_ERROR);

        FATAL_CHECK(c.year_of_construction(strs[k++], flat));
        FATAL_CHECK(c.number_of_owners(strs[k++], flat));
        FATAL_CHECK(c.number_of_tenants(strs[k++], flat));
        FATAL_CHECK(c.were_animals(strs[k++], flat));
    }

    return NEW_SUCCESS();
}

error_t convert_all_from_flat(mystring_t *strs, size_t n, flat_t *flat, flat_converter_t c)
{
    if (n != MAX_NUM_OF_FIELDS)
        return NEW_ERROR("Неверное количество полей", CONVERSION_ERROR);

    error_t err;
    size_t k = 0;
    FATAL_CHECK(c.address(strs[k++], flat));
    FATAL_CHECK(c.area(strs[k++], flat));
    FATAL_CHECK(c.number_of_rooms(strs[k++], flat));
    FATAL_CHECK(c.cost_per_square_meter(strs[k++], flat));
    FATAL_CHECK(c.class(strs[k++], flat));
    FATAL_CHECK(c.have_finishing(strs[k++], flat));
    FATAL_CHECK(c.year_of_construction(strs[k++], flat));
    FATAL_CHECK(c.number_of_owners(strs[k++], flat));
    FATAL_CHECK(c.number_of_tenants(strs[k++], flat));
    FATAL_CHECK(c.were_animals(strs[k++], flat));

    return NEW_SUCCESS();
}