#include "sr_latch.h"
#include "../gates/gates.h"

void sr_latch_reset(sr_latch_t *latch) {
    latch->q = 0;
    latch->q_bar = 1;
}

void sr_latch_update(sr_latch_t *latch, bit s, bit r) {
    latch->q = NOR(r, latch->q_bar);
    latch->q_bar = NOR(s, latch->q);
}
