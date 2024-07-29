#ifndef EXTERNAL_SORT_H
#define EXTERNAL_SORT_H

typedef void (*SortFunction)(int *, long long);
typedef struct sort_options SortOptions;

struct sort_options
{
    SortFunction sort;
    char *input_filename;
    char *output_filename;
    char *tmp_directory;
    long long run_count;
    long long run_size;
    long long buffer_size;
};


void sort_files(SortOptions *options);

#endif