#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "heap.h"

typedef struct element Element;
typedef struct run Run;

typedef void (*SortFunction)(int *, int);

char *TMP_DIR = "tmp";

struct run
{
    char filename[256];
};

struct element {
    int value;
    FILE *file;
};

bool compare_run_element(void *a, void *b)
{
    Element *element_a = (Element*)a;
    Element *element_b = (Element*)b;

    return element_a->value > element_b->value;
}

bool take_run_element_out(FILE *file, Element *out)
{
    out->file = file;
    return fread(&out->value, sizeof(int), 1, file) == 1;
}

int create_runs(Run *runs, char *input_filename, int run_count, int run_size, SortFunction sort)
{
    fprintf(stderr, "DEBUG: creating runs...\n");

    FILE *input_file = fopen(input_filename, "r");

    int *array = (int*)malloc(run_size * sizeof(int));
    int element_count = 0;

    for (int i = 0; i < run_count; i++)
    {
        int n = 0;

        sprintf(runs[i].filename, "%s/%06d.tmp", TMP_DIR, i);
        FILE *run_file = fopen(runs[i].filename, "wb");


        while (n < run_size && fscanf(input_file, "%d", &array[n]) == 1)
        {
            n++;
        }

        clock_t start = clock();

        sort(array, n);

        clock_t end = clock();
        float seconds = (float)(end - start) / CLOCKS_PER_SEC;

        fprintf(stderr, "DEBUG: %d elements of run %d sorted in %.2fs.\n", n, i, seconds);

        fwrite(array, sizeof(int), n, run_file);
        fclose(run_file);

        element_count += n;

        if (n < run_size)
        {
            break;
        }
    }

    free(array);

    fprintf(stderr, "DEBUG: %d elements read.\n", element_count);

    return element_count;
}

void merge_runs(Run *runs, char *output_filename, int run_count, int element_count)
{
    fprintf(stderr, "DEBUG: merging runs...\n");

    FILE *output_file = fopen(output_filename, "w");
    FILE *run_files[run_count];

    Heap heap = create_heap(run_count, sizeof(Element), compare_run_element);

    fprintf(stderr, "DEBUG: creating initial nodes in heap...\n");
    for (int i = 0; i < run_count; i++)
    {
        Element element;

        run_files[i] = fopen(runs[i].filename, "rb");

        if (take_run_element_out(run_files[i], &element))
        {
            push_heap(&heap, &element);
        }
    }

    int merged_count = 0;
    int merged_step = element_count / 20;

    if (merged_step == 0)
    {
        merged_step = 1;
    }

    clock_t start = clock();

    while (!is_heap_empty(&heap))
    {
        Element root = *(Element *)peek_heap(&heap);

        fprintf(output_file, "%d\n", root.value);

        if (take_run_element_out(root.file, &root))
        {
            replace_heap(&heap, &root);
        }
        else
        {
            pop_heap(&heap);
        }

        merged_count++;

        if (merged_count % merged_step == 0)
        {
            fprintf(stderr, "DEBUG: %d elements merged (%.1lf%%).\n", merged_count, merged_count * 100.0 / element_count);
        }
    }

    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;

    fprintf(stderr, "DEBUG: %d elements merged in %.2fs.\n", merged_count, seconds);

    free_heap(&heap);

    for (int i = 0; i < run_count; i++)
    {
        fclose(run_files[i]);
    }

    fclose(output_file);
}

void clear_runs(Run *runs, int run_count)
{
    fprintf(stderr, "DEBUG: clearing runs...\n");

    for (int i = 0; i < run_count; i++)
    {
        remove(runs[i].filename);
    }
}

void sort_files(char *input_filename, char *output_filename, int run_count, int run_size, SortFunction sort)
{
    Run runs[run_count];

    int element_count = create_runs(runs, input_filename, run_count, run_size, sort);

    merge_runs(runs, output_filename, run_count, element_count);
    clear_runs(runs, run_count);
}