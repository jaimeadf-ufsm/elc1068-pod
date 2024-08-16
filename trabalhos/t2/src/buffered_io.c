#include <stdlib.h>
#include <ctype.h>

#include "common/buffered_io.h"

void reader_fill_buffer(BufferedReader *reader)
{
    reader->size = fread(reader->buffer, sizeof(char), reader->capacity, reader->file);
    reader->index = 0;
}

BufferedReader reader_open(const char *filename, size_t capacity)
{
    BufferedReader reader;
    reader.file = fopen(filename, "r");
    reader.buffer = (char *)malloc(capacity * sizeof(char));
    reader.index = 0;
    reader.size = 0;
    reader.capacity = capacity;

    if (reader.file == NULL)
    {
        fprintf(stderr, "ERRO: não foi possível abrir o arquivo \"%s\" para leitura.\n", filename);
        exit(EXIT_FAILURE);
    }

    reader_fill_buffer(&reader);

    return reader;
}

void reader_close(BufferedReader *reader)
{
    fclose(reader->file);
    free(reader->buffer);
}

char reader_read_char(BufferedReader *reader)
{
    if (reader_is_end_of_file(reader))
    {
        fprintf(stderr, "ERROR: fim do arquivo inesperado ao tentar ler caractere.\n");
        exit(EXIT_FAILURE);
    }

    char c = reader->buffer[reader->index++];

    if (reader->index == reader->size)
    {
        reader_fill_buffer(reader);
    }

    return c;
}

bool reader_is_end_of_file(BufferedReader *reader)
{
    return reader->size == 0;
}

BufferedWriter writer_open(const char *filename, size_t capacity)
{
    BufferedWriter writer;
    writer.file = fopen(filename, "w");
    writer.buffer = (char *)malloc(capacity * sizeof(char));
    writer.size = 0;
    writer.capacity = capacity;

    if (writer.file == NULL)
    {
        fprintf(stderr, "ERRO: não foi possível abrir o arquivo \"%s\" para escrita.\n", filename);
        exit(EXIT_FAILURE);
    }

    return writer;
}

void writer_close(BufferedWriter *writer)
{
    writer_flush(writer);
    fclose(writer->file);
    free(writer->buffer);
}

void writer_write_char(BufferedWriter *writer, char c)
{
    if (writer->size == writer->capacity)
    {
        writer_flush(writer);
    }

    writer->buffer[writer->size++] = c;
}

void writer_flush(BufferedWriter *writer)
{
    fwrite(writer->buffer, sizeof(char), writer->size, writer->file);
    writer->size = 0;
}
