#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "common/mergix.h"

bool has_negative_values(int *array, long long size)
{
    long long i;
    for (i = 0; i < size; i++)
    {
        if (array[i] < 0)
        {
            return true;
        }
    }

    return false;
}

void radixsort(int *array, long long n)
{
    int *output = (int *)malloc(n * sizeof(int));
    int count[256] = {0};

    int shift;

    for (shift = 0; shift < 32; shift += 8)
    {
        int i;

        for (i = 0; i < 256; i++)
        {
            count[i] = 0;
        }

        for (i = 0; i < n; i++)
        {
            count[(array[i] >> shift) & 0xFF]++;
        }

        for (i = 1; i < 256; i++)
        {
            count[i] += count[i - 1];
        }

        for (i = n - 1; i >= 0; i--)
        {
            output[count[(array[i] >> shift) & 0xFF] - 1] = array[i];
            count[(array[i] >> shift) & 0xFF]--;
        }

        for (i = 0; i < n; i++)
        {
            array[i] = output[i];
        }
    }

    free(output);
}

void merge(int *array, long long left, long long mid, long long right)
{
    long long left_index, right_index, merged_index;
    long long left_size = mid - left + 1;
    long long right_size = right - mid;

    int *left_array = (int *)malloc(left_size * sizeof(int));
    int *right_array = (int *)malloc(right_size * sizeof(int));

    for (left_index = 0; left_index < left_size; left_index++)
    {
        left_array[left_index] = array[left + left_index];
    }

    for (right_index = 0; right_index < right_size; right_index++)
    {
        right_array[right_index] = array[mid + 1 + right_index];
    }

    left_index = 0;
    right_index = 0;
    merged_index = left;

    while (left_index < left_size && right_index < right_size)
    {
        if (left_array[left_index] <= right_array[right_index])
        {
            array[merged_index] = left_array[left_index];
            left_index++;
        }
        else
        {
            array[merged_index] = right_array[right_index];
            right_index++;
        }

        merged_index++;
    }

    while (left_index < left_size)
    {
        array[merged_index] = left_array[left_index];
        left_index++;
        merged_index++;
    }

    while (right_index < right_size)
    {
        array[merged_index] = right_array[right_index];
        right_index++;
        merged_index++;
    }

    free(left_array);
    free(right_array);
}

void mergesort_recursion(int *array, long long left, long long right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergesort_recursion(array, left, mid);
        mergesort_recursion(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}

void mergesort(int *array, long long size)
{
    mergesort_recursion(array, 0, size - 1);
}

void mergix_recursion(int *array, long long left, long long right, long long threshold)
{
    if (left < right)
    {
        long long mid, n;

        n = right - left + 1;

        if (n <= threshold)
        {
            radixsort(array + left, n);
        }
        else
        {
            mid = left + (right - left) / 2;
            mergix_recursion(array, left, mid, threshold);
            mergix_recursion(array, mid + 1, right, threshold);
            merge(array, left, mid, right);
        }
    }
}

void mergix(int *array, long long size, long long threshold)
{
    if (has_negative_values(array, size))
    {
        mergesort_recursion(array, 0, size - 1);
    }
    else
    {
        mergix_recursion(array, 0, size - 1, threshold);
    }
}
