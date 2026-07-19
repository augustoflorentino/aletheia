#ifndef ALETHEIA_BCD_TO_BIN_H
#define ALETHEIA_BCD_TO_BIN_H

#include "../../core/types.h"

#define BCD_DIGITS 10

typedef struct {
    bit bcd[BCD_DIGITS][4];
    bit result[32];
} bcd_to_bin_t;

void bcd_to_bin_init(bcd_to_bin_t *conv);
void bcd_to_bin_reset(bcd_to_bin_t *conv);
void bcd_to_bin_set_bcd(bcd_to_bin_t *conv, bit bcd[BCD_DIGITS][4]);
void bcd_to_bin_convert(bcd_to_bin_t *conv);

void mul_by_10_32(bit input[32], bit output[32]);
void add_digit_32(bit input[32], bit digit[4], bit output[32]);

#endif
