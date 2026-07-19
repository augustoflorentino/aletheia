#ifndef ALETHEIA_DIVIDER_H
#define ALETHEIA_DIVIDER_H

#include "../types.h"
#include "../sequential/t_flip_flop.h"

typedef struct {
    t_flip_flop_t ff0;
    t_flip_flop_t ff1;
    t_flip_flop_t ff2;
    t_flip_flop_t ff3;
    bit div2;
    bit div4;
    bit div8;
    bit div16;
} clock_divider_t;

void divider_init(clock_divider_t *div);
void divider_clock_rise(clock_divider_t *div);
void divider_clock_fall(clock_divider_t *div);

#endif
