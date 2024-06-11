#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <math.h>

#include "common/input.h"
#include "common/buffered_io.h"

#define PROGRESS_STEPS 20
#define BUFFER_SIZE 16 * 1024

int random_number(int modulo, bool sign)
{
    int number = rand() % modulo;

    if (sign && rand() % 2 == 0)
    {
        number = -number;
    }

    return number;
}

long long generate_random_numbers(BufferedWriter *writer, long long n, int modulo, bool sign)
{
    fprintf(stderr, "DEBUG: generating %lld numbers...\n", n);

    long long step = n / PROGRESS_STEPS;
    long long count = 0;

    if (step == 0)
    {
        step = 1;
    }

    while (count < n)
    {
        write_number(writer, random_number(modulo, sign));

        count++;

        if (count % step == 0)
        {
            fprintf(stderr, "DEBUG: %lld numbers generated (%.2lf%%).\n", count, floor(count * 10000.0 / n) / 100.0);
        }
    }

    return n;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <count/size> <filename> [signed] [modulo] [seed]\n", argv[0]);
        return 1;
    }

    char *amount = argv[1];
    char *filename = argv[2];

    int modulo = RAND_MAX;
    bool sign = false;
    int seed = time(NULL);

    if (argc >= 4)
    {
        sign = parse_bool_or_exit(argv[3]);
    }

    if (argc >= 5)
    {

        modulo = parse_int_or_exit(argv[4]);
    }

    if (argc >= 6)
    {
        seed = parse_int_or_exit(argv[5]);
    }

    long long n = 0;

    if (is_size(amount))
    {
        n = parse_size_or_exit(amount) / sizeof(int);
    }
    else
    {
        n = parse_long_long_or_exit(amount);
    }

    srand(seed);

    BufferedWriter writer = open_writer(filename, BUFFER_SIZE);
    generate_random_numbers(&writer, n, modulo, sign);
    close_writer(&writer);

    fprintf(stderr, "DEBUG: generated %lld numbers.\n", n);

    return 0;
}
