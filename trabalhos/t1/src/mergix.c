#include <stdlib.h>
#include <stdio.h>

void radixsort(int *array, int size)
{
    int i, exp = 1;
    int max = array[0];
    int *output = (int *)malloc(size * sizeof(int));

    for (i = 1; i < size; i++)
        if (array[i] > max)
            max = array[i];

    while (max / exp > 0)
    {
        int bucket[10] = {0};

        for (i = 0; i < size; i++)
            bucket[(array[i] / exp) % 10]++;

        for (i = 1; i < 10; i++)
            bucket[i] += bucket[i - 1];

        for (i = size - 1; i >= 0; i--)
            output[--bucket[(array[i] / exp) % 10]] = array[i];

        for (i = 0; i < size; i++)
            array[i] = output[i];

        exp *= 10;
    }

    free(output);
}

void merge(int *array, int left, int mid, int right)
{
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = array[left + i];
    for (j = 0; j < n2; j++)
        R[j] = array[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            array[k] = L[i++];
        else
            array[k] = R[j++];
        k++;
    }

    while (i < n1)
        array[k++] = L[i++];

    while (j < n2)
        array[k++] = R[j++];

    free(L);
    free(R);
}

void mergix(int *array, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergix(array, left, mid);
        mergix(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}

void sortNegatives(int *array, int size)
{
    for (int i = 0; i < size; i++)
        array[i] = -array[i];
    radixsort(array, size);
    for (int i = 0; i < size / 2; i++)
    {
        int temp = array[i];
        array[i] = array[size - 1 - i];
        array[size - 1 - i] = temp;
    }
    for (int i = 0; i < size; i++)
        array[i] = -array[i];
}

void radixSortNegatives(int *array, int size)
{
    int positiveCount = 0, negativeCount = 0;
    int *positives = (int *)malloc(size * sizeof(int));
    int *negatives = (int *)malloc(size * sizeof(int));

    for (int i = 0; i < size; i++)
    {
        if (array[i] >= 0)
            positives[positiveCount++] = array[i];
        else
            negatives[negativeCount++] = array[i];
    }

    radixsortNegatives(positives, positiveCount);
    sortNegatives(negatives, negativeCount);

    for (int i = 0; i < negativeCount; i++)
        array[i] = negatives[i];
    for (int i = 0; i < positiveCount; i++)
        array[negativeCount + i] = positives[i];

    free(positives);
    free(negatives);
}
