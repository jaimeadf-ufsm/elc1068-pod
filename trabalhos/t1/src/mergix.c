#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void radixsort(int *array, int size)
{
    int i;
    int *temp_array;
    int biggest_value = array[0];
    int exp = 1;

    temp_array = (int *)malloc(size * sizeof(int));

    for (i = 0; i < size; i++)
    {
        if (array[i] > biggest_value)
            biggest_value = array[i];
    }

    while (biggest_value / exp > 0)
    {
        int bucket[10] = {0};
        for (i = 0; i < size; i++)
            bucket[(array[i] / exp) % 10]++;
        for (i = 1; i < 10; i++)
            bucket[i] += bucket[i - 1];
        for (i = size - 1; i >= 0; i--)
            temp_array[--bucket[(array[i] / exp) % 10]] = array[i];
        for (i = 0; i < size; i++)
            array[i] = temp_array[i];
        exp *= 10;
    }
    free(temp_array);
}

void merge(int *array, int left, int mid, int right)
{
    int left_index, right_index, merged_index;
    int left_size = mid - left + 1;
    int right_size = right - mid;

    int Left[left_size], Right[right_size];

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
}

void mergix(int *array, int left, int right)
{
    if (left < right)
    {
        int max_size_for_radix = 10;

        int n = right - left + 1;

        if (n <= max_size_for_radix)
        {
            radixsort(array + left, n);
        }
        else
        {
            int mid = left + (right - left) / 2;
            mergix(array, left, mid);
            mergix(array, mid + 1, right);
            merge(array, left, mid, right);
        }
    }
}