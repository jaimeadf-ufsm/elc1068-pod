#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <stdio.h>
#include <inttypes.h>

int64_t readline(char **lineptr, size_t *n, FILE *stream);
int64_t readdelim(char **lineptr, size_t *n, int delim, FILE *stream);

#endif