#ifndef ALETHEIA_D_LATCH_H
#define ALETHEIA_D_LATCH_H

#include "../types.h"

typedef struct {
    bit q;
    bit q_bar;
} d_latch_t;

void d_latch_reset(d_latch_t *latch);
void d_latch_update(d_latch_t *latch, bit d, bit enable);

#endif
