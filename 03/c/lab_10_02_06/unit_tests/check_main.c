#include <check.h>

Suite* mlist_mul_suite(Suite *s);
Suite* mlist_add_suite(Suite *s);
Suite* mlist_del_row_max_suite(Suite *s);

Suite *main_suite(void)
{
    Suite *s = suite_create("main_suite");

    s = mlist_mul_suite(s);
    s = mlist_add_suite(s);
    s = mlist_del_row_max_suite(s);

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