#ifndef ALETHEIA_COMPARATOR_H
#define ALETHEIA_COMPARATOR_H

#include "../../core/types.h"

bit bit_eq(bit a, bit b);
bit bit_lt(bit a, bit b);
bit bit_gt(bit a, bit b);

bit is_zero_8(bit *a);
bit is_zero_32(bit *a);

void cmp_8(bit *a, bit *b, bit *eq, bit *lt, bit *gt);
void cmp_32(bit *a, bit *b, bit *eq, bit *lt, bit *gt);

#endif
