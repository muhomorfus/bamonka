#include <check.h>

Suite* to_double_suite(Suite *s);
Suite* to_integer_suite(Suite *s);

Suite* pop_front_suite(Suite *s);
Suite* pop_back_suite(Suite *s);
Suite* remove_duplicates_suite(Suite *s);
Suite* sorted_insert_suite(Suite *s);
Suite* sort_suite(Suite *s);

Suite *main_suite(void)
{
    Suite *s = suite_create("main_suite");

    s = to_double_suite(s);
    s = to_integer_suite(s);

    s = pop_front_suite(s);
    s = pop_back_suite(s);
    s = remove_duplicates_suite(s);
    s = sorted_insert_suite(s);
    s = sort_suite(s);

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