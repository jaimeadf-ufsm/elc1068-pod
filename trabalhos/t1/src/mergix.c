#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "common/mergix.h"

#define MERGIX_THRESHOLD 97

int get_max(int *array, int size)
{
    int mx = array[0];
    int i;
    for (i = 1; i < size; i++)
        if (array[i] > mx)
            mx = array[i];
    return mx;
}

void radixsort(int *array, int n)
{
    int *output = (int *)malloc(n * sizeof(int));
    int count[256] = {0};
    int exp, i;

    for (exp = 0; exp < 32; exp += 8)
    {
        for (i = 0; i < 256; i++)
            count[i] = 0;

        for (i = 0; i < n; i++)
            count[(array[i] >> exp) & 0xFF]++;

        for (i = 1; i < 256; i++)
            count[i] += count[i - 1];

        for (i = n - 1; i >= 0; i--)
        {
            output[count[(array[i] >> exp) & 0xFF] - 1] = array[i];
            count[(array[i] >> exp) & 0xFF]--;
        }

        for (i = 0; i < n; i++)
            array[i] = output[i];
    }

    free(output);
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

bool has_negative_values(int *array, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (array[i] < 0)
        {
            return true;
        }
    }
    return false;
}

void mergix_call(int *array, int size)
{
    if (has_negative_values(array, size))
    {
        mergesort(array, 0, size - 1);
    }
    else
    {
        mergix(array, 0, size - 1);
    }
}

void mergix(int *array, int left, int right)
{
    if (left < right)
    {
        int mid, n;

        n = right - left + 1;

        if (n <= MERGIX_THRESHOLD)
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

void mergesort_call(int *array, int size)
{
    mergesort(array, 0, size - 1);
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
