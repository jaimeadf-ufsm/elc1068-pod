#ifndef BUFFERED_IO_H
#define BUFFERED_IO_H

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

BufferedReader open_reader(char *filename, long long capacity);
void close_reader(BufferedReader *reader);

int read_number(BufferedReader *reader);
char read_char(BufferedReader *reader);

bool has_reader_ended(BufferedReader *reader);

BufferedWriter open_writer(char *filename, long long capacity);
void flush_writer(BufferedWriter *writer);
void close_writer(BufferedWriter *writer);

void write_number(BufferedWriter *writer, int number);
void write_char(BufferedWriter *writer, char c);

#endif
