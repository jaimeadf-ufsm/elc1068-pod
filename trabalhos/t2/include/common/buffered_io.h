#ifndef BUFFERED_IO_H
#define BUFFERED_IO_H

#include <stdio.h>
#include <stdbool.h>

#define DEFAULT_IO_BUFFER_CAPACITY (4096)

typedef struct buffered_reader BufferedReader;
typedef struct buffered_writer BufferedWriter;

struct buffered_reader
{
    FILE *file;
    char *buffer;
    size_t index;
    size_t size;
    size_t capacity;
};

struct buffered_writer
{
    FILE *file;
    char *buffer;
    size_t size;
    size_t capacity;
};

BufferedReader reader_open(const char *filename, size_t capacity);
void reader_close(BufferedReader *reader);

char reader_read_char(BufferedReader *reader);

bool reader_is_end_of_file(BufferedReader *reader);

BufferedWriter writer_open(const char *filename, size_t capacity);
void writer_flush(BufferedWriter *writer);
void writer_close(BufferedWriter *writer);

void writer_write_char(BufferedWriter *writer, char c);

#endif
