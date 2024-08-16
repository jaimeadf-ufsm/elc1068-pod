#ifndef LZ78_H
#define LZ78_H

#include "common/buffered_io.h"

void lz78_compress(BufferedReader *input, BufferedWriter *output);
void lz78_decompress(BufferedReader *input, BufferedWriter *output);

#endif