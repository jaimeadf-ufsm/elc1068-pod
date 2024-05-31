#ifndef EXTERNAL_HEAP_H
#define EXTERNAL_HEAP_H

#include <stdbool.h>

#include "buffered_io.h"

typedef struct element Element;
typedef struct heap Heap;

struct element {
    int value;
    BufferedReader *reader;
};

struct heap
{
    Element *array;

    int size;
    int capacity;
};

Heap create_heap(int capacity);
void free_heap(Heap *heap);

Element *peek_heap(Heap *heap);

void push_heap(Heap *heap, Element *element);
void pop_heap(Heap *heap);
void replace_heap(Heap *heap, Element *element);

bool is_heap_empty(Heap *heap);

#endif