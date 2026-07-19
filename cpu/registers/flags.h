#ifndef ALETHEIA_FLAGS_H
#define ALETHEIA_FLAGS_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"

typedef struct {
    d_flip_flop_t zero;
    d_flip_flop_t negative;
    d_flip_flop_t carry;
    d_flip_flop_t overflow;
    bit load;
    bit z_in;
    bit n_in;
    bit c_in;
    bit v_in;
} flags_register_t;

void flags_init(flags_register_t *flags);
void flags_reset(flags_register_t *flags);
void flags_set_load(flags_register_t *flags, bit load);
void flags_set_input(flags_register_t *flags, bit z, bit n, bit c, bit v);
void flags_clock_rise(flags_register_t *flags);
void flags_clock_fall(flags_register_t *flags);
void flags_propagate(flags_register_t *flags);

bit flags_get_zero(flags_register_t *flags);
bit flags_get_negative(flags_register_t *flags);
bit flags_get_carry(flags_register_t *flags);
bit flags_get_overflow(flags_register_t *flags);

#endif
