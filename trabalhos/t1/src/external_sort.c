#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "common/external_heap.h"

#include "common/external_sort.h"
#include "common/high_precision_timer.h"
#include "common/buffered_io.h"

typedef struct run Run;

char *TMP_DIR = "tmp";

struct run
{
    char filename[256];
};

bool take_run_element_out(FILE *file, Element *out)
{
    out->file = file;
    return fread(&out->value, sizeof(int), 1, file) == 1;
}

int create_runs(Run *runs, char *input_filename, int run_count, int run_size, SortFunction sort, int buffer_size)
{
    fprintf(stderr, "DEBUG: creating runs...\n");

    BufferedReader buffer = open_reader(input_filename, buffer_size);

    int *array = (int *)malloc(run_size * sizeof(int));
    int element_count = 0;

    for (int i = 0; i < run_count; i++)
    {
        int n = 0;

        sprintf(runs[i].filename, "%s/%06d.tmp", TMP_DIR, i);
        FILE *run_file = fopen(runs[i].filename, "wb");

        while (n < run_size && (array[n] = read_number(&buffer)) != EOF && !has_reader_ended(&buffer))
        {
            n++;
        }
        Timer timer = start_timer();
        sort(array, n);
        stop_timer(&timer);

        fprintf(stderr, "DEBUG: %d elements of run %d sorted in %.2fs.\n", n, i, get_timer_nanoseconds(&timer) / 1e9);

        fwrite(array, sizeof(int), n, run_file);
        fclose(run_file);

        element_count += n;

        if (n < run_size)
        {
            break;
        }
    }

    close_reader(&buffer);
    free(array);

    fprintf(stderr, "DEBUG: %d elements read.\n", element_count);

    return element_count;
}

void merge_runs(Run *runs, char *output_filename, int run_count, int element_count, int buffer_size)
{
    fprintf(stderr, "DEBUG: merging runs...\n");

    BufferedWriter buffer = open_writer(output_filename, buffer_size);

    FILE **run_files = (FILE **)malloc(run_count * sizeof(FILE *));
    if (run_files == NULL)
    {
        fprintf(stderr, "ERROR: Unable to allocate memory for run files\n");
        exit(EXIT_FAILURE);
    }

    Heap heap = create_heap(run_count);

    fprintf(stderr, "DEBUG: creating initial nodes in heap...\n");
    for (int i = 0; i < run_count; i++)
    {
        Element element;

        run_files[i] = fopen(runs[i].filename, "r");
        if (run_files[i] == NULL)
        {
            fprintf(stderr, "ERROR: Unable to open run file '%s'\n", runs[i].filename);
            exit(EXIT_FAILURE);
        }

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

    Timer timer = start_timer();

    while (!is_heap_empty(&heap))
    {
        Element root = *peek_heap(&heap);

        write_number(&buffer, root.value);

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

    stop_timer(&timer);

    fprintf(stderr, "DEBUG: %d elements merged in %.2fs.\n", merged_count, get_timer_nanoseconds(&timer) / 1e9);

    flush_writer(&buffer);

    close_writer(&buffer);

    for (int i = 0; i < run_count; i++)
    {
        fclose(run_files[i]);
    }

    free_heap(&heap);
    free(run_files);
}

void clear_runs(Run *runs, int run_count)
{
    fprintf(stderr, "DEBUG: clearing runs...\n");

    for (int i = 0; i < run_count; i++)
    {
        remove(runs[i].filename);
    }
}

void sort_files(char *input_filename, char *output_filename, int run_count, int run_size, SortFunction sort, int buffer_size)
{

    Run *runs = (Run *)malloc(run_count * sizeof(Run));

    int element_count = create_runs(runs, input_filename, run_count, run_size, sort, buffer_size);

    merge_runs(runs, output_filename, run_count, element_count, buffer_size);
    clear_runs(runs, run_count);
}