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

#include <stdbool.h>

bool is_sorted(char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        return false;
    }

    int prev_num, curr_num;
    bool first_num = true;

    while (fscanf(file, "%d", &curr_num) == 1)
    {
        if (!first_num && curr_num < prev_num)
        {
            fclose(file);
            return false;
        }

        prev_num = curr_num;
        first_num = false;
    }

    fclose(file);
    return true;
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

    // debug
    bool sorted_output = is_sorted(output_filename);
    if (sorted_output)
    {
        printf("true\n");
    }
    else
    {
        printf("false\n");
    }

    return 0;
}