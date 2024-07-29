#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/buffer.h"

#define BUFFER_INITIAL_CAPACITY 4 * 1024

Buffer buffer_create()
{
    Buffer buffer;
    buffer.capacity = BUFFER_INITIAL_CAPACITY;
    buffer.size = 0;
    buffer.data = (char *)malloc(buffer.capacity);

    return buffer;
}

void buffer_free(Buffer *buffer)
{
    free(buffer->data);
}

char *buffer_data(Buffer *buffer)
{
    return buffer->data;
}

size_t buffer_size(Buffer *buffer)
{
    return buffer->size;
}

void buffer_ensure_capacity(Buffer *buffer, size_t size)
{
    while (buffer->capacity < size)
    {
        buffer->capacity *= 2;
    }

    buffer->data = (char *)realloc(buffer->data, buffer->capacity);

    if (buffer->data == NULL)
    {
        fprintf(stderr, "ERRO: não foi possível realocar memória para o buffer.\n");
        exit(EXIT_FAILURE);
    }
}

void buffer_write_array(Buffer *buffer, const char *data, size_t length)
{
    buffer_ensure_capacity(buffer, buffer->size + length);
    memcpy(buffer->data + buffer->size, data, length);
    buffer->size += length;
}

void buffer_write_char(Buffer *buffer, char c)
{
    buffer_ensure_capacity(buffer, buffer->size + 1);
    buffer->data[buffer->size++] = c;
}

char buffer_read_char(Buffer *buffer, size_t position)
{
    return buffer->data[position];
}

void buffer_reverse(Buffer *buffer)
{
    size_t i;

    for (i = 0; i < buffer->size / 2; i++)
    {
        char temp = buffer->data[i];

        buffer->data[i] = buffer->data[buffer->size - i - 1];
        buffer->data[buffer->size - i - 1] = temp;
    }
}

void buffer_clear(Buffer *buffer)
{
    buffer->size = 0;
}