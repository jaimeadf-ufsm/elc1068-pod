#include <stdlib.h>

#include "common/csv.h"

CSV csv_open(const char *filename, char delimiter, char quote)
{
    CSV csv;
    csv.file = fopen(filename, "r");

    if (csv.file == NULL)
    {
        fprintf(stderr, "ERRO: não foi possível abrir o arquivo \"%s\".\n", filename);
        exit(EXIT_FAILURE);
    }
    
    csv.line = NULL;
    csv.field = NULL;

    csv.delimiter = delimiter;
    csv.quote = quote;

    csv.line_capacity = 0;

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
    csv->field = realloc(csv->field, csv->line_capacity);

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

    size_t field_position = 0;
    bool quoted = false;

    while (true)
    {
        if (quoted)
        {
            if (csv->line[csv->line_position] == csv->quote)
            {
                csv->line_position++;

                if (csv->line[csv->line_position] == csv->quote)
                {
                    csv->field[field_position++] = csv->quote;
                    csv->line_position++;
                }
                else
                {
                    quoted = false;
                }

                continue;
            }

            if (csv->line[csv->line_position] == '\0')
            {
                printf("ERRO: fim de linha inesperado ao tentar ler campo do CSV: %s.\n", csv->line);
            }

            csv->field[field_position++] = csv->line[csv->line_position++];

            continue;
        }

        if (csv->line[csv->line_position] == csv->delimiter || csv->line[csv->line_position] == '\n')
        {
            csv->line_position++;
            break;
        }

        if (csv->line[csv->line_position] == csv->quote)
        {
            quoted = true;
            csv->line_position++;
            continue;
        }

        csv->field[field_position++] = csv->line[csv->line_position++];
    }

    csv->field[field_position] = '\0';

    return csv->field;
}
