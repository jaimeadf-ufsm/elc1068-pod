#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "common/input.h"

bool is_size(char *str)
{
    return isalpha(str[strlen(str) - 1]);
}

bool parse_bool_or_exit(char *str)
{
    if (strcmp(str, "true") == 0)
    {
        return true;
    }
    else if (strcmp(str, "false") == 0)
    {
        return false;
    }
    else
    {
        fprintf(stderr, "ERROR: invalid boolean value (expected \"true\" or \"false\", but got \"%s\").\n", str);
        exit(EXIT_FAILURE);
    }
}

int parse_int_or_exit(char *str)
{
    int result;

    if (sscanf(str, "%d", &result) != 1)
    {
        fprintf(stderr, "ERROR: cannot convert \"%s\" to integer.\n", str);
        exit(EXIT_FAILURE);
    }

    return result;
}

long long parse_long_long_or_exit(char *str)
{
    long long result;

    if (sscanf(str, "%lld", &result) != 1)
    {
        fprintf(stderr, "ERROR: cannot convert \"%s\" to long long.\n", str);
        exit(EXIT_FAILURE);
    }

    return result;
}

long long parse_size_or_exit(char *str)
{
    long long amount = parse_long_long_or_exit(str);
    char unit = str[strlen(str) - 1];

    switch (unit)
    {
    case 'B':
        return amount;
    case 'K':
        return amount * 1024;
    case 'M':
        return amount * 1024 * 1024;
    case 'G':
        return amount * 1024 * 1024 * 1024;
    default:
        fprintf(stderr, "ERROR: invalid size unit \"%c\".\n", unit);
        exit(EXIT_FAILURE);
    }
}