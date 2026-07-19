#ifndef ALETHEIA_ADDER_H
#define ALETHEIA_ADDER_H

#include "../../core/types.h"

void half_adder(bit a, bit b, bit *sum, bit *carry);
void full_adder(bit a, bit b, bit cin, bit *sum, bit *cout);

void add_4bit(bit *a, bit *b, bit cin, bit *result, bit *cout);

void adder_8(bit *a, bit *b, bit *result, bit *carries);
void adder_8_cin(bit *a, bit *b, bit cin, bit *result);

void adder_32(bit *a, bit *b, bit *result, bit *carries);
void adder_32_cin(bit *a, bit *b, bit cin, bit *result);

#endif
