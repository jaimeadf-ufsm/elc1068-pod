#ifndef STATS_H
#define STATS_H

typedef struct stats Stats;

struct stats
{
    double minimum;
    double maximum;
    double average;
    double median;
    double standard_deviation;
};

Stats compute_stats(double *samples, long long n);
double compute_percentange_change(double value, double reference);

#endif