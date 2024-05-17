#include <stdio.h>
#include <stdbool.h>

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int *array, int low, int high)
{
    int pivot = array[(low + high) / 2];

    int i = low - 1;
    int j = high + 1;

    while (true)
    {
        do
        {
            i++;
        } while (array[i] < pivot);

        do
        {
            j--;
        } while (array[j] > pivot);

        if (i >= j)
        {
            return j;
        }

        swap(&array[i], &array[j]);
    }
}

void quicksort(int *array, int low, int high)
{
    if (low >= high)
    {
        return;
    }

    int p = partition(array, low, high);

    quicksort(array, low, p);
    quicksort(array, p + 1, high);
}

void insertionsort(int *array, int low, int high)
{
    for (int i = low + 1; i <= high; i++)
    {
        for (int j = i; j > 0 && array[j - 1] > array[j]; j--)
        {
            swap(&array[j - 1], &array[j]);
        }
    }
}

void quicksert(int *array, int low, int high, int threshold)
{
    int n = high - low + 1;

    if (n <= threshold)
    {
        insertionsort(array, low, high);   
    }
    else
    {
        int p = partition(array, low, high);

        quicksert(array, low, p, threshold);
        quicksert(array, p + 1, high, threshold);
    }
}