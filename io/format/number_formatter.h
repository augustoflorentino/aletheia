#ifndef ALETHEIA_NUMBER_FORMATTER_H
#define ALETHEIA_NUMBER_FORMATTER_H

#include "../../core/types.h"

#define NF_DIGITS 10

void number_format(bit input[32], bit ascii[NF_DIGITS][8]);
void number_format_signed(bit input[32], bit ascii[NF_DIGITS][8]);

#endif
