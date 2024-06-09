#ifndef EXTERNAL_SORT_H
#define EXTERNAL_SORT_H

typedef void (*SortFunction)(int *, int);
typedef struct sort_options SortOptions;

struct sort_options
{
    SortFunction sort;
    char *input_filename;
    char *output_filename;
    char *tmp_directory;
    int run_count;
    int run_size;
    int buffer_size;
};


void sort_files(SortOptions *options);

#endif