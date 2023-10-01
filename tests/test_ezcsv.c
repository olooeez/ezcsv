#define _XOPEN_SOURCE 500

#include <check.h>
#include <stdlib.h>
#include <string.h>
#include "ezcsv.h"

#define FAKE_DATA "./data/fake.csv"

START_TEST(test_csv_parser_create_success)
{
    const char *filename = FAKE_DATA;
    char delimiter = ',';

    CSVParser *parser = csv_parser_create(filename, delimiter);

    ck_assert_ptr_ne(parser, NULL);
    ck_assert_int_eq(csv_get_last_error(), CSV_SUCCESS);

    csv_parser_destroy(parser);
}
END_TEST

START_TEST(test_csv_parser_create_file_open_failure)
{
    const char *filename = "nonexistent.csv";
    char delimiter = ',';

    CSVParser *parser = csv_parser_create(filename, delimiter);

    ck_assert_ptr_eq(parser, NULL);
    ck_assert_int_eq(csv_get_last_error(), CSV_ERROR_FILE_OPEN);
}
END_TEST

START_TEST(test_csv_parser_destroy_success)
{
    CSVParser *parser = csv_parser_create(FAKE_DATA, ',');

    csv_parser_destroy(parser);

    ck_assert_int_eq(csv_get_last_error(), CSV_SUCCESS);
}
END_TEST

START_TEST(test_csv_parser_destroy_null_argument)
{
    CSVParser *parser = NULL;

    csv_parser_destroy(parser);

    ck_assert_int_eq(csv_get_last_error(), CSV_ERROR_INVALID_ARG);
}
END_TEST

START_TEST(test_csv_read_row_success)
{
    CSVParser *parser = csv_parser_create(FAKE_DATA, ',');
    CSVRow *row;

    row = csv_read_row(parser);

    ck_assert_ptr_ne(row, NULL);
    ck_assert_int_eq(csv_get_last_error(), CSV_SUCCESS);

    csv_row_destroy(row);
    csv_parser_destroy(parser);
}
END_TEST

START_TEST(test_csv_read_row_field_storage)
{
    CSVParser *parser = csv_parser_create(FAKE_DATA, ',');
    CSVRow *row;

    row = csv_read_row(parser);

    ck_assert_ptr_ne(row, NULL);
    ck_assert_int_eq(csv_get_last_error(), CSV_SUCCESS);

    ck_assert_int_eq(row->field_count, 3);

    ck_assert_str_eq(row->fields[0], "Name");
    ck_assert_str_eq(row->fields[1], "Age");
    ck_assert_str_eq(row->fields[2], "Location\n");

    csv_row_destroy(row);
    csv_parser_destroy(parser);
}
END_TEST

START_TEST(test_csv_read_row_invalid_argument)
{
    CSVParser *parser = NULL;
    CSVRow *row;

    row = csv_read_row(parser);

    ck_assert_ptr_eq(row, NULL);
    ck_assert_int_eq(csv_get_last_error(), CSV_ERROR_INVALID_ARG);
}
END_TEST

START_TEST(test_csv_row_destroy_success)
{
    CSVParser *parser = csv_parser_create(FAKE_DATA, ',');
    CSVRow *row = csv_read_row(parser);

    ck_assert_ptr_ne(row, NULL);

    csv_row_destroy(row);

    ck_assert_int_eq(csv_get_last_error(), CSV_SUCCESS);
}
END_TEST

START_TEST(test_csv_row_destroy_null_argument)
{
    CSVRow *row = NULL;

    csv_row_destroy(row);

    ck_assert_int_eq(csv_get_last_error(), CSV_ERROR_INVALID_ARG);
}
END_TEST

START_TEST(test_csv_parse_string_success)
{
    const char *csv_string = "John Doe,30,New York";
    char delimiter = ',';
    CSVRow *row;

    row = csv_parse_string(csv_string, delimiter);

    ck_assert_ptr_ne(row, NULL);
    ck_assert_int_eq(csv_get_last_error(), CSV_SUCCESS);

    ck_assert_int_eq(row->field_count, 3);
    ck_assert_str_eq(row->fields[0], "John Doe");
    ck_assert_str_eq(row->fields[1], "30");
    ck_assert_str_eq(row->fields[2], "New York");

    csv_row_destroy(row);
}
END_TEST

