#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/lz78.h"

#define CHUNK_SIZE 4096

Buffer read_file_to_buffer(char *filename)
{
    Buffer buffer = buffer_create();

    FILE *file = fopen(filename, "rb");

    char chunk[CHUNK_SIZE];
    size_t bytes;

    while ((bytes = fread(chunk, 1, sizeof(chunk), file)) > 0)
    {
        buffer_write_array(&buffer, chunk, bytes);
    }

    fclose(file);

    return buffer;
}

void write_buffer_to_file(Buffer *buffer, char *filename)
{
    FILE *file = fopen(filename, "wb");

    fwrite(buffer_data(buffer), 1, buffer_size(buffer), file);
    fclose(file);
}

void compress(char *decompressed_filename, char *compressed_filename)
{
    Buffer input_buffer = read_file_to_buffer(decompressed_filename);
    Buffer output_buffer = buffer_create();

    lz78_compress(&input_buffer, &output_buffer);

    write_buffer_to_file(&output_buffer, compressed_filename);

    buffer_free(&input_buffer);
    buffer_free(&output_buffer);
}

void decompress(char *compressed_filename, char *decompressed_filename)
{
    Buffer input_buffer = read_file_to_buffer(compressed_filename);
    Buffer output_buffer = buffer_create();

    lz78_decompress(&input_buffer, &output_buffer);

    write_buffer_to_file(&output_buffer, decompressed_filename);

    buffer_free(&input_buffer);
    buffer_free(&output_buffer);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Uso:\n");
        printf("    %s C <entrada.EMPRECSV> <saida.wd41>\n", argv[0]);
        printf("    %s D <arquivo.wd41> <saida.EMPRECSV>\n", argv[0]);

        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "C") == 0)
    {
        printf("Compactando arquivo \"%s\" para \"%s\"...\n", argv[2], argv[3]);
        compress(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "D") == 0)
    {
        printf("Descompactando arquivo \"%s\" para \"%s\"...\n", argv[2], argv[3]);
        decompress(argv[2], argv[3]);
    }
    else
    {
        printf("ERRO: a opção \"%s\" não é válida.\n", argv[1]);
        return EXIT_FAILURE;
    }
    
    return 0;
}