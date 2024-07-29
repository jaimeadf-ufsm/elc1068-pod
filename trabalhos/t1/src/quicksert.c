#include <stdbool.h>

long long quicksort_partition(int *array, long long low, long long high)
{
    int pivot = array[(low + high) / 2];

    long long i = low - 1;
    long long j = high + 1;

    while (true)
    {
        do
        {
            i++;
        } while (i < high && array[i] < pivot);

        do
        {
            j--;
        } while (j > low && array[j] > pivot);

        if (i >= j)
        {
            return j;
        }

        int temporary = array[i];

        array[i] = array[j];
        array[j] = temporary;
    }
}

void quicksort_recursion(int *array, long long low, long long high)
{
    if (low < high)
    {
        long long p = quicksort_partition(array, low, high);

        quicksort_recursion(array, low, p);
        quicksort_recursion(array, p + 1, high);
    }
}

void quicksert_recursion(int *array, long long low, long long high, long long threshold)
{
    long long n = high - low + 1;

    if (low < high && n > threshold)
    {
        long long p = quicksort_partition(array, low, high);

        quicksert_recursion(array, low, p, threshold);
        quicksert_recursion(array, p + 1, high, threshold);
    }
}

void insertionsort(int *array, long long n)
{
    long long i;

    for (i = 1; i < n; i++)
    {
        long long j = i;
        int temporary = array[i];

        while (j > 0 && array[j - 1] > temporary)
        {
            array[j] = array[j - 1];
            j--;
        }

        array[j] = temporary;
    }
}

void quicksort(int *array, long long n)
{
    quicksort_recursion(array, 0, n - 1);
}

void quicksert(int *array, long long n, long long threshold)
{
    quicksert_recursion(array, 0, n - 1, threshold);
    insertionsort(array, n);
}