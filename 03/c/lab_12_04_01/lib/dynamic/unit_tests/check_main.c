#include <check.h>

Suite* new_ints_array_suite(Suite *s);
Suite* key_suite(Suite *s);
Suite* compare_ints_suite(Suite *s);
Suite* mysort_suite(Suite *s);

Suite *main_suite(void)
{
    Suite *s = suite_create("main_suite");

    s = new_ints_array_suite(s);
    s = key_suite(s);
    s = compare_ints_suite(s);
    s = mysort_suite(s);

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