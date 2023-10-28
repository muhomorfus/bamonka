#include <check.h>

Suite* queue_push_many_suite(Suite *s);
Suite* queue_push_suite(Suite *s);
Suite* queue_pop_suite(Suite *s);

Suite *main_suite(void)
{
    Suite *s = suite_create("main_suite");

    s = queue_push_many_suite(s);
    s = queue_push_suite(s);
    s = queue_pop_suite(s);

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
