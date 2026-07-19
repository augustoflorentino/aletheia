#ifndef ALETHEIA_JK_FLIP_FLOP_H
#define ALETHEIA_JK_FLIP_FLOP_H

#include "../types.h"

typedef struct {
    bit q;
    bit q_bar;
    bit j;
    bit k;
    bit master_q;
} jk_flip_flop_t;

void jkff_reset(jk_flip_flop_t *ff);
void jkff_clock_rise(jk_flip_flop_t *ff);
void jkff_clock_fall(jk_flip_flop_t *ff);
void jkff_set_jk(jk_flip_flop_t *ff, bit j, bit k);

#endif
