#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdbool.h>

typedef struct csv CSV;

struct csv
{
    FILE *file;

    char delimiter;
    char quote;

    char *line;
    char *field;

    size_t line_position;
    size_t line_capacity;
};

CSV csv_open(const char *filename, char delimiter, char quote);
void csv_close(CSV *csv);

bool csv_next_record(CSV *csv);
char *csv_next_field(CSV *csv);

#endif
