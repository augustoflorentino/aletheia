#ifndef ALETHEIA_D_FLIP_FLOP_H
#define ALETHEIA_D_FLIP_FLOP_H

#include "../types.h"

typedef struct {
    bit q;
    bit q_bar;
    bit d;
    bit master_q;
} d_flip_flop_t;

void dff_reset(d_flip_flop_t *ff);
void dff_clock_rise(d_flip_flop_t *ff);
void dff_clock_fall(d_flip_flop_t *ff);
void dff_set_d(d_flip_flop_t *ff, bit d);

#endif
