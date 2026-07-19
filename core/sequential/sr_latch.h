#ifndef ALETHEIA_SR_LATCH_H
#define ALETHEIA_SR_LATCH_H

#include "../types.h"

typedef struct {
    bit q;
    bit q_bar;
} sr_latch_t;

void sr_latch_reset(sr_latch_t *latch);
void sr_latch_update(sr_latch_t *latch, bit s, bit r);

#endif
