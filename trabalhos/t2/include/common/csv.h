#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdbool.h>

#include "buffered_io.h"

typedef struct csv CSV;

struct csv
{
    BufferedReader reader;

    char delimiter;
    char quote;

    size_t field_length;
    size_t field_capacity;
    char *field_array;

    bool end_of_record;
};

CSV csv_open(const char *filename, char delimiter, char quote);
void csv_close(CSV *csv);

bool csv_next_record(CSV *csv);
char *csv_next_field(CSV *csv);

#endif
