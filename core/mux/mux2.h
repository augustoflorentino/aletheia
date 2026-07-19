#ifndef ALETHEIA_MUX2_H
#define ALETHEIA_MUX2_H

#include "../types.h"

bit mux2(bit a, bit b, bit sel);

void mux2_8(bit *a, bit *b, bit sel, bit *out);
void mux2_32(bit *a, bit *b, bit sel, bit *out);

#endif
