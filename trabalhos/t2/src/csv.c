#include <stdlib.h>

#include "common/csv.h"

#define DELIMITER ';'

CSV csv_open(const char *filename)
{
    CSV csv;
    csv.file = fopen(filename, "r");

    if (csv.file == NULL)
    {
        fprintf(stderr, "ERRO: não foi possível abrir o arquivo \"%s\".\n", filename);
        exit(EXIT_FAILURE);
    }
    
    csv.line = NULL;
    csv.line_capacity = 0;
    csv.line_position = 0;

    return csv;
}

void csv_close(CSV *csv)
{
    fclose(csv->file);
 
    if (csv->line != NULL)
    {
        free(csv->line);
    }
}

bool csv_next_record(CSV *csv)
{
    if (getline(&csv->line, &csv->line_capacity, csv->file) == -1)
    {
        return false;
    }

    csv->line_position = 0;

    return true;
}

char *csv_next_field(CSV *csv)
{
    if (csv->line == NULL)
    {
        return NULL;
    }

    if (csv->line[csv->line_position] == '\0')
    {
        return NULL;
    }

    char *field = &csv->line[csv->line_position];

    while (csv->line[csv->line_position] != DELIMITER && csv->line[csv->line_position] != '\n')
    {
        csv->line_position++;
    }

    csv->line[csv->line_position++] = '\0';

    return field;
}
