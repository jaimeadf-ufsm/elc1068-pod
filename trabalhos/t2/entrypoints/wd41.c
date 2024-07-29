#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/company.h"

void compress(char *decompressed_filename, char *compressed_filename)
{
    FILE *output = fopen(compressed_filename, "wb");

    Company *head = company_read(decompressed_filename);
    Company *company = head;

    while (company != NULL)
    {
        unsigned int cnpj;
        unsigned int nature;
        unsigned int qualification;
        unsigned int capital_integer;
        unsigned int capital_fraction;
        unsigned int size;

        sscanf(company->cnpj, "%u", &cnpj);
        sscanf(company->nature, "%u", &nature);
        sscanf(company->responsible_qualification, "%u", &qualification);
        sscanf(company->capital, "%u,%u", &capital_integer, &capital_fraction);
        sscanf(company->size, "%u", &size);

        fwrite(&cnpj, 4, 1, output);
        fwrite(company->name, 1, strlen(company->name) + 1, output);
        fwrite(&nature, 2, 1, output);
        fwrite(&qualification, 2, 1, output);
        fwrite(&capital_integer, 4, 1, output);
        fwrite(&capital_fraction, 1, 1, output);
        fwrite(&size, 1, 1, output);
        fwrite(company->responsible_federal_entity, 1, strlen(company->responsible_federal_entity) + 1, output);

        company = company->next;
    }

    fclose(output);
}

void decompress(char *compressed_filename, char *decompressed_filename)
{

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