#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "quicksert.h"
#include "highprecisiontimer.h"
#include "input.h"
#include "stats.h"

int *generate_datasets(int number_count, int sample_count)
{
    int *datasets = (int *)malloc(sample_count * number_count * sizeof(int));

    for (int i = 0; i < sample_count; i++)
    {
        for (int j = 0; j < number_count; j++)
        {
            datasets[i * number_count + j] = rand();
        }
    }

    return datasets;
}

void copy_dataset(int *target, int *datasets, int number_count, int sample_index)
{
    for (int i = 0; i < number_count; i++)
    {
        target[i] = datasets[sample_index * number_count + i];
    }
}


double measure_quicksort(int *dataset, int n)
{
    Timer timer = start_timer();
    quicksort(dataset, 0, n - 1);
    stop_timer(&timer);

    return get_timer_nanoseconds(&timer) / 1e6; 
}

double measure_quicksert(int *dataset, int n, int threshold)
{
    Timer timer = start_timer();
    quicksert(dataset, 0, n - 1, threshold);
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

    fprintf(stderr, "N: %d, Samples: %d, Threshold: [%d, %d], Seed: %d\n", number_count, sample_count, threshold_min, threshold_max, seed);

    double *samples = malloc(sample_count * sizeof(double));

    int *datasets = generate_datasets(number_count, sample_count);
    int *test = (int *)malloc(number_count * sizeof(int));

    srand(seed);

    for (int i = 0; i < sample_count; i++)
    {
        copy_dataset(test, datasets, number_count, i);
        samples[i] = measure_quicksort(test, number_count);
    }

    Stats quicksort_stats = compute_stats(samples, sample_count);

    fprintf(
        stderr,
        "Quicksort: Min: %.2lf, Max: %.2lf, Avg: %.2lf, Median: %.2lf, Stddev: %.2lf\n",
        quicksort_stats.minimum,
        quicksort_stats.maximum,
        quicksort_stats.average,
        quicksort_stats.median,
        quicksort_stats.standard_deviation
    );

    for (int threshold = threshold_min; threshold <= threshold_max; threshold++)
    {
        for (int i = 0; i < sample_count; i++)
        {
            copy_dataset(test, datasets, number_count, i);
            samples[i] = measure_quicksert(test, number_count, threshold);
        }

        Stats quicksert_stats = compute_stats(samples, sample_count);
    
        fprintf(
            stderr,
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

    free(datasets);
    free(test);

    return 0;
}
