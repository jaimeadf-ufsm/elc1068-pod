#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int get_max(int *array, int size)
{
    int max = abs(array[0]);
    int i;
    for (i = 1; i < size; i++)
    {
        if (abs(array[i]) > max)
            max = abs(array[i]);
    }
    return max;
}

void radixsort(int *array, int size)
{
    if (size <= 0 || array == NULL)
    {
        printf("Empty array or invalid size.\n");
        return;
    }

    int i;
    int *tempArray;
    int maxnumber = get_max(array, size);
    int exp = 1;

    tempArray = (int *)malloc(size * sizeof(int));

    for (i = 0; i < size; i++)
        tempArray[i] = 0;

    if (tempArray == NULL)
    {
        printf("Failed to allocate memory for the temporary array.\n");
        return;
    }

    while (maxnumber / exp > 0)
    {
        int bucket[20] = {0};

        for (i = 0; i < size; i++)
            bucket[(array[i] / exp) % 10 + 10]++;

        for (i = 1; i < 20; i++)
            bucket[i] += bucket[i - 1];

        for (i = size - 1; i >= 0; i--)
            tempArray[--bucket[(array[i] / exp) % 10 + 10]] = array[i];

        for (i = 0; i < size; i++)
            array[i] = tempArray[i];

        exp *= 10;
    }

    free(tempArray);
}

void merge(int *array, int left, int mid, int right)
{
    int left_index, right_index, merged_index;
    int left_size = mid - left + 1;
    int right_size = right - mid;
    int *Left;
    int *Right;

    Left = (int *)malloc(left_size * sizeof(int));
    Right = (int *)malloc(right_size * sizeof(int));

    for (left_index = 0; left_index < left_size; left_index++)
        Left[left_index] = array[left + left_index];
    for (right_index = 0; right_index < right_size; right_index++)
        Right[right_index] = array[mid + 1 + right_index];

    left_index = 0;
    right_index = 0;
    merged_index = left;
    while (left_index < left_size && right_index < right_size)
    {
        if (Left[left_index] <= Right[right_index])
        {
            array[merged_index] = Left[left_index];
            left_index++;
        }
        else
        {
            array[merged_index] = Right[right_index];
            right_index++;
        }
        merged_index++;
    }

    while (left_index < left_size)
    {
        array[merged_index] = Left[left_index];
        left_index++;
        merged_index++;
    }

    while (right_index < right_size)
    {
        array[merged_index] = Right[right_index];
        right_index++;
        merged_index++;
    }

    free(Left);
    free(Right);
}

void mergix(int *array, int left, int right)
{
    if (left < right)
    {
        int mid, n;
        int max_size_for_radix = 10;

        n = right - left + 1;

        if (n <= max_size_for_radix)
        {
            radixsort(array + left, n);
        }
        else
        {
            mid = left + (right - left) / 2;
            mergix(array, left, mid);
            mergix(array, mid + 1, right);
            merge(array, left, mid, right);
        }
    }
}

void mergesort(int *array, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergesort(array, left, mid);
        mergesort(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}
