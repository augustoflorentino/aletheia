#ifndef ALETHEIA_SHIFTER_H
#define ALETHEIA_SHIFTER_H

#include "../../core/types.h"

void shl_32_1(bit *a, bit *out);
void shl_32_2(bit *a, bit *out);
void shl_32_4(bit *a, bit *out);
void shl_32_8(bit *a, bit *out);
void shl_32_16(bit *a, bit *out);

void shr_32_1(bit *a, bit *out);
void shr_32_2(bit *a, bit *out);
void shr_32_4(bit *a, bit *out);
void shr_32_8(bit *a, bit *out);
void shr_32_16(bit *a, bit *out);

void shl_32(bit *a, bit *n, bit *out);
void shr_32(bit *a, bit *n, bit *out);

#endif
