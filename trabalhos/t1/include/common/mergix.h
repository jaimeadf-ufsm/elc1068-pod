#ifndef MERGIX_H
#define MERGIX_H

void radixsort(int *array, int size);
int get_max(int *array, int size);
void merge(int *array, int left, int mid, int right);
void mergix(int *array, int left, int right);
void mergix_call(int *array, int size);
void mergesort(int *array, int left, int right);
bool has_negative_values(int *array, int size);

#endif