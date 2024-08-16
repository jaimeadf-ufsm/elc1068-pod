#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/lz78.h"

#define BUFFER_SIZE (4096)

void compress(char *decompressed_filename, char *compressed_filename)
{
    BufferedReader input = reader_open(decompressed_filename, BUFFER_SIZE);
    BufferedWriter output = writer_open(compressed_filename, BUFFER_SIZE);

    lz78_compress(&input, &output);

    reader_close(&input);
    writer_close(&output);
}

void decompress(char *compressed_filename, char *decompressed_filename)
{
    BufferedReader input = reader_open(compressed_filename, BUFFER_SIZE);
    BufferedWriter output = writer_open(decompressed_filename, BUFFER_SIZE);

    lz78_decompress(&input, &output);

    reader_close(&input);
    writer_close(&output);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Uso:\n");
        printf("    %s C <entrada.EMPRECSV> <saida.wd41>\n", argv[0]);
        printf("    %s D <entrada.wd41> <saida.EMPRECSV>\n", argv[0]);

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
    
    return EXIT_SUCCESS;
}