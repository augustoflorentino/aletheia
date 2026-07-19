#ifndef ALETHEIA_DIVIDER_H
#define ALETHEIA_DIVIDER_H

#include "../../core/types.h"

void copy_32(bit *src, bit *dst);
void div_step(bit *remainder, bit dividend_bit, bit *divisor, bit *new_remainder, bit *quotient_bit);
void div_32(bit *dividend, bit *divisor, bit *quotient, bit *remainder);
void mod_32(bit *dividend, bit *divisor, bit *remainder);

#endif
