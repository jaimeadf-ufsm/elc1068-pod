#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "quicksert.h"
#include "highprecisiontimer.h"
#include "input.h"
#include "stats.h"

void generate_random_numbers(int *numbers, int n)
{
    for (int i = 0; i < n; i++)
    {
        numbers[i] = rand();
    }
}

double measure_quicksort(int *numbers, int n)
{
    Timer timer = start_timer();
    quicksort(numbers, n);
    stop_timer(&timer);

    return get_timer_nanoseconds(&timer) / 1e6; 
}

double measure_quicksert(int *numbers, int n, int threshold)
{
    Timer timer = start_timer();
    quicksert(numbers, n, threshold);
    stop_timer(&timer);

    return get_timer_nanoseconds(&timer) / 1e6;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <numbers_count> <samples_count> <threshold_min> <threshold_max> [seed]\n", argv[0]);
        exit(1);
    }

    int number_count = parse_int_or_exit(argv[1]);
    int sample_count = parse_int_or_exit(argv[2]);
    int threshold_min = parse_int_or_exit(argv[3]);
    int threshold_max = parse_int_or_exit(argv[4]);
    int seed = 0;

    if (argc > 5)
    {
        seed = parse_int_or_exit(argv[5]);
    }

    printf("N: %d, Samples: %d, Threshold: [%d, %d], Seed: %d\n", number_count, sample_count, threshold_min, threshold_max, seed);

    double *samples = malloc(sample_count * sizeof(double));
    int *numbers = malloc(number_count * sizeof(int));

    srand(seed);

    for (int i = 0; i < sample_count; i++)
    {
        generate_random_numbers(numbers, number_count);
        samples[i] = measure_quicksort(numbers, number_count);
    }

    Stats quicksort_stats = compute_stats(samples, sample_count);

    printf(
        "Quicksort: Min: %.2lf, Max: %.2lf, Avg: %.2lf, Median: %.2lf, Stddev: %.2lf\n",
        quicksort_stats.minimum,
        quicksort_stats.maximum,
        quicksort_stats.average,
        quicksort_stats.median,
        quicksort_stats.standard_deviation
    );

    for (int threshold = threshold_min; threshold <= threshold_max; threshold++)
    {
        srand(seed);

        for (int i = 0; i < sample_count; i++)
        {
            generate_random_numbers(numbers, number_count);
            samples[i] = measure_quicksert(numbers, number_count, threshold);
        }

        Stats quicksert_stats = compute_stats(samples, sample_count);
    
        printf(
            "Quicksert (%d): Min: %.2lf, Max: %.2lf, Avg: %.2lf (%.2lf%%), Median: %.2lf (%.2lf%%), Stddev: %.2lf\n",
            threshold,
            quicksert_stats.minimum,
            quicksert_stats.maximum,
            quicksert_stats.average,
            compute_percentange_change(quicksert_stats.average, quicksort_stats.average),
            quicksert_stats.median,
            compute_percentange_change(quicksert_stats.median, quicksort_stats.median),
            quicksert_stats.standard_deviation
        );
    }

    free(samples);
    free(numbers);

    return 0;
}
