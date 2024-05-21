#include <stdio.h>
#include <stdlib.h>

#include "common/swap.h"

#include "common/external_heap.h"

#define HEAP_LEFT(i) 2 * i + 1
#define HEAP_RIGHT(i) 2 * i + 2
#define HEAP_PARENT(i) (i - 1) / 2

void heapify_up(Heap *heap, int i)
{
    int parent = HEAP_PARENT(i);

    while (i != 0 && heap->array[parent].value > heap->array[i].value)
    {
        SWAP(Element, heap->array[parent], heap->array[i]);
        i = parent;
        parent = HEAP_PARENT(i);
    }
}

void heapify_down(Heap *heap)
{
    int i = 0;

    while (true)
    {
        int l = HEAP_LEFT(i);
        int r = HEAP_RIGHT(i);

        int smallest = i;

        if (l < heap->size && heap->array[l].value < heap->array[smallest].value)
        {
            smallest = l;
        }

        if (r < heap->size && heap->array[r].value < heap->array[smallest].value)
        {
            smallest = r;
        }

        if (i == smallest) {
            break;
        }

        SWAP(Element, heap->array[i], heap->array[smallest]);
        i = smallest;
    }
}

Heap create_heap(int capacity)
{
    Heap heap;
    heap.array = (Element *)malloc(sizeof(Element) * capacity);
    heap.size = 0;
    heap.capacity = capacity;

    return heap;
}

void free_heap(Heap *heap)
{
    free(heap->array);
}

Element *peek_heap(Heap *heap)
{
    return heap->array;
}

void push_heap(Heap *heap, Element *element)
{
    heap->array[heap->size] = *element;
    heapify_up(heap, heap->size);
    heap->size++;
}

void pop_heap(Heap *heap)
{
    heap->size--;
    heap->array[0] = heap->array[heap->size];
    heapify_down(heap);
}

void replace_heap(Heap *heap, Element *element)
{
    heap->array[0] = *element;
    heapify_down(heap);
}

bool is_heap_empty(Heap *heap)
{
    return heap->size == 0;
}