#include "jk_flip_flop.h"
#include "../gates/gates.h"

void jkff_reset(jk_flip_flop_t *ff) {
    ff->q = 0;
    ff->q_bar = 1;
    ff->j = 0;
    ff->k = 0;
    ff->master_q = 0;
}

void jkff_clock_rise(jk_flip_flop_t *ff) {
    ff->q = ff->master_q;
    ff->q_bar = NOT(ff->master_q);
}

void jkff_clock_fall(jk_flip_flop_t *ff) {
    bit set = AND(ff->j, ff->q_bar);
    bit reset = AND(ff->k, ff->q);
    bit toggle = AND(ff->j, ff->k);
    bit hold = NOR(ff->j, ff->k);

    bit new_q_from_set = AND(set, NOT(reset));
    bit new_q_from_toggle = AND(toggle, ff->q_bar);
    bit new_q_from_hold = AND(hold, ff->q);

    ff->master_q = OR3(new_q_from_set, new_q_from_toggle, new_q_from_hold);
}

void jkff_set_jk(jk_flip_flop_t *ff, bit j, bit k) {
    ff->j = j;
    ff->k = k;
}
