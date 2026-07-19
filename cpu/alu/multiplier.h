#ifndef ALETHEIA_MULTIPLIER_H
#define ALETHEIA_MULTIPLIER_H

#include "../../core/types.h"

void and_32(bit *a, bit sel, bit *result);
void multiply(bit *m, bit *r, bit *result, bit carries[15][33]);
void multiply_simple(bit *a, bit *b, bit *result);

#endif
