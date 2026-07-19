#include "d_latch.h"
#include "../gates/gates.h"
#include "../mux/mux2.h"

void d_latch_reset(d_latch_t *latch) {
    latch->q = 0;
    latch->q_bar = 1;
}

void d_latch_update(d_latch_t *latch, bit d, bit enable) {
    latch->q = mux2(latch->q, d, enable);
    latch->q_bar = NOT(latch->q);
}
