#ifndef ALETHEIA_BIN_TO_BCD_H
#define ALETHEIA_BIN_TO_BCD_H

#include "../../core/types.h"

#define BCD_DIGITS 10

typedef struct {
    bit input[32];
    bit bcd[BCD_DIGITS][4];

    bit shift_reg[32 + BCD_DIGITS * 4];
} bin_to_bcd_t;

void bin_to_bcd_init(bin_to_bcd_t *conv);
void bin_to_bcd_reset(bin_to_bcd_t *conv);
void bin_to_bcd_set_input(bin_to_bcd_t *conv, bit input[32]);
void bin_to_bcd_convert(bin_to_bcd_t *conv);

bit bin_to_bcd_digit_gte5(bit d[4]);
void bin_to_bcd_add3_if_gte5(bit d[4], bit result[4]);
void bin_to_bcd_shift_once(bin_to_bcd_t *conv);

#endif
