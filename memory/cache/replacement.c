#include "replacement.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

void fifo_init(fifo_replacement_t *f) {
    dff_reset(&f->counter[0]);
    dff_reset(&f->counter[1]);
    f->advance = 0;
}

void fifo_reset(fifo_replacement_t *f) {
    dff_reset(&f->counter[0]);
    dff_reset(&f->counter[1]);
    f->advance = 0;
}

void fifo_set_advance(fifo_replacement_t *f, bit advance) {
    f->advance = advance;
}

void fifo_clock_fall(fifo_replacement_t *f) {
    bit c0 = f->counter[0].q;
    bit c1 = f->counter[1].q;

    bit inc0 = NOT(c0);
    bit inc1 = XOR(c1, c0);

    f->counter[0].d = mux2(c0, inc0, f->advance);
    f->counter[1].d = mux2(c1, inc1, f->advance);

    dff_clock_fall(&f->counter[0]);
    dff_clock_fall(&f->counter[1]);
}

void fifo_clock_rise(fifo_replacement_t *f) {
    dff_clock_rise(&f->counter[0]);
    dff_clock_rise(&f->counter[1]);
}

void fifo_propagate(fifo_replacement_t *f) {
    f->victim[0] = f->counter[0].q;
    f->victim[1] = f->counter[1].q;
}
