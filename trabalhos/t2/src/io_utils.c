
#include <stdlib.h>

#include "common/io_utils.h"

#define INITIAL_CAPACITY 64

int64_t readline(char **lineptr, size_t *n, FILE *stream)
{
    return readdelim(lineptr, n, '\n', stream);
}

int64_t readdelim(char **lineptr, size_t *n, int delim, FILE *stream)
{
    int character;
    size_t position = 0;

    if (*lineptr == NULL)
    {
        *n = 0;
    }

    while ((character = fgetc(stream)) != EOF)
    {
        if (position + 2 > *n)
        {
            *n = *n == 0 ? INITIAL_CAPACITY : *n * 2;
            *lineptr = (char *)realloc(*lineptr, *n);
        }

        (*lineptr)[position++] = character;

        if (character == delim)
        {
            (*lineptr)[position] = '\0';
            return position;
        }
    }

    return -1;
}