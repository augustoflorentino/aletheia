#ifndef ALETHEIA_MUX4_H
#define ALETHEIA_MUX4_H

#include "../types.h"

bit mux4(bit a, bit b, bit c, bit d, bit sel0, bit sel1);

void mux4_8(bit *a, bit *b, bit *c, bit *d, bit sel0, bit sel1, bit *out);
void mux4_32(bit *a, bit *b, bit *c, bit *d, bit sel0, bit sel1, bit *out);

#endif
