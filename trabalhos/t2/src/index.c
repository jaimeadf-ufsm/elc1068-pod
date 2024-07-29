#include <stdlib.h>
#include <string.h>

#include "common/index.h"

int index_compare(const void *a, const void *b)
{
    IndexEntry *entry_a = (IndexEntry *)a;
    IndexEntry *entry_b = (IndexEntry *)b;

    return strcmp(entry_a->key, entry_b->key);
}

Index index_create(Company *head, KeyFunction key_function)
{
    Index index;
    
    index.size = company_count(head);
    index.entries = (IndexEntry *)malloc(index.size * sizeof(IndexEntry));

    size_t i = 0;
    Company *company = head;

    while (company != NULL)
    {
        index.entries[i].key = key_function(company);
        index.entries[i].company = company;

        i++;
        company = company->next;
    }

    qsort(index.entries, index.size, sizeof(IndexEntry), index_compare);
    
    return index;
}

void index_free(Index *index)
{
    size_t i;

    for (i = 0; i < index->size; i++)
    {
        free(index->entries[i].key);
    }

    free(index->entries);
}

Company *index_search(Index *index, const char *key)
{
    IndexEntry key_entry;
    key_entry.key = (char *)key;

    IndexEntry *entry = bsearch(&key_entry, index->entries, index->size, sizeof(IndexEntry), index_compare);

    if (entry == NULL)
    {
        return NULL;
    }

    return entry->company;
}