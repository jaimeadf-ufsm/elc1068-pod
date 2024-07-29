#ifndef LZ78_H
#define LZ78_H

#include <stddef.h>
#include "common/buffer.h"

void lz78_compress(Buffer *input_buffer, Buffer *output_buffer);
void lz78_decompress(Buffer *input_buffer, Buffer *output_buffer);

#endif