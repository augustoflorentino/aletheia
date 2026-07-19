#ifndef ALETHEIA_ASCII_TO_BCD_H
#define ALETHEIA_ASCII_TO_BCD_H

#include "../../core/types.h"

void ascii_digit_to_bcd(bit ascii[8], bit bcd[4]);
void ascii_to_bcd_string(bit ascii[10][8], bit bcd[10][4]);

bit ascii_is_digit(bit ascii[8]);

#endif
