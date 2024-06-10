#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void radixsort(int *array, int size)
{
    int i;
    int *temp_array;
    int biggest_value;
    int exp;
    int bucket[19];

    biggest_value = abs(array[0]);
    for (i = 1; i < size; i++)
    {
        if (abs(array[i]) > biggest_value)
            biggest_value = abs(array[i]);
    }

    temp_array = (int *)malloc(size * sizeof(int));

    exp = 1;
    while (biggest_value / exp > 0)
    {
        for (i = 0; i < 19; i++)
            bucket[i] = 0;

        for (i = 0; i < size; i++)
        {
            int bucket_index;
            if (array[i] < 0)
                bucket_index = 9 - (abs(array[i]) / exp) % 10;
            else
                bucket_index = 10 + (array[i] / exp) % 10;
            bucket[bucket_index]++;
        }

        for (i = 1; i < 19; i++)
            bucket[i] += bucket[i - 1];

        for (i = size - 1; i >= 0; i--)
        {
            int bucket_index;
            if (array[i] < 0)
                bucket_index = 9 - (abs(array[i]) / exp) % 10;
            else
                bucket_index = 10 + (array[i] / exp) % 10;
            temp_array[--bucket[bucket_index]] = array[i];
        }

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
