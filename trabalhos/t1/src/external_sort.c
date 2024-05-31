#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "common/external_heap.h"

#include "common/external_sort.h"
#include "common/high_precision_timer.h"

typedef struct run Run;

char *TMP_DIR = "tmp";

struct run
{
    char filename[256];
};

bool take_run_element_out(BufferedReader *reader, Element *out)
{
    out->reader = reader;

    if (has_reader_ended(reader))
    {
        return false;
    }

    out->value = read_number(reader);

    return true;
}

int create_runs(Run *runs, char *input_filename, int run_count, int run_size, SortFunction sort, int buffer_size)
{
    fprintf(stderr, "DEBUG: creating runs...\n");

    BufferedReader input_reader = open_reader(input_filename, buffer_size);

    int *array = (int *)malloc(run_size * sizeof(int));
    int element_count = 0;

    for (int i = 0; i < run_count; i++)
    {
        int n = 0;

        sprintf(runs[i].filename, "%s/%06d.tmp", TMP_DIR, i);
        BufferedWriter run_writer = open_writer(runs[i].filename, buffer_size);

        while (n < run_size && !has_reader_ended(&input_reader))
        {
            array[n++] = read_number(&input_reader);
        }

        Timer timer = start_timer();
        sort(array, n);
        stop_timer(&timer);

        fprintf(stderr, "DEBUG: %d elements of run %d sorted in %.2fs.\n", n, i, get_timer_nanoseconds(&timer) / 1e9);

        for (int j = 0; j < n; j++)
        {
            write_number(&run_writer, array[j]);
        }

        element_count += n;
        close_writer(&run_writer);
    }

    close_reader(&input_reader);
    free(array);

    fprintf(stderr, "DEBUG: %d elements read.\n", element_count);

    return element_count;
}

void merge_runs(Run *runs, char *output_filename, int run_count, int element_count, int buffer_size)
{
    fprintf(stderr, "DEBUG: merging runs...\n");

    BufferedWriter output_writer = open_writer(output_filename, buffer_size);
    BufferedReader *run_readers = (BufferedReader *)malloc(run_count * sizeof(BufferedReader));

    if (run_readers == NULL)
    {
        fprintf(stderr, "ERROR: unable to allocate memory for run readers\n");
        exit(EXIT_FAILURE);
    }

    Heap heap = create_heap(run_count);

    fprintf(stderr, "DEBUG: creating initial nodes in heap...\n");
    for (int i = 0; i < run_count; i++)
    {
        Element element;

        run_readers[i] = open_reader(runs[i].filename, buffer_size);

        if (take_run_element_out(&run_readers[i], &element))
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

        write_number(&output_writer, root.value);

        if (take_run_element_out(root.reader, &root))
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

    close_writer(&output_writer);

    for (int i = 0; i < run_count; i++)
    {
        close_reader(&run_readers[i]);
    }

    free_heap(&heap);
    free(run_readers);
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

    free(runs);
}