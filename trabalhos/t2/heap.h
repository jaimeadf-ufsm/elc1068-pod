#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef struct heap Heap;
typedef bool (*HeapComparisonFunction)(void *, void *);

struct heap
{
    void *array;

    int element_count;
    int element_capacity;

    int element_size;

    HeapComparisonFunction compare;
};

void swap(void *a, void *b, int size)
{
    char temp[size];
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

Heap create_heap(int element_capacity, int element_size, HeapComparisonFunction compare)
{
    Heap heap;
    heap.array = malloc(element_capacity * element_size);
    heap.element_count = 0;
    heap.element_capacity = element_capacity;
    heap.element_size = element_size;
    heap.compare = compare;

    return heap;
}

void free_heap(Heap *heap)
{
    free(heap->array);
}

int get_heap_left(Heap *heap, int i)
{
    return 2 * i + 1 * heap->element_size;
} 

int get_heap_right(Heap *heap, int i)
{
    return 2 * i + 2 * heap->element_size;
}

int get_heap_parent(Heap *heap, int i)
{
    return  heap->element_size * ((i /  heap->element_size - 1) / 2);
}

void heapify_up(Heap *heap, int i)
{
    int parent = get_heap_parent(heap, i);

    while (i != 0 && heap->compare(heap->array + parent, heap->array + i))
    {
        swap(heap->array + i, heap->array + parent, heap->element_size);
        i = parent;
        parent = get_heap_parent(heap, i);
    }
}

void heapify_down(Heap *heap)
{
    int i = 0;

    while (true)
    {
        int l = get_heap_left(heap, i);
        int r = get_heap_right(heap, i);

        int largest = i;

        if (l < heap->element_count * heap->element_size && heap->compare(heap->array + largest, heap->array + l))
        {
            largest = l;
        }

        if (r < heap->element_count * heap->element_size && heap->compare(heap->array + largest, heap->array + r))
        {
            largest = r;
        }

        if (i == largest) {
            break;
        }

        swap(heap->array + i, heap->array + largest, heap->element_size);
        i = largest;
    }
}

void *peek_heap(Heap *heap)
{
    return heap->array;
}

void push_heap(Heap *heap, void *element)
{
    memcpy(heap->array + heap->element_count * heap->element_size, element, heap->element_size);
    heapify_up(heap, heap->element_count * heap->element_size);

    heap->element_count++;
}

void pop_heap(Heap *heap)
{
    heap->element_count--;
    swap(heap->array, heap->array + heap->element_count * heap->element_size, heap->element_size);
    heapify_down(heap);
}

void replace_heap(Heap *heap, void *element)
{
    memcpy(heap->array, element, heap->element_size);
    heapify_down(heap);
}

bool is_heap_empty(Heap *heap)
{
    return heap->element_count == 0;
}