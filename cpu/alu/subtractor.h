#ifndef ALETHEIA_SUBTRACTOR_H
#define ALETHEIA_SUBTRACTOR_H

#include "../../core/types.h"

void not_8(bit *a, bit *out);
void not_32(bit *a, bit *out);

void sub_8(bit *a, bit *b, bit *result);
void sub_32(bit *a, bit *b, bit *result);

void inc_8(bit *a, bit *result);
void inc_32(bit *a, bit *result);

void dec_8(bit *a, bit *result);
void dec_32(bit *a, bit *result);

void negate_32(bit *a, bit *result);

#endif
