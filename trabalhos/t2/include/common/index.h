#ifndef INDEX_H

#include <stddef.h>

#include "company.h"

typedef char *(*KeyFunction)(const Company *company);

typedef struct index Index;
typedef struct index_entry IndexEntry;

struct index_entry
{
    char *key;
    Company *company;
};

struct index
{
    size_t size;
    struct index_entry *entries;
};

Index index_create(Company *head, KeyFunction key_function);
void index_free(Index *index);

Company *index_search(Index *index, const char *key);

#endif