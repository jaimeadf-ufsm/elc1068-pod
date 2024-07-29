#include <stdlib.h>
#include <string.h>

#include "common/company.h"
#include "common/csv.h"

Company *company_read(const char *filename)
{
    CSV csv = csv_open(filename);
    
    Company *head = NULL;
    Company *tail = NULL;
    
    while (csv_next_record(&csv))
    {
        Company *company = (Company *)malloc(sizeof(Company));

        company->cnpj = strdup(csv_next_field(&csv));
        company->name = strdup(csv_next_field(&csv));
        company->nature = strdup(csv_next_field(&csv));
        company->responsible_qualification = strdup(csv_next_field(&csv));
        company->capital = strdup(csv_next_field(&csv));
        company->size = strdup(csv_next_field(&csv));
        company->responsible_federal_entity = strdup(csv_next_field(&csv));
        
        company->next = NULL;
        
        if (head == NULL)
        {
            head = company;
        }
        else
        {
            tail->next = company;
        }
        
        tail = company;
    }
    
    csv_close(&csv);
    
    return head;
}

void company_free(Company *head)
{
    while (head != NULL)
    {
        Company *next = head->next;
        
        free(head->cnpj);
        free(head->name);
        free(head->nature);
        free(head->responsible_qualification);
        free(head->capital);
        free(head->size);
        free(head->responsible_federal_entity);
        
        free(head);
        
        head = next;
    }
}

size_t company_count(const Company *head)
{
    size_t count = 0;
    const Company *company = head;
    
    while (company != NULL)
    {
        count++;
        company = company->next;
    }

    return count;
}

void company_print(const Company *company)
{
    printf("CNPJ: %s\n", company->cnpj);
    printf("RAZÃO SOCIAL: %s\n", company->name);
    printf("NATUREZA JURÍDICA: %s\n", company->nature);
    printf("QUALIFICAÇÃO DO RESPONSÁVEL: %s\n", company->responsible_qualification);
    printf("CAPITAL SOCIAL DA EMPRESA: %s\n", company->capital);
    printf("PORTE DA EMPRESA: %s\n", company->size);
    printf("ENTE FEDERATIVO RESPONSÁVEL: %s\n", company->responsible_federal_entity);
}