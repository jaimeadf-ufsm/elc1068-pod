#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/zip.h"

#define ENTRY_FILENAME "data.EMPRECSV"

void compress(char *decompressed_filename, char *compressed_filename)
{
    struct zip_t *zip = zip_open(compressed_filename, ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');

    zip_entry_open(zip, ENTRY_FILENAME);
    zip_entry_fwrite(zip, decompressed_filename);
    zip_entry_close(zip);

    zip_close(zip);
}

void decompress(char *compressed_filename, char *decompressed_filename)
{
    struct zip_t *zip = zip_open(compressed_filename, 0, 'r');

    zip_entry_open(zip, ENTRY_FILENAME);
    zip_entry_fread(zip, decompressed_filename);
    zip_entry_close(zip);

    zip_close(zip);
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
        printf("ERRO: a opção \"%s\" inválida.\n", argv[1]);
        return EXIT_FAILURE;
    }
    

    return 0;
}