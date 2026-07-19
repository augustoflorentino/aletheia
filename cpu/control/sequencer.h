#ifndef ALETHEIA_SEQUENCER_H
#define ALETHEIA_SEQUENCER_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"

#define MAX_MICRO_STEPS 8

typedef struct {
    d_flip_flop_t step[3];
    bit reset_seq;
    bit advance;
    bit step_out[3];
    bit decoded[8];
} sequencer_t;

void sequencer_init(sequencer_t *seq);
void sequencer_reset(sequencer_t *seq);
void sequencer_set_reset(sequencer_t *seq, bit reset);
void sequencer_set_advance(sequencer_t *seq, bit advance);
void sequencer_clock_rise(sequencer_t *seq);
void sequencer_clock_fall(sequencer_t *seq);
void sequencer_propagate(sequencer_t *seq);

bit sequencer_is_step_bits(sequencer_t *seq, bit s0, bit s1, bit s2);
void sequencer_get_step(sequencer_t *seq, bit *out);

#endif
