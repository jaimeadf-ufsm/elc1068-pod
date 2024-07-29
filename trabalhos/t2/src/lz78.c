#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "common/lz78.h"

#define MAXIMUM_DICT_SIZE (0xFFFFFF)

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

HT ht_create(size_t capacity)
{
    HT table;
    table.capacity = capacity;
    table.size = 1;
    table.entries = (CompressorEntry *)calloc(sizeof(CompressorEntry), capacity);

    return table;
}

void ht_free(HT *table)
{
    free(table->entries);
}

/* Jenkins hash function */
size_t ht_hash(HT *table, uint32_t parent_index, char child_value)
{
    size_t hash = 0;

    hash += parent_index & 0xFF;
    hash += hash << 10;
    hash ^= hash >> 6;

    hash += (parent_index >> 8) & 0xFF;
    hash += hash << 10;
    hash ^= hash >> 6;

    hash += (parent_index >> 16) & 0xFF;
    hash += hash << 10;
    hash ^= hash >> 6;

    hash += (parent_index >> 24) & 0xFF;
    hash += hash << 10;
    hash ^= hash >> 6;

    hash += child_value & 0xFF;
    hash += hash << 10;
    hash ^= hash >> 6;

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    
    return hash % table->capacity;
}

void ht_insert(HT *table, uint32_t parent_index, uint32_t child_index, char child_value)
{
    size_t index = ht_hash(table, parent_index, child_value);

    while (table->entries[index].child_index)
    {
        index++;
    }

    table->entries[index].parent_index = parent_index;
    table->entries[index].child_index = child_index;
    table->entries[index].child_value = child_value;

    table->size++;
}

uint32_t ht_search(HT *table, uint32_t parent_index, char child_value)
{
    size_t index = ht_hash(table, parent_index, child_value);

    while (table->entries[index].child_index && (table->entries[index].parent_index != parent_index ||table->entries[index].child_value != child_value))
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

void lz78_compress(Buffer *input_buffer, Buffer *output_buffer)
{
    HT table = ht_create(2 * MAXIMUM_DICT_SIZE);

    size_t position = 0;

    while (position < input_buffer->size)
    {
        uint32_t parent_index = 0;
        uint32_t search_index = 0;

        size_t byte_shift;

        do
        {
            parent_index = search_index;
            search_index = ht_search(&table, parent_index, input_buffer->data[position++]);

            if (position % MB_BYTES == 0)
            {
                fprintf(stderr, "DEBUG: [LZ78] comprimido %zuMB.\n", position / MB_BYTES);
            }
        } while (search_index && position < input_buffer->size);

        char symbol = input_buffer->data[position - 1];

        for (byte_shift = 0; table.size >> byte_shift; byte_shift += 8)
        {
            buffer_write_char(output_buffer, (parent_index >> byte_shift) & 0xFF);
        }

        buffer_write_char(output_buffer, symbol);

        if (table.size == MAXIMUM_DICT_SIZE)
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

void lz78_decompress(Buffer *input_buffer, Buffer *output_buffer)
{
    DecompressorEntry *table = (DecompressorEntry *)calloc(sizeof(DecompressorEntry), MAXIMUM_DICT_SIZE);
    size_t table_size = 1;

    size_t position = 0;
    Buffer temporary_buffer = buffer_create();


    while (position < input_buffer->size)
    {
        uint32_t parent_index = 0;
        uint32_t search_index;

        size_t byte_shift;

        for (byte_shift = 0; table_size >> byte_shift; byte_shift += 8)
        {
            parent_index |= (input_buffer->data[position++] & 0xFF) << byte_shift;

            if (position % MB_BYTES == 0)
            {
                fprintf(stderr, "DEBUG: [LZ78] descomprimido %zuMB.\n", position / MB_BYTES);
            }
        }

        char symbol = input_buffer->data[position++];

        buffer_write_char(&temporary_buffer, symbol);

        for (search_index = parent_index; search_index; search_index = table[search_index].parent_index)
        {
            buffer_write_char(&temporary_buffer, table[search_index].child_value);
        }

        buffer_reverse(&temporary_buffer);
        buffer_write_array(output_buffer, buffer_data(&temporary_buffer), buffer_size(&temporary_buffer));

        buffer_clear(&temporary_buffer);

        if (table_size == MAXIMUM_DICT_SIZE)
        {
            fprintf(stderr, "DEBUG: [LZ78] dicionário limpo\n");
            table_size = 1;
        }
        else
        {
            table[table_size].parent_index = parent_index;
            table[table_size].child_value = symbol;

            table_size++;
        }
    }

    free(table);
}