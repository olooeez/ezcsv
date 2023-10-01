#include <stdlib.h>
#include <string.h>
#include "ezcsv.h"

static CSVError last_error;

CSVParser *csv_parser_create(const char *filename, char delimiter)
{
    CSVParser *parser = malloc(sizeof(CSVParser));
    if (!parser)
    {
        last_error = CSV_ERROR_MEMORY;
        return NULL;
    }

    parser->file = fopen(filename, "r");
    if (!parser->file)
    {
        free(parser);
        last_error = CSV_ERROR_FILE_OPEN;
        return NULL;
    }

    parser->delimiter = delimiter;

    last_error = CSV_SUCCESS;

    return parser;
}

void csv_parser_destroy(CSVParser *parser)
{
    if (parser)
    {
        if (parser->file)
        {
            fclose(parser->file);
        }

        free(parser);

        last_error = CSV_SUCCESS;
    }
    else
    {
        last_error = CSV_ERROR_INVALID_ARG;
    }
}

CSVRow *csv_read_row(CSVParser *parser)
{
    if (!parser || !parser->file)
    {
        last_error = CSV_ERROR_INVALID_ARG;
        return NULL;
    }

    CSVRow *row = malloc(sizeof(CSVRow));

    if (!row)
    {
        last_error = CSV_ERROR_MEMORY;
        return NULL;
    }

    char *line = NULL;
    size_t line_length = 0;
    size_t bytes_read = getline(&line, &line_length, parser->file);

    if (bytes_read == -1)
    {
        free(row);
        free(line);
        last_error = CSV_SUCCESS;
        return NULL;
    }

    row->field_count = 0;
    row->fields = NULL;

    char *token = strtok(line, &(parser->delimiter));

    while (token)
    {
        row->field_count++;
        row->fields = realloc(row->fields, sizeof(char *) * row->field_count);

        if (!row->fields)
        {
            free(row);
            free(line);
            last_error = CSV_ERROR_MEMORY;
            return NULL;
        }

        row->fields[row->field_count - 1] = strdup(token);

        if (!row->fields[row->field_count - 1])
        {
            free(row);
            free(line);
            last_error = CSV_ERROR_MEMORY;
            return NULL;
        }

        token = strtok(NULL, &(parser->delimiter));
    }

    free(line);

    last_error = CSV_SUCCESS;

    return row;
}

void csv_row_destroy(CSVRow *row)
{
    if (row)
    {
        for (int i = 0; i < row->field_count; i++)
        {
            free(row->fields[i]);
        }

        free(row->fields);
        free(row);

        last_error = CSV_SUCCESS;
    }
    else
    {
        last_error = CSV_ERROR_INVALID_ARG;
    }
}

CSVRow *csv_parse_string(const char *csv_string, char delimiter)
{
    if (!csv_string)
    {
        last_error = CSV_ERROR_INVALID_ARG;
        return NULL;
    }

    CSVRow *row = malloc(sizeof(CSVRow));

    if (!row)
    {
        last_error = CSV_ERROR_MEMORY;
        return NULL;
    }

    row->field_count = 0;
    row->fields = NULL;

    char *line = strdup(csv_string);

    if (!line)
    {
        free(row);
        last_error = CSV_ERROR_MEMORY;
        return NULL;
    }

    char *token = strtok(line, &(delimiter));

    while (token)
    {
        row->field_count++;
        row->fields = realloc(row->fields, sizeof(char *) * row->field_count);

        if (!row->fields)
        {
            free(line);
            free(row);
            last_error = CSV_ERROR_MEMORY;
            return NULL;
        }

        row->fields[row->field_count - 1] = strdup(token);

        if (!row->fields[row->field_count - 1])
        {
            free(line);
            free(row);
            last_error = CSV_ERROR_MEMORY;
            return NULL;
        }

        token = strtok(NULL, &(delimiter));
    }

    free(line);

    last_error = CSV_SUCCESS;

    return row;
}

int csv_write_file(const CSVRow *row, const char *filename, char delimiter)
{
    if (!row || !row->fields || row->field_count == 0 || !filename)
    {
        last_error = CSV_ERROR_INVALID_ARG;
        return 0;
    }

    FILE *file = fopen(filename, "w");

    if (!file)
    {
        last_error = CSV_ERROR_FILE_OPEN;
        return 0;
    }

    for (int i = 0; i < row->field_count; i++)
    {
        fprintf(file, "%s", row->fields[i]);

        if (i < row->field_count - 1)
        {
            fputc(delimiter, file);
        }
    }

    fclose(file);

    last_error = CSV_SUCCESS;

    return 1;
}

char *csv_get_field(const CSVRow *row, int field_index)
{
    if (!row || field_index < 0 || field_index >= row->field_count)
    {
        last_error = CSV_ERROR_INVALID_ARG;

        return NULL;
    }

    last_error = CSV_SUCCESS;

    return row->fields[field_index];
}

char **csv_get_headers(const char *csv_string, char delimiter)
{
    if (!csv_string)
    {
        last_error = CSV_ERROR_INVALID_ARG;

        return NULL;
    }

    char *line = strdup(csv_string);
    if (!line)
    {
        last_error = CSV_ERROR_MEMORY;

        return NULL;
    }

    char *token = strtok(line, &delimiter);
    int field_count = 0;

    while (token)
    {
        token = strtok(NULL, &delimiter);
        field_count++;
    }

    char **headers = malloc(sizeof(char *) * field_count);
    if (!headers)
    {
        free(line);
        last_error = CSV_ERROR_MEMORY;

        return NULL;
    }

    strcpy(line, csv_string);
    token = strtok(line, &delimiter);

    for (int i = 0; i < field_count; i++)
    {
        headers[i] = strdup(token);
        if (!headers[i])
        {
            for (int j = 0; j < i; j++)
            {
                free(headers[j]);
            }

            free(headers);
            free(line);

            last_error = CSV_ERROR_MEMORY;

            return NULL;
        }

        token = strtok(NULL, &delimiter);
    }

    free(line);

    last_error = CSV_SUCCESS;

    return headers;
}

void csv_set_delimiter(CSVParser *parser, char new_delimiter)
{
    if (parser)
    {
        parser->delimiter = new_delimiter;
        last_error = CSV_SUCCESS;
    }
    else
    {
        last_error = CSV_ERROR_INVALID_ARG;
    }
}

char csv_get_delimiter(const CSVParser *parser)
{
    if (parser)
    {
        last_error = CSV_SUCCESS;
        return parser->delimiter;
    }
    else
    {
        last_error = CSV_ERROR_INVALID_ARG;
        return '\0';
    }
}

const char *csv_error_message(CSVError error)
{
    switch (error)
    {
    case CSV_SUCCESS:
        return "Success";
    case CSV_ERROR_MEMORY:
        return "Memory allocation error";
    case CSV_ERROR_FILE_OPEN:
        return "Unable to open file";
    case CSV_ERROR_INVALID_ARG:
        return "Invalid argument";
    default:
        return "Unknown error";
    }
}

CSVError csv_get_last_error(void)
{
    return last_error;
}
