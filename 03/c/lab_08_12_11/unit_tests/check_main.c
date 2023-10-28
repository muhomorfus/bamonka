#include <check.h>

Suite* dm_fill_multiply_suite(Suite *s);
Suite* dm_fill_by_matrix_suite(Suite *s);
Suite* dm_fill_addition_suite(Suite *s);
Suite *dm_det_suite(Suite *s);

Suite *main_suite(void)
{
    Suite *s = suite_create("main_suite");
    
    s = dm_fill_multiply_suite(s);
    s = dm_fill_addition_suite(s);
    s = dm_fill_by_matrix_suite(s);
    s = dm_det_suite(s);

    return s;
}

int main(void)
{
    int no_failed = 0;
    Suite *s;
    SRunner *runner;

    s = main_suite();
    runner = srunner_create(s);

    srunner_run_all(runner, CK_VERBOSE);
    no_failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return no_failed;
}