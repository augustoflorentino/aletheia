#include "d_flip_flop.h"
#include "../gates/gates.h"

void dff_reset(d_flip_flop_t *ff) {
    ff->q = 0;
    ff->q_bar = 1;
    ff->d = 0;
    ff->master_q = 0;
}

void dff_clock_rise(d_flip_flop_t *ff) {
    ff->q = ff->master_q;
    ff->q_bar = NOT(ff->master_q);
}

void dff_clock_fall(d_flip_flop_t *ff) {
    ff->master_q = ff->d;
}

void dff_set_d(d_flip_flop_t *ff, bit d) {
    ff->d = d;
}
