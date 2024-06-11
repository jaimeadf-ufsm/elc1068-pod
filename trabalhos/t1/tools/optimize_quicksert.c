#include <stdio.h>
#include <stdlib.h>

#include "common/high_precision_timer.h"
#include "common/input.h"
#include "common/quicksert.h"

void generate_random_numbers(int *numbers, int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        numbers[i] = rand();
    }
}

double measure_quicksert(int *numbers, int n, int threshold)
{
    Timer timer = start_timer();
    quicksert(numbers, n, threshold);
    stop_timer(&timer);

    return get_timer_units(&timer);
}


int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        fprintf(stderr, "Usage: %s <numbers_count> <samples_count> <threshold_min> <threshold_max> [seed] [output_filename]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int number_count = parse_int_or_exit(argv[1]);
    int sample_count = parse_int_or_exit(argv[2]);
    int threshold_min = parse_int_or_exit(argv[3]);
    int threshold_max = parse_int_or_exit(argv[4]);
    int seed = 0;

    FILE *output_file = stdout;

    if (argc > 5)
    {
        seed = parse_int_or_exit(argv[5]);
    }

    if (argc > 6)
    {
        output_file = fopen(argv[6], "w");

        if (output_file == NULL)
        {
            fprintf(stderr, "Error: could not open output file.\n");
            return EXIT_FAILURE;
        }
    }

    fprintf(output_file, "N: %d, Samples: %d, Threshold: [%d, %d], Seed: %d\n", number_count, sample_count, threshold_min, threshold_max, seed);

    int *datasets = (int *)malloc(sample_count * number_count * sizeof(int));
    int *playground = (int *)malloc(number_count * sizeof(int));

    fprintf(stderr, "DEBUG: generating random numbers...\n");

    srand(seed);
    generate_random_numbers(datasets, sample_count * number_count);

    int threshold;

    for (threshold = threshold_min; threshold <= threshold_max; threshold++)
    {
        fprintf(stderr, "DEBUG: sampling threshold %d...\n", threshold);
        fprintf(output_file, "%06d", threshold);

        int j;

        for (j = 0; j < sample_count; j++)
        {
            memcpy(playground, datasets + j * number_count, number_count * sizeof(int));
            fprintf(output_file, " %lf", measure_quicksert(playground, number_count, threshold));
        }

        fprintf(output_file, "\n");
    }

    if (output_file != stdout)
    {
        fclose(output_file);
    }

    free(datasets);
    free(playground);

    return 0;
}