#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "quicksert.h"

#define MODULO 1000

#define TEST_COUNT sizeof(TESTS) / sizeof(int)
#define TEST_MAX 10000000

#define THRESHOLD_MIN 2
#define THRESHOLD_MAX 999

typedef struct record Record;

struct record
{
    int test;
    int threshold;

    clock_t quicksort_time;
    clock_t quicksert_time;
};

const int TESTS[] = {
    10,
    100,
    1000,
    10000,
    100000,
    200000,
    300000,
    400000,
    500000,
    1000000,
    2000000,
    3000000,
    4000000,
    5000000,
    6000000,
    7000000,
    8000000,
    9000000,
    10000000
};

int random_array[TEST_MAX];
int test_array[TEST_MAX];

Record records[TEST_COUNT];

void fill_random_array()
{
    srand(0);

    for (int i = 0; i < TEST_MAX; i++)
    {
        random_array[i] = rand() % MODULO;
    }
}

void fill_test_array(int n)
{
    for (int i = 0; i < n; i++)
    {
        test_array[i] = random_array[i];
    }
}

clock_t measure_quicksort(int n)
{
    fill_test_array(n);

    clock_t start = clock();
    quicksort(test_array, 0, n - 1);
    clock_t end = clock();

    return end - start;
}

clock_t measure_quicksert(int n, int threshold)
{
    fill_test_array(n);

    clock_t start = clock();
    quicksert(test_array, 0, n - 1, threshold);
    clock_t end = clock();

    return end - start;
}

double calculate_variance_percentage(clock_t a, clock_t b)
{
    if (b == 0)
    {
        return 0.0;
    }

    return (a - b) * 100.0 / b;
}


int main()
{
    fill_random_array();

    printf("Running quicksort benchmarks...\n");

    for (int i = 0; i < TEST_COUNT; i++)
    {
        records[i].test = TESTS[i];
        records[i].quicksort_time = measure_quicksort(TESTS[i]);
        records[i].quicksert_time = LONG_MAX;

        printf("N: %8d, Quicksort: %ld\n", TESTS[i], records[i].quicksort_time);
    }

    printf("\nRunning quicksert benchmarks...\n");

    for (int threshold = THRESHOLD_MIN; threshold <= THRESHOLD_MAX; threshold++)
    {
        for (int i = 0; i < TEST_COUNT; i++)
        {
            clock_t quicksert_time = measure_quicksert(TESTS[i], threshold);

            printf(
                "N: %8d, Quicksert(%3d): %ld (%lf%%)\n",
                TESTS[i], threshold,
                quicksert_time,
                calculate_variance_percentage(quicksert_time, records[i].quicksort_time)
            );

            if (quicksert_time < records[i].quicksert_time)
            {
                records[i].threshold = threshold;
                records[i].quicksert_time = quicksert_time;
            }
        }
    }

    printf("\n\nResults:\n");

    for (int i = 0; i < TEST_COUNT; i++)
    {
        printf(
            "N: %8d, Quicksort: %ld, Quicksert(%3d): %ld (%lf%%)\n",
            records[i].test,
            records[i].quicksort_time,
            records[i].threshold,
            records[i].quicksert_time,
            calculate_variance_percentage(records[i].quicksort_time, records[i].quicksert_time)
        );
    }

    return 0;
}