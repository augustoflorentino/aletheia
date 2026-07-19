#include "t_flip_flop.h"
#include "../gates/gates.h"

void tff_reset(t_flip_flop_t *ff) {
    ff->q = 0;
    ff->q_bar = 1;
    ff->t = 0;
    ff->master_q = 0;
}

void tff_clock_rise(t_flip_flop_t *ff) {
    ff->q = ff->master_q;
    ff->q_bar = NOT(ff->master_q);
}

void tff_clock_fall(t_flip_flop_t *ff) {
    bit toggle = AND(ff->t, 1);
    bit toggled_value = XOR(ff->q, toggle);
    ff->master_q = toggled_value;
}

void tff_set_t(t_flip_flop_t *ff, bit t) {
    ff->t = t;
}
