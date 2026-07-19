#ifndef ALETHEIA_BCD_TO_ASCII_H
#define ALETHEIA_BCD_TO_ASCII_H

#include "../../core/types.h"

void bcd_digit_to_ascii(bit bcd[4], bit ascii[8]);
void bcd_to_ascii_string(bit bcd[10][4], bit ascii[10][8]);

#endif
