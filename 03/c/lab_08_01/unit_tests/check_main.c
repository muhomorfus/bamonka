#include <check.h>

Suite* im_fill_identity_suite(Suite *s);
Suite* im_fill_multiply_suite(Suite *s);
Suite* im_fill_by_matrix_suite(Suite *s);
Suite* im_delete_col_suite(Suite *s);
Suite* im_delete_row_suite(Suite *s);
Suite* im_max_element_pos_suite(Suite *s);
Suite* im_make_square_suite(Suite *s);
Suite* im_increase_to_size_suite(Suite *s);

Suite *main_suite(void)
{
    Suite *s = suite_create("main_suite");

    s = im_fill_identity_suite(s);
    s = im_fill_multiply_suite(s);
    s = im_fill_by_matrix_suite(s);
    s = im_delete_col_suite(s);
    s = im_delete_row_suite(s);
    s = im_max_element_pos_suite(s);
    s = im_make_square_suite(s);
    s = im_increase_to_size_suite(s);

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