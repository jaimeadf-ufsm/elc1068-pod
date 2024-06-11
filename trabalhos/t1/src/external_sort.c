#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "common/external_heap.h"

#include "common/external_sort.h"
#include "common/high_precision_timer.h"

typedef struct run Run;

struct run
{
    char filename[256];
};

int read_run(BufferedReader *reader, int *array, int maximum_size)
{
    int n = 0;

    while (n < maximum_size && !has_reader_ended(reader))
    {
        array[n++] = read_number(reader);
    }

    return n;
}

void write_run(BufferedWriter *writer, int *array, int size)
{
    int i;

    for (i = 0; i < size; i++)
    {
        write_number(writer, array[i]);
    }
}

int create_runs(SortOptions *options, Run *runs)
{
    fprintf(stderr, "DEBUG: creating runs...\n");

    BufferedReader input_reader = open_reader(options->input_filename, options->buffer_size);

    int *array = (int *)malloc(options->run_size * sizeof(int));
    int element_count = 0;

    int i;

    for (i = 0; i < options->run_count; i++)
    {
        sprintf(runs[i].filename, "%s/%06d.tmp", options->tmp_directory, i);

        BufferedWriter run_writer = open_writer(runs[i].filename, options->buffer_size);

        Timer read_timer = start_timer();
        int n = read_run(&input_reader, array, options->run_size);
        stop_timer(&read_timer);

        Timer reading_timer = start_timer();
        options->sort(array, n);
        stop_timer(&reading_timer);

        Timer writing_timer = start_timer();
        write_run(&run_writer, array, n);
        close_writer(&run_writer);
        stop_timer(&writing_timer);

        element_count += n;

        double reading_seconds = get_timer_seconds(&read_timer);
        double sorting_seconds = get_timer_seconds(&reading_timer);
        double writing_seconds = get_timer_seconds(&writing_timer);

        fprintf(
            stderr,
            "DEBUG: created run %d with %d elements in %.2lfs (reading: %.2lfs, sorting: %.2lfs, writing: %.2lfs).\n",
            i,
            n,
            reading_seconds + sorting_seconds + writing_seconds,
            reading_seconds,
            sorting_seconds,
            writing_seconds
        );
    }

    close_reader(&input_reader);
    free(array);

    fprintf(stderr, "DEBUG: created %d runs with %d elements.\n", options->run_count, element_count);

    return element_count;
}

void merge_runs(SortOptions *options, Run *runs, int element_count)
{
    fprintf(stderr, "DEBUG: merging runs...\n");

    BufferedReader *run_readers = (BufferedReader *)malloc(options->run_count * sizeof(BufferedReader));
    BufferedWriter output_writer = open_writer(options->output_filename, options->buffer_size);

    if (run_readers == NULL)
    {
        fprintf(stderr, "ERROR: unable to allocate memory for run readers\n");
        exit(EXIT_FAILURE);
    }

    Heap heap = create_heap(options->run_count);

    fprintf(stderr, "DEBUG: creating initial nodes in heap...\n");

    int i;

    for (i = 0; i < options->run_count; i++)
    {
        run_readers[i] = open_reader(runs[i].filename, options->buffer_size);

        if (!has_reader_ended(&run_readers[i]))
        {
            Element element;
            element.reader = &run_readers[i];
            element.value = read_number(&run_readers[i]);

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

        if (!has_reader_ended(root.reader))
        {
            Element element;
            element.reader = root.reader;
            element.value = read_number(root.reader);

            replace_heap(&heap, &element);
        }
        else
        {
            pop_heap(&heap);
        }

        merged_count++;

        if (merged_count % merged_step == 0)
        {
            fprintf(stderr, "DEBUG: %d elements merged (%.2lf%%).\n", merged_count, floor(merged_count * 10000.0 / element_count) / 100.0);
        }
    }

    close_writer(&output_writer);
    stop_timer(&timer);

    int j;

    for (j = 0; j < options->run_count; j++)
    {
        close_reader(&run_readers[j]);
    }

    fprintf(stderr, "DEBUG: %d elements merged in %.2fs.\n", merged_count, get_timer_seconds(&timer));

    free_heap(&heap);
    free(run_readers);
}

void clear_runs(SortOptions *options, Run *runs)
{
    fprintf(stderr, "DEBUG: clearing runs...\n");

    int i;

    for (i = 0; i < options->run_count; i++)
    {
        remove(runs[i].filename);
    }
}

void sort_files(SortOptions *options)
{
    Run *runs = (Run *)malloc(options->run_count * sizeof(Run));

    int element_count = create_runs(options, runs);

    merge_runs(options, runs, element_count);
    clear_runs(options, runs);

    free(runs);
}