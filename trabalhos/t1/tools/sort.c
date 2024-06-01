#include <stdio.h>
#include <stdlib.h>

#include "common/input.h"
#include "common/external_sort.h"

int compare(const void *a, const void *b)
{
    int *int_a = (int *)a;
    int *int_b = (int *)b;

    return *int_a - *int_b;
}

void sort(int *array, int n)
{
    qsort(array, n, sizeof(int), compare);
}

int main(int argc, char **argv)
{
    if (argc < 5)
    {
        printf("Usage: %s <input_filename> <output_filename> <run_count> <run_size> <buffer_size>\n", argv[0]);
        return 1;
    }

    char *input_filename = argv[1];
    char *output_filename = argv[2];

    int run_count = parse_int_or_exit(argv[3]);
    int run_size = parse_int_or_exit(argv[4]);
    int buffer_size = parse_int_or_exit(argv[5]);

    sort_files(input_filename, output_filename, run_count, run_size, sort, buffer_size);

    return 0;
}