#ifndef EZCSV_H
#define EZCSV_H

#include <stdio.h>

typedef enum
{
    CSV_SUCCESS,
    CSV_ERROR_MEMORY,
    CSV_ERROR_FILE_OPEN,
    CSV_ERROR_INVALID_ARG,
} CSVError;

typedef struct
{
    char **fields;
    int field_count;
} CSVRow;

typedef struct
{
    FILE *file;
    char delimiter;
} CSVParser;

CSVParser *csv_parser_create(const char *filename, char delimiter);
void csv_parser_destroy(CSVParser *parser);
CSVRow *csv_read_row(CSVParser *parser);
void csv_row_destroy(CSVRow *row);
CSVRow *csv_parse_string(const char *csv_string, char delimiter);
char *csv_write_string(const CSVRow *row, char delimiter);
int csv_write_file(const CSVRow *row, const char *filename, char delimiter);
char *csv_get_field(const CSVRow *row, int field_index);
char **csv_get_headers(const char *csv_string, char delimiter);
void csv_set_delimiter(CSVParser *parser, char new_delimiter);
char csv_get_delimiter(const CSVParser *parser);
const char *csv_error_message(CSVError error);
CSVError csv_get_last_error(void);

#endif
