#include <stdio.h>
#include <stdlib.h>

#include "common/input.h"

int parse_int_or_exit(char *str)
{
    char *end;
    int result = strtol(str, &end, 10);

    if (str == end)
    {
        fprintf(stderr, "ERROR: cannot convert \"%s\" to integer.\n", str);
        exit(1);
    }

    return result;
}

long long parse_long_long_or_exit(char *str)
{
    char *end;
    long long result = strtoll(str, &end, 10);

    if (str == end)
    {
        fprintf(stderr, "ERROR: cannot convert \"%s\" to long long.\n", str);
        exit(1);
    }

    return result;
}