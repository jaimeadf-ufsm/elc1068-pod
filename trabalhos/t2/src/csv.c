#include <stdlib.h>

#include "common/csv.h"
#include "common/io_utils.h"

#define FIELD_CAPACITY 256

void csv_field_append(CSV *csv, char c)
{
    if (csv->field_length + 1 > csv->field_capacity)
    {
        csv->field_capacity *= 2;
        csv->field_array = (char *)realloc(csv->field_array, csv->field_capacity * sizeof(char));
    }

    csv->field_array[csv->field_length++] = c;
}

void csv_field_clear(CSV *csv)
{
    csv->field_length = 0;
}

CSV csv_open(const char *filename, char delimiter, char quote)
{
    CSV csv;
    csv.reader = reader_open(filename, DEFAULT_IO_BUFFER_CAPACITY);

    csv.delimiter = delimiter;
    csv.quote = quote;
    
    csv.field_length = 0;
    csv.field_capacity = FIELD_CAPACITY;
    csv.field_array = (char *)malloc(csv.field_capacity * sizeof(char));

    csv.end_of_record = true;

    return csv;
}

void csv_close(CSV *csv)
{
    reader_close(&csv->reader);
    free(csv->field_array);
}

bool csv_next_record(CSV *csv)
{
    while (csv_next_field(csv) != NULL);
    csv->end_of_record = false;

    return !reader_is_end_of_file(&csv->reader);
}

char *csv_next_field(CSV *csv)
{
    if (csv->end_of_record)
    {
        return NULL;
    }

    csv_field_clear(csv);

    bool quoted = false;
    char c = reader_read_char(&csv->reader);

    while (true)
    {
        if (quoted)
        {
            if (c == csv->quote)
            {
                c = reader_read_char(&csv->reader);

                if (c != csv->quote)
                {
                    quoted = false;
                    continue;
                }
            }

            csv_field_append(csv, c);
            c = reader_read_char(&csv->reader);

            continue;
        }

        if (c == csv->delimiter)
        {
            break;
        }

        if (c == '\n')
        {
            csv->end_of_record = true;
            break;
        }

        if (c != csv->quote)
        {
            csv_field_append(csv, c);
        }
        else
        {
            quoted = true;
        }

        c = reader_read_char(&csv->reader);
    }

    csv_field_append(csv, '\0');

    return csv->field_array;
}
