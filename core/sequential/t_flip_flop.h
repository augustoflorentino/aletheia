#ifndef ALETHEIA_T_FLIP_FLOP_H
#define ALETHEIA_T_FLIP_FLOP_H

#include "../types.h"

typedef struct {
    bit q;
    bit q_bar;
    bit t;
    bit master_q;
} t_flip_flop_t;

void tff_reset(t_flip_flop_t *ff);
void tff_clock_rise(t_flip_flop_t *ff);
void tff_clock_fall(t_flip_flop_t *ff);
void tff_set_t(t_flip_flop_t *ff, bit t);

#endif
