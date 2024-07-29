#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdbool.h>

typedef struct csv CSV;

struct csv
{
    FILE *file;

    char *line;

    size_t line_capacity;
    size_t line_position;
};

CSV csv_open(const char *filename);
void csv_close(CSV *csv);

bool csv_next_record(CSV *csv);
char *csv_next_field(CSV *csv);

#endif
