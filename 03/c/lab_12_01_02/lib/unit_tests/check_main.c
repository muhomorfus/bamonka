#include <check.h>

Suite* arr_lshift_suite(Suite *s);
Suite* arr_copy_null_pointer_suite(Suite *s);

Suite *main_suite(void)
{
    Suite *s = suite_create("main_suite");

    s = arr_lshift_suite(s);
    s = arr_copy_null_pointer_suite(s);

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