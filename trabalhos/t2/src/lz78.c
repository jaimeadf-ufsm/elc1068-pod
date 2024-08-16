#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "common/lz78.h"

#define DICT_SIZE (0xFFFFFF)
#define HT_CAPACITY (33554467)
#define TEMPORARY_CAPACITY (64)

#define KB_BYTES (1024)
#define MB_BYTES (1024 * KB_BYTES)

typedef struct compressor_entry CompressorEntry;
typedef struct decompressor_entry DecompressorEntry;
typedef struct ht HT;

struct compressor_entry
{
    uint32_t parent_index;
    uint32_t child_index;
    char child_value;
};

struct decompressor_entry
{
    uint32_t parent_index;
    char child_value;
};

struct ht
{
    size_t capacity;
    size_t size;
    CompressorEntry *entries;
};

void mark_progress(size_t position)
{
    if (position % MB_BYTES == 0)
    {
        fprintf(stderr, "DEBUG: [LZ78] %zuMB concluído.\n", position / MB_BYTES);
    }
}

HT ht_create(size_t capacity)
{
    HT table;
    table.capacity = capacity;
    table.size = 1;
    table.entries = (CompressorEntry *)calloc(capacity, sizeof(CompressorEntry));

    return table;
}

void ht_free(HT *table)
{
    free(table->entries);
}

size_t ht_hash(HT *table, uint32_t parent_index, char child_value)
{
    return (parent_index * 31 + (unsigned char)child_value) % table->capacity;
}

void ht_insert(HT *table, uint32_t parent_index, uint32_t child_index, char child_value)
{
    size_t index = ht_hash(table, parent_index, child_value);

    while (table->entries[index].child_index)
    {
        index = (index + 1) % table->capacity;
    }

    table->entries[index].parent_index = parent_index;
    table->entries[index].child_index = child_index;
    table->entries[index].child_value = child_value;

    table->size++;
}

uint32_t ht_search(HT *table, uint32_t parent_index, char child_value)
{
    size_t index = ht_hash(table, parent_index, child_value);

    while (table->entries[index].child_index && (
        table->entries[index].parent_index != parent_index ||
        table->entries[index].child_value != child_value)
    )
    {
        index = (index + 1) % table->capacity;
    }

    return table->entries[index].child_index;
}

void ht_reset(HT *table)
{
    table->size = 1;

    size_t i = 0;

    for (i = 0; i < table->capacity; i++)
    {
        table->entries[i].child_index = 0;
    }
}

void lz78_compress(BufferedReader *input, BufferedWriter *output)
{
    HT table = ht_create(HT_CAPACITY);

    size_t position = 0;

    while (!reader_is_end_of_file(input))
    {
        size_t byte_shift;

        uint32_t parent_index = 0;
        uint32_t search_index = 0;

        char symbol;

        do
        {
            symbol = reader_read_char(input);

            parent_index = search_index;
            search_index = ht_search(&table, parent_index, symbol);

            mark_progress(position++);
        } while (search_index && !reader_is_end_of_file(input));

        for (byte_shift = 0; table.size >> byte_shift; byte_shift += 8)
        {
            writer_write_char(output, (parent_index >> byte_shift) & 0xFF);
        }

        writer_write_char(output, symbol);

        if (table.size == DICT_SIZE)
        {
            fprintf(stderr, "DEBUG: [LZ78] dicionário limpo.\n");
            ht_reset(&table);
        }
        else
        {
            ht_insert(&table, parent_index, table.size, symbol);
        }
    }

    ht_free(&table);
}

void lz78_decompress(BufferedReader *input, BufferedWriter *output)
{
    size_t table_size = 1;
    DecompressorEntry *table_array = (DecompressorEntry *)calloc(DICT_SIZE, sizeof(DecompressorEntry));

    size_t temporary_size = 0;
    size_t temporary_capacity = TEMPORARY_CAPACITY;
    char *temporary_array = (char *)malloc(temporary_capacity * sizeof(char));

    size_t position = 0;

    while (!reader_is_end_of_file(input))
    {
        uint32_t parent_index = 0;
        uint32_t search_index = 0;

        size_t byte_shift;

        for (byte_shift = 0; table_size >> byte_shift; byte_shift += 8)
        {
            parent_index |= (reader_read_char(input) & 0xFF) << byte_shift;
            mark_progress(position++);
        }

        char symbol = reader_read_char(input);

        temporary_array[temporary_size++] = symbol;
        mark_progress(position++);

        for (search_index = parent_index; search_index; search_index = table_array[search_index].parent_index)
        {
            temporary_array[temporary_size++] = table_array[search_index].child_value;

            if (temporary_size == temporary_capacity)
            {
                temporary_capacity = 2 * temporary_capacity;
                temporary_array = (char *)realloc(temporary_array, sizeof(char) * temporary_capacity);
            }
        }

        while (temporary_size)
        {
            writer_write_char(output, temporary_array[--temporary_size]);
        }

        if (table_size == DICT_SIZE)
        {
            fprintf(stderr, "DEBUG: [LZ78] dicionário limpo\n");
            table_size = 1;
        }
        else
        {
            table_array[table_size].parent_index = parent_index;
            table_array[table_size].child_value = symbol;

            table_size++;
        }
    }

    free(table_array);
    free(temporary_array);
}