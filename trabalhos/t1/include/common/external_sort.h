#ifndef EXTERNAL_SORT_H
#define EXTERNAL_SORT_H

typedef void (*SortFunction)(int *, int);

void sort_files(char *input_filename, char *output_filename, int run_count, int run_size, SortFunction sort);

#endif