START_TEST(test_csv_parse_string_invalid_argument)
{
    const char *csv_string = NULL;
    char delimiter = ',';
    CSVRow *row;

    row = csv_parse_string(csv_string, delimiter);

    ck_assert_ptr_eq(row, NULL);
    ck_assert_int_eq(csv_get_last_error(), CSV_ERROR_INVALID_ARG);
}
END_TEST

START_TEST(test_csv_write_file_success)
{
    const char *filename = "output.csv";
    char delimiter = ',';

    const char *field_values[] = {"John Doe", "30", "New York"};

    CSVRow *row = csv_row_create(3, field_values);

    ck_assert_ptr_ne(row, NULL);

    int result = csv_write_file(row, filename, delimiter);

    ck_assert_int_eq(result, 1);
    ck_assert_int_eq(csv_get_last_error(), CSV_SUCCESS);

    csv_row_destroy(row);
    remove(filename);
}
END_TEST

START_TEST(test_csv_write_file_invalid_argument)
{
    const char *filename = "output.csv";
    char delimiter = ',';
    CSVRow *row = NULL;

    int result = csv_write_file(row, filename, delimiter);

    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(csv_get_last_error(), CSV_ERROR_INVALID_ARG);
}
END_TEST

START_TEST(test_csv_write_file_empty_row)
{
    const char *filename = "output.csv";
    char delimiter = ',';
    CSVRow *row = malloc(sizeof(CSVRow));

    row->field_count = 0;
    row->fields = NULL;

    int result = csv_write_file(row, filename, delimiter);

    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(csv_get_last_error(), CSV_ERROR_INVALID_ARG);

    csv_row_destroy(row);
}
END_TEST

START_TEST(test_csv_get_field_success)
{
    const char *field_values[] = {"John Doe", "30", "New York"};

    CSVRow *row = csv_row_create(3, field_values);

    ck_assert_ptr_ne(row, NULL);

    char *field = csv_get_field(row, 1);

    ck_assert_ptr_ne(field, NULL);
    ck_assert_str_eq(field, "30");
    ck_assert_int_eq(csv_get_last_error(), CSV_SUCCESS);

    csv_row_destroy(row);
}
END_TEST

START_TEST(test_csv_get_field_invalid_argument)
{
    const char *field_values[] = {"John Doe", "30", "New York"};

    CSVRow *row = csv_row_create(3, field_values);

    ck_assert_ptr_ne(row, NULL);

    char *field = csv_get_field(row, -1);

    ck_assert_ptr_eq(field, NULL);
    ck_assert_int_eq(csv_get_last_error(), CSV_ERROR_INVALID_ARG);

    field = csv_get_field(row, 3);

    ck_assert_ptr_eq(field, NULL);
    ck_assert_int_eq(csv_get_last_error(), CSV_ERROR_INVALID_ARG);

    csv_row_destroy(row);
}
END_TEST

TCase *ezcsv_parser_tests(void)
{
    TCase *tc_core = tcase_create("EZCSV Parser");

    tcase_add_test(tc_core, test_csv_parser_create_success);
    tcase_add_test(tc_core, test_csv_parser_create_file_open_failure);
    tcase_add_test(tc_core, test_csv_parser_destroy_success);
    tcase_add_test(tc_core, test_csv_parser_destroy_null_argument);
    tcase_add_test(tc_core, test_csv_read_row_success);
    tcase_add_test(tc_core, test_csv_read_row_field_storage);
    tcase_add_test(tc_core, test_csv_read_row_invalid_argument);
    tcase_add_test(tc_core, test_csv_row_destroy_success);
    tcase_add_test(tc_core, test_csv_row_destroy_null_argument);
    tcase_add_test(tc_core, test_csv_parse_string_success);
    tcase_add_test(tc_core, test_csv_parse_string_invalid_argument);
    tcase_add_test(tc_core, test_csv_write_file_success);
    tcase_add_test(tc_core, test_csv_write_file_invalid_argument);
    tcase_add_test(tc_core, test_csv_write_file_empty_row);
    tcase_add_test(tc_core, test_csv_get_field_success);
    tcase_add_test(tc_core, test_csv_get_field_invalid_argument);

    return tc_core;
}
