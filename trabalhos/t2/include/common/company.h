#ifndef COMPANY_H
#define COMPANY_H

#include <stddef.h>

/* No arquivo empresas0, foram encontrados os seguintes campos:
 * CNPJ BÁSICO
 * RAZÃO SOCIAL / NOME EMPRESARIAL
 * NATUREZA JURÍDICA
 * QUALIFICAÇÃO DO RESPONSÁVEL
 * CAPITAL SOCIAL DA EMPRESA
 * PORTE DA EMPRESA
 * ENTE FEDERATIVO RESPONSÁVEL
 */

typedef struct company Company;

struct company
{
    char *cnpj;
    char *name;
    char *nature;
    char *responsible_qualification;
    char *capital;
    char *size;
    char *responsible_federal_entity;

    Company *next;
};

Company *company_read(const char *filename);
void company_free(Company *head);

size_t company_count(const Company *head);
void company_print(const Company *company);

#endif