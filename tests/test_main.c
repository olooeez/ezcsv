#include <check.h>

extern TCase *ezcsv_parser_tests(void);

int main(void)
{
    Suite *s;
    SRunner *sr;

    int failed_count;

    s = suite_create("EZCSV Tests");

    suite_add_tcase(s, ezcsv_parser_tests());

    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);

    failed_count = srunner_ntests_failed(sr);

    srunner_free(sr);

    return failed_count;
}
