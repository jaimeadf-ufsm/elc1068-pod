#ifndef MERGIX_H
#define MERGIX_H

void radixsort(int *array, int size);
void merge(int *array, int left, int mid, int right);
void mergix(int *array, int left, int right);
void sortNegatives(int *array, int size);
void radixSortNegatives(int *array, int size);

#endif