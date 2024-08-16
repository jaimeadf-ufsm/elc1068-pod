#ifndef BUFFERED_IO_H
#define BUFFERED_IO_H

#include <stdio.h>
#include <stdbool.h>

typedef struct buffered_reader BufferedReader;
typedef struct buffered_writer BufferedWriter;

struct buffered_reader
{
    FILE *file;
    char *buffer;
    long long index;
    long long size;
    long long capacity;
};

struct buffered_writer
{
    FILE *file;
    char *buffer;
    long long size;
    long long capacity;
};

BufferedReader reader_open(char *filename, long long capacity);
void reader_close(BufferedReader *reader);

int read_number(BufferedReader *reader);
char reader_read_char(BufferedReader *reader);

bool reader_is_empty(BufferedReader *reader);

BufferedWriter writer_open(char *filename, long long capacity);
void writer_flush(BufferedWriter *writer);
void writer_close(BufferedWriter *writer);

void writer_write_char(BufferedWriter *writer, char c);

#endif
