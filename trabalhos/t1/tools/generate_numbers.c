#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "common/input.h"
#include "common/buffered_io.h"

bool is_size(char *str)
{
    return isalpha(str[strlen(str) - 1]);
}

long long parse_size_or_exit(char *str)
{
    long long amount = parse_long_long_or_exit(str);
    char unit = str[strlen(str) - 1];

    switch (unit)
    {
    case 'K':
        return amount * 1024;
    case 'M':
        return amount * 1024 * 1024;
    case 'G':
        return amount * 1024 * 1024 * 1024;
    case 'B':
        return amount;
    default:
        fprintf(stderr, "ERROR: invalid size unit \"%c\".\n", unit);
        exit(1);
    }
}

long long generate_random_numbers_by_count(BufferedWriter *writer, long long count, int modulo)
{
    for (long long i = 0; i < count; i++)
    {
        write_number(writer, rand() % modulo);
    }

    return count;
}

long long generate_random_numbers_by_size(BufferedWriter *writer, long long size, int modulo)
{
    long long file_size = 0;
    long long count = 0;

    while (file_size < size)
    {
        write_number(writer, rand() % modulo);
        file_size = ftell(writer->file);
        count++;
    }

    return count;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <count/size> <filename> [modulo]\n", argv[0]);
        return 1;
    }

    char *amount = argv[1];
    char *filename = argv[2];

    int modulo = RAND_MAX;

    if (argc >= 4)
    {
        modulo = parse_int_or_exit(argv[3]);
    }

    srand(time(NULL));

    BufferedWriter output = open_writer(filename, 8192); // 8KB
    long long count = 0;

    if (is_size(amount))
    {
        count = generate_random_numbers_by_size(&output, parse_size_or_exit(amount), modulo);
    }
    else
    {
        count = generate_random_numbers_by_count(&output, parse_long_long_or_exit(amount), modulo);
    }

    close_writer(&output);
    fprintf(stderr, "DEBUG: generated %lld numbers.\n", count);

    return 0;
}
