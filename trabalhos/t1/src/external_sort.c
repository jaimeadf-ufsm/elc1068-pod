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
    for (int i = 0; i < size; i++)
    {
        write_number(writer, array[i]);
    }
}

int create_runs(Run *runs, char *input_filename, int run_count, int run_size, SortFunction sort, int buffer_size)
{
    fprintf(stderr, "DEBUG: creating runs...\n");

    BufferedReader input_reader = open_reader(input_filename, buffer_size);

    int *array = (int *)malloc(run_size * sizeof(int));
    int element_count = 0;

    for (int i = 0; i < run_count; i++)
    {
        sprintf(runs[i].filename, "%s/%06d.tmp", TMP_DIR, i);

        BufferedWriter run_writer = open_writer(runs[i].filename, buffer_size);

        Timer read_timer = start_timer();
        int n = read_run(&input_reader, array, run_size);
        stop_timer(&read_timer);

        Timer reading_timer = start_timer();
        sort(array, n);
        stop_timer(&reading_timer);

        Timer writing_timer = start_timer();
        write_run(&run_writer, array, n);
        close_writer(&run_writer);
        stop_timer(&writing_timer);

        element_count += n;

        double reading_seconds = get_timer_nanoseconds(&read_timer) / 1e9;
        double sorting_seconds = get_timer_nanoseconds(&reading_timer) / 1e9;
        double writing_seconds = get_timer_nanoseconds(&writing_timer) / 1e9;

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

    fprintf(stderr, "DEBUG: created %d runs with %d elements.\n", run_count, element_count);

    return element_count;
}

void merge_runs(Run *runs, char *output_filename, int run_count, int element_count, int buffer_size)
{
    fprintf(stderr, "DEBUG: merging runs...\n");

    BufferedReader *run_readers = (BufferedReader *)malloc(run_count * sizeof(BufferedReader));
    BufferedWriter output_writer = open_writer(output_filename, buffer_size);

    if (run_readers == NULL)
    {
        fprintf(stderr, "ERROR: unable to allocate memory for run readers\n");
        exit(EXIT_FAILURE);
    }

    Heap heap = create_heap(run_count);

    fprintf(stderr, "DEBUG: creating initial nodes in heap...\n");
    for (int i = 0; i < run_count; i++)
    {
        run_readers[i] = open_reader(runs[i].filename, buffer_size);

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
            fprintf(stderr, "DEBUG: %d elements merged (%.2lf%%).\n", merged_count, merged_count * 100.0 / element_count);
        }
    }

    close_writer(&output_writer);
    stop_timer(&timer);

    for (int i = 0; i < run_count; i++)
    {
        close_reader(&run_readers[i]);
    }

    fprintf(stderr, "DEBUG: %d elements merged in %.2fs.\n", merged_count, get_timer_nanoseconds(&timer) / 1e9);

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