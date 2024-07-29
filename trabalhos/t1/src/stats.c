#include <math.h>
#include <stdlib.h>

#include "common/stats.h"

int compare_samples(const void *a, const void *b)
{
    double x = *(double *)a;
    double y = *(double *)b;

    if (x < y)
    {
        return -1;
    }
    
    if (x > y)
    {
        return 1;
    }

    return 0;
}

Stats compute_stats(double *samples, long long n)
{
    Stats stats;
    stats.minimum = HUGE_VAL;
    stats.maximum = -HUGE_VAL;
    stats.average = 0;
    stats.median = 0;
    stats.standard_deviation = 0;

    if (n == 0)
    {
        return stats;
    }

    qsort(samples, n, sizeof(double), compare_samples);

    stats.minimum = samples[0];
    stats.maximum = samples[n - 1];

    long long i;

    for (i = 0; i < n; i++)
    {
        stats.average += samples[i];
    }

    stats.average /= n;

    if (n % 2 == 0)
    {
        stats.median = (samples[n / 2 - 1] + samples[n / 2]) / 2;
    }
    else
    {
        stats.median = samples[n / 2];
    }

    int j;

    for (j = 0; j < n; j++)
    {
        stats.standard_deviation += (samples[j] - stats.average) * (samples[j] - stats.average);
    }

    stats.standard_deviation = sqrt(stats.standard_deviation / n);

    return stats;
}

double compute_percentange_change(double value, double reference)
{
    return (value - reference) * 100.0 / reference;
}