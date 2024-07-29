#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

typedef struct buffer Buffer;

struct buffer
{
    size_t capacity;
    size_t size;
    char *data;
};

Buffer buffer_create();
void buffer_free(Buffer *buffer);

char *buffer_data(Buffer *buffer);
size_t buffer_size(Buffer *buffer);

void buffer_write_array(Buffer *buffer, const char *data, size_t length);
void buffer_write_char(Buffer *buffer, char c);

char buffer_read_char(Buffer *buffer, size_t position);

void buffer_reverse(Buffer *buffer);
void buffer_clear(Buffer *buffer);


#endif