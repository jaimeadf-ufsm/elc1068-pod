#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "heap.h"

typedef struct element Element;
typedef void (*SortFunction)(int *, int);

struct element {
    int value;
    FILE *run;
};

bool compare_run_element(void *a, void *b)
{
    Element *element_a = (Element*)a;
    Element *element_b = (Element*)b;

    return element_a->value > element_b->value;
}

bool take_run_element_out(FILE *run, Element *out)
{
    out->run = run;
    return fscanf(run, "%d\n", &out->value) == 1;
}

int create_runs(FILE **runs, char *input_filename, int run_count, int run_size, SortFunction sort)
{
    fprintf(stderr, "DEBUG: creating runs...\n");

    FILE *input = fopen(input_filename, "r");

    int *array = (int*)malloc(run_size * sizeof(int));
    int element_count = 0;

    for (int i = 0; i < run_count; i++)
    {
        int n = 0;

        fprintf(stderr, "DEBUG: sorting run %d...\n", i);

        runs[i] = tmpfile();

        while (n < run_size && fscanf(input, "%d", &array[n]) == 1)
        {
            n++;
        }

        sort(array, n);

        for (int j = 0; j < n; j++)
        {
            fprintf(runs[i], "%d\n", array[j]);
        }

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

void merge_runs(FILE **runs, char *output_filename, int run_count, int element_count)
{
    fprintf(stderr, "DEBUG: merging runs...\n");

    FILE *output = fopen(output_filename, "w");
    Heap heap = create_heap(run_count, sizeof(Element), compare_run_element);

    fprintf(stderr, "DEBUG: creating initial nodes in heap...\n");
    for (int i = 0; i < run_count; i++)
    {
        Element element;

        rewind(runs[i]);

        if (take_run_element_out(runs[i], &element))
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

    while (!is_heap_empty(&heap))
    {
        Element root = *(Element *)peek_heap(&heap);

        fprintf(output, "%d\n", root.value);

        if (take_run_element_out(root.run, &root))
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

    free_heap(&heap);
    fclose(output);
}

void clear_runs(FILE **runs, int run_count)
{
    fprintf(stderr, "DEBUG: clearing runs...\n");

    for (int i = 0; i < run_count; i++)
    {
        fclose(runs[i]);
    }
}

void sort_files(char *input_filename, char *output_filename, int run_count, int run_size, SortFunction sort)
{
    FILE *output = fopen(output_filename, "w");
    FILE *runs[run_count];

    int element_count = create_runs(runs, input_filename, run_count, run_size, sort);

    merge_runs(runs, output_filename, run_count, element_count);
    clear_runs(runs, run_count);
}