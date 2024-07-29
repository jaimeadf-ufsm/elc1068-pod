#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "common/company.h"
#include "common/index.h"

char *make_cnpj_key(const Company *company)
{
    return strdup(company->cnpj);
}

char *make_name_key(const Company *company)
{
    return strdup(company->name);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s <empresas0.EMPRECSV>\n", argv[0]);
        return EXIT_FAILURE;
    }

    printf("Lendo empresas do arquivo \"%s\"...\n", argv[1]);
    Company *companies = company_read(argv[1]);

    printf("Criando índice por CNPJ..\n");
    Index index_cnpj = index_create(companies, make_cnpj_key);

    printf("Criando índice por nome...\n");
    Index index_name = index_create(companies, make_name_key);

    printf("\n");
    printf("Digite cpnj \"<cpnj>\" ou nome \"<razão social>\" para buscar uma empresa.\n");
    printf("Digite exit para sair.\n");
    printf("\n");
    printf("OBS: A chave a ser buscada deve estar entre aspas duplas.\n");
    printf("\n");
    printf("Exemplos:\n");
    printf("\n");
    printf("    $ cnpj \"43241060\"\n");
    printf("    $ nome \"MC DONALDS COMERCIO E PADARIA LTDA\"\n");
    printf("\n");
    printf("\n");

    char *line = NULL;
    size_t line_capacity = 0;

    while (true)
    {
        printf("$ ");
        getline(&line, &line_capacity, stdin);

        line[strlen(line) - 1] = '\0';

        char *command = line;
        char *argument = strchr(line, ' ');

        if (argument != NULL)
        {
            *argument = '\0';
            argument++;
        }

        if (strcmp(command, "exit") == 0)
        {
            break;
        }
        else if (strcmp(command, "cnpj") == 0)
        {
            Company *company = index_search(&index_cnpj, argument);

            if (company == NULL)
            {
                printf("Nenhuma empresa com o CNPJ %s foi encontrada.\n", argument);
            }
            else
            {
                company_print(company);
            }
        }
        else if (strcmp(command, "nome") == 0)
        {
            Company *company = index_search(&index_name, argument);

            if (company == NULL)
            {
                printf("Nenhuma empresa com o nome %s foi encontrada.\n", argument);
            }
            else
            {
                company_print(company);
            }
        }
        else
        {
            printf("O comando \"%s\" não é válido.\n", command);
        }

        printf("\n");
    }

    printf("\n");
    printf("Liberando memória...\n");

    free(line);

    index_free(&index_cnpj);
    index_free(&index_name);
    company_free(companies);

    return 0;
}