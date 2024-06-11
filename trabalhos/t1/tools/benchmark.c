#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/high_precision_timer.h"
#include "common/quicksert.h"
#include "common/input.h"
#include "common/stats.h"
#include "common/mergix.h"

int compare(const void *a, const void *b)
{
    int int_a = *((int *)a);
    int int_b = *((int *)b);

    if (int_a == int_b)
    {
        return 0;
    }
    else if (int_a < int_b)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

bool is_sorted(int *array, int *result, long long size)
{
    int *expected_result = (int *)malloc(size * sizeof(int));

    memcpy(expected_result, array, size * sizeof(int));
    qsort(expected_result, size, sizeof(int), compare);

    long long i;

    for (i = 0; i < size; i++)
    {
        if (result[i] != expected_result[i])
        {
            break;
        }
    }

    free(expected_result);

    return i == size;
}

double measure_quicksort(int *array, long long n)
{
    Timer timer = start_timer();
    quicksort(array, n);
    stop_timer(&timer);

    return get_timer_units(&timer);
}

double measure_quicksert(int *array, long long n, long long threshold)
{
    Timer timer = start_timer();
    quicksert(array, n, threshold);
    stop_timer(&timer);

    return get_timer_units(&timer);
}

double measure_mergix(int *array, long long n)
{
    Timer timer = start_timer();
    mergix_call(array, n);
    stop_timer(&timer);

    return get_timer_units(&timer);
}

double measure_algorithm(int *playground, long long size, char *algorithm_type, long long parameter)
{
    if (strcmp(algorithm_type, "quicksort") == 0)
    {
        return measure_quicksort(playground, size);
    }
    else if (strcmp(algorithm_type, "quicksert") == 0)
    {
        return measure_quicksert(playground, size, parameter);
    }
    else if (strcmp(algorithm_type, "mergix") == 0)
    {
        return measure_mergix(playground, size);
    }
    else
    {
        fprintf(stderr, "ERROR: invalid algorithm: \"%s\".\n", algorithm_type);
        exit(EXIT_FAILURE);
    }
}

void generate_ascending_sequences(int *sequences, long long sequence_count, long long sequence_size)
{
    long long i, j;

    for (i = 0; i < sequence_count; i++)
    {
        for (j = 0; j < sequence_size; j++)
        {
            sequences[i * sequence_size + j] = j;
        }
    }
}

void generate_descending_sequences(int *sequences, long long sequence_count, long long sequence_size)
{
    long long i, j;

    for (i = 0; i < sequence_count; i++)
    {
        for (j = 0; j < sequence_size; j++)
        {
            sequences[i * sequence_size + j] = sequence_size - j;
        }
    }
}

void generate_random_sequences(int *sequences, long long sequence_count, long long sequence_size, long long modulo)
{
    srand(0);

    long long i, j;

    for (i = 0; i < sequence_count; i++)
    {
        for (j = 0; j < sequence_size; j++)
        {
            sequences[i * sequence_size + j] = modulo > 0 ? rand() % modulo : rand();
        }
    }
}

void generate_sequences(int *sequences, long long sequence_count, long long sequence_size, char *sequence_type, long long parameter)
{
    if (strcmp(sequence_type, "ascending") == 0)
    {
        generate_ascending_sequences(sequences, sequence_count, sequence_size);
    }
    else if (strcmp(sequence_type, "descending") == 0)
    {
        generate_descending_sequences(sequences, sequence_count, sequence_size);
    }
    else if (strcmp(sequence_type, "random") == 0)
    {
        generate_random_sequences(sequences, sequence_count, sequence_size, parameter);
    }
    else
    {
        fprintf(stderr, "ERROR: invalid sequence: \"%s\".\n", sequence_type);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 7)
    {
        fprintf(
            stderr,
            "Usage: %s <minimum_size> <maximum_size> <increment> <sequence_count> <sequence_type:(ascending, descending, random)> <algorithm_type:(quicksort, quicksert, mergix)> <output_filename> [verify] [sequence_parameter] [algorithm_parameter]\n",
            argv[0]);

        return EXIT_FAILURE;
    }

    long long minimum_size = parse_long_long_or_exit(argv[1]);
    long long maximum_size = parse_long_long_or_exit(argv[2]);
    long long increment = parse_long_long_or_exit(argv[3]);
    long long sequence_count = parse_long_long_or_exit(argv[4]);
    char *sequence_type = argv[5];
    char *algorithm_type = argv[6];
    char *output_filename = argv[7];
    bool verify = false;
    long long sequence_parameter = 0;
    long long algorithm_parameter = 0;

    if (argc > 8)
    {
        verify = parse_bool_or_exit(argv[8]);
    }

    if (argc > 9)
    {
        sequence_parameter = parse_long_long_or_exit(argv[9]);
    }

    if (argc > 10)
    {
        algorithm_parameter = parse_long_long_or_exit(argv[10]);
    }

    FILE *output_file = fopen(output_filename, "w");

    if (output_file == NULL)
    {
        fprintf(stderr, "ERROR: could not open file \"%s\".\n", output_filename);
        return EXIT_FAILURE;
    }

    fprintf(
        output_file,
        "N: [%lld, %lld], Increment: %lld, Samples: %lld, Sequence: %s, Algorithm: %s, Sequence Parameter: %lld, Algorithm Parameter: %lld\n",
        minimum_size,
        maximum_size,
        increment,
        sequence_count,
        sequence_type,
        algorithm_type,
        sequence_parameter,
        algorithm_parameter);

    int *sequences = (int *)malloc(maximum_size * sequence_count * sizeof(int));
    int *working_array = (int *)malloc(maximum_size * sizeof(int));

    double *samples = (double *)malloc(sequence_count * sizeof(double));

    generate_sequences(sequences, sequence_count, maximum_size, sequence_type, sequence_parameter);

    long long size;

    for (size = minimum_size; size <= maximum_size; size += increment)
    {
        long long i;

        for (i = 0; i < sequence_count; i++)
        {
            memcpy(working_array, sequences + i * maximum_size, size * sizeof(int));

            samples[i] = measure_algorithm(working_array, size, algorithm_type, algorithm_parameter);

            if (i == 0)
            {
                fprintf(output_file, "%lf", samples[i]);
            }
            else
            {
                fprintf(output_file, ",%lf", samples[i]);
            }

            if (verify && !is_sorted(sequences + i * maximum_size, working_array, size))
            {
                fprintf(stderr, "ERROR: sequence is not sorted.\n");
                return EXIT_FAILURE;
            }
        }

        fprintf(output_file, "\n");

        Stats stats = compute_stats(samples, sequence_count);

        printf(
            "%s (%lld): Size: %lld, Min: %.2lf, Max: %.2lf, Avg: %.2lf, Median: %.2lf, Stddev: %.2lf\n",
            algorithm_type,
            algorithm_parameter,
            size,
            stats.minimum,
            stats.maximum,
            stats.average,
            stats.median,
            stats.standard_deviation);
    }

    free(sequences);
    free(working_array);
    free(samples);

    fclose(output_file);

    return 0;
}