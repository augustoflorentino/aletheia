#ifndef ALETHEIA_LOGIC_UNIT_H
#define ALETHEIA_LOGIC_UNIT_H

#include "../../core/types.h"

void and_word_32(bit *a, bit *b, bit *result);
void or_word_32(bit *a, bit *b, bit *result);
void xor_word_32(bit *a, bit *b, bit *result);
void not_word_32(bit *a, bit *result);
void nand_word_32(bit *a, bit *b, bit *result);
void nor_word_32(bit *a, bit *b, bit *result);

#endif
