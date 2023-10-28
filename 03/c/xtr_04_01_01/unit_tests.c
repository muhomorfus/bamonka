#include "test_set.h"

int main(void)
{
    int failed = 0;

    failed += test_find_position();
    failed += test_in();
    failed += test_add();
    failed += test_remove();
    failed += test_copy();
    failed += test_intersection();
    failed += test_union();
    failed += test_difference();
    failed += test_symmetric_difference();
    failed += test_print();

    return failed;
}