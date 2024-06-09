#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/input.h"
#include "common/external_sort.h"
#include "common/quicksert.h"
#include "common/mergix.h"

#define DEFAULT_BUFFER_SIZE 64 * 1024
#define DEFAULT_TMP_DIR "tmp"

#define QUICKSERT_THRESHOLD 100

void sort_with_quicksort(int *array, int size)
{
    quicksort(array, size);
}

void sort_with_quicksert(int *array, int size)
{
    quicksert(array, size, QUICKSERT_THRESHOLD);
}

void sort_with_mergix(int *array, int size)
{
    mergix(array, 0, size - 1);
}

SortFunction resolve_algorithm(char *argument)
{
    if (strcmp(argument, "quicksort") == 0)
    {
        return sort_with_quicksort;
    }
    else if (strcmp(argument, "quicksert") == 0)
    {
        return sort_with_quicksert;
    }
    else if (strcmp(argument, "mergix") == 0)
    {
        return sort_with_mergix;
    }
    else
    {
        fprintf(stderr, "ERROR: unknown algorithm: %s\n", argument);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    if (argc < 6)
    {
        printf("Usage: %s <input_filename> <output_filename> <run_count> <run_size> <algorithm:(quicksort, quicksert, mergix)> [temporary_directory] [buffer_size]\n", argv[0]);
        return EXIT_FAILURE;
    }

    SortOptions options;

    options.sort = resolve_algorithm(argv[5]);
    options.input_filename = argv[1];
    options.output_filename = argv[2];
    options.tmp_directory = DEFAULT_TMP_DIR;
    options.run_count = parse_int_or_exit(argv[3]);
    options.run_size = parse_int_or_exit(argv[4]);
    options.buffer_size = DEFAULT_BUFFER_SIZE;

    if (argc > 6)
    {
        options.tmp_directory = argv[6];
    }

    if (argc > 7)
    {
        options.buffer_size = parse_int_or_exit(argv[7]);
    }

    sort_files(&options);
    
    return 0;
}