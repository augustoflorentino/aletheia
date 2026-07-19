#ifndef ALETHEIA_REPLACEMENT_H
#define ALETHEIA_REPLACEMENT_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"

typedef struct {
    d_flip_flop_t counter[2];

    bit advance;

    bit victim[2];
} fifo_replacement_t;

void fifo_init(fifo_replacement_t *f);
void fifo_reset(fifo_replacement_t *f);
void fifo_set_advance(fifo_replacement_t *f, bit advance);
void fifo_clock_fall(fifo_replacement_t *f);
void fifo_clock_rise(fifo_replacement_t *f);
void fifo_propagate(fifo_replacement_t *f);

#endif
