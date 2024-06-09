#include <stdbool.h>

int quicksort_partition(int *array, int low, int high)
{
    int pivot = array[(low + high) / 2];

    int i = low - 1;
    int j = high + 1;

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

void quicksort_recursion(int *array, int low, int high)
{
    if (low < high)
    {
        int p = quicksort_partition(array, low, high);

        quicksort_recursion(array, low, p);
        quicksort_recursion(array, p + 1, high);
    }
}

void quicksert_recursion(int *array, int low, int high, int threshold)
{
    int n = high - low + 1;

    if (low < high && n > threshold)
    {
        int p = quicksort_partition(array, low, high);

        quicksert_recursion(array, low, p, threshold);
        quicksert_recursion(array, p + 1, high, threshold);
    }
}

void insertionsort(int *array, int n)
{
    int i;

    for (i = 1; i < n; i++)
    {
        int j = i;
        int temporary = array[i];

        while (j > 0 && array[j - 1] > temporary)
        {
            array[j] = array[j - 1];
            j--;
        }

        array[j] = temporary;
    }
}

void quicksort(int *array, int n)
{
    quicksort_recursion(array, 0, n - 1);
}

void quicksert(int *array, int n, int threshold)
{
    quicksert_recursion(array, 0, n - 1, threshold);
    insertionsort(array, n);
}