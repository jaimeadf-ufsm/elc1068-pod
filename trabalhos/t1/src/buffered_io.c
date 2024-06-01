#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "common/buffered_io.h"

void fill_reader_buffer(BufferedReader *reader)
{
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

    fill_reader_buffer(&reader);

    return reader;
}

void close_reader(BufferedReader *reader)
{
    fclose(reader->file);
    free(reader->buffer);
}

int read_number(BufferedReader *reader)
{
    int sign = 1;
    int number = 0;

    char c = read_char(reader);

    if (c == '-')
    {
        sign = -1;
        c = read_char(reader);
    }

    do
    {
        if (!isdigit(c))
        {
            fprintf(stderr, "ERROR: invalid character '%c' when trying to read number.\n", c);
            exit(EXIT_FAILURE);
        }

        number = number * 10 + (c - '0');
        c = read_char(reader);
    } while (c != '\n');

    return sign * number;
}

char read_char(BufferedReader *reader)
{
    if (has_reader_ended(reader))
    {
        fprintf(stderr, "ERROR: unexpected end of file when trying to read character.\n");
        exit(EXIT_FAILURE);
    }

    char c = reader->buffer[reader->index++];

    if (reader->index == reader->size)
    {
        fill_reader_buffer(reader);
    }

    return c;
}

bool has_reader_ended(BufferedReader *reader)
{
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

    if (number == 0)
    {
        write_char(writer, '0');
        characters++;
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
