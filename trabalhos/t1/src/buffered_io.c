#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "common/buffered_io.h"

void ensure_buffer_is_not_empty(BufferedReader *reader)
{
    if (reader->index < reader->size)
    {
        return;
    }

    reader->size = fread(reader->buffer, sizeof(char), reader->capacity, reader->file);
    reader->index = 0;
}

BufferedReader open_reader(char *filename, int capacity)
{
    BufferedReader reader;
    reader.file = fopen(filename, "r");
    reader.buffer = (char *)malloc(capacity * sizeof(char));
    reader.index = 0;
    reader.size = 0;
    reader.capacity = capacity;

    if (reader.file == NULL)
    {
        fprintf(stderr, "ERROR: could not open file \"%s\".\n", filename);
        exit(EXIT_FAILURE);
    }

    return reader;
}

void close_reader(BufferedReader *reader)
{
    fclose(reader->file);
    free(reader->buffer);
}

int read_number(BufferedReader *reader)
{
    int number = 0;

    while (true)
    {

        char c = read_char(reader);

        if (!isdigit(c))
        {
            if (c == '\n')
            {
                break;
            }

            fprintf(stderr, "ERROR: unexpected end of number. Expected: '\\n', Received: %c.\n", c);
            exit(EXIT_FAILURE);
        }

        number = number * 10 + (c - '0');
    }

    return number;
}

char read_char(BufferedReader *reader)
{
    if (has_reader_ended(reader))
    {
        fprintf(stderr, "ERROR: unexpected end of file when trying to read character.\n");
        exit(EXIT_FAILURE);
    }

    return reader->buffer[reader->index++];
}

bool has_reader_ended(BufferedReader *reader)
{
    ensure_buffer_is_not_empty(reader);
    return reader->size == 0;
}

BufferedWriter open_writer(char *filename, int capacity)
{
    BufferedWriter writer;
    writer.file = fopen(filename, "w");
    writer.buffer = (char *)malloc(capacity * sizeof(char));
    writer.size = 0;
    writer.capacity = capacity;

    if (writer.file == NULL)
    {
        fprintf(stderr, "ERROR: could not open file \"%s\".\n", filename);
        exit(EXIT_FAILURE);
    }

    return writer;
}

void close_writer(BufferedWriter *writer)
{
    flush_writer(writer);
    fclose(writer->file);
    free(writer->buffer);
}

int write_number(BufferedWriter *writer, int number)
{
    int characters = 1;

    if (number < 0)
    {
        write_char(writer, '-');
        number = -number;
        characters++;
    }

    for (int i = 1000000000; i > 0; i /= 10)
    {
        if (number >= i)
        {
            write_char(writer, '0' + (number / i) % 10);
            characters++;
        }
    }

    write_char(writer, '\n');

    return characters;
}

void write_char(BufferedWriter *writer, char c)
{
    if (writer->size == writer->capacity)
    {
        flush_writer(writer);
    }

    writer->buffer[writer->size++] = c;
}

void flush_writer(BufferedWriter *writer)
{
    fwrite(writer->buffer, sizeof(char), writer->size, writer->file);
    writer->size = 0;
}