#include "sequencer.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

void sequencer_init(sequencer_t *seq) {
    dff_reset(&seq->step[0]);
    dff_reset(&seq->step[1]);
    dff_reset(&seq->step[2]);
    seq->reset_seq = 0;
    seq->advance = 0;
    seq->step_out[0] = 0;
    seq->step_out[1] = 0;
    seq->step_out[2] = 0;
    seq->decoded[0] = 1;
    seq->decoded[1] = 0;
    seq->decoded[2] = 0;
    seq->decoded[3] = 0;
    seq->decoded[4] = 0;
    seq->decoded[5] = 0;
    seq->decoded[6] = 0;
    seq->decoded[7] = 0;
}

void sequencer_reset(sequencer_t *seq) {
    sequencer_init(seq);
}

void sequencer_set_reset(sequencer_t *seq, bit reset) {
    seq->reset_seq = reset;
}

void sequencer_set_advance(sequencer_t *seq, bit advance) {
    seq->advance = advance;
}

void sequencer_clock_rise(sequencer_t *seq) {
    dff_clock_rise(&seq->step[0]);
    dff_clock_rise(&seq->step[1]);
    dff_clock_rise(&seq->step[2]);
}

void sequencer_clock_fall(sequencer_t *seq) {
    bit s0 = seq->step[0].q;
    bit s1 = seq->step[1].q;
    bit s2 = seq->step[2].q;

    bit next0 = XOR(s0, seq->advance);
    bit carry0 = AND(s0, seq->advance);

    bit next1 = XOR(s1, carry0);
    bit carry1 = AND(s1, carry0);

    bit next2 = XOR(s2, carry1);

    seq->step[0].d = mux2(next0, 0, seq->reset_seq);
    seq->step[1].d = mux2(next1, 0, seq->reset_seq);
    seq->step[2].d = mux2(next2, 0, seq->reset_seq);

    dff_clock_fall(&seq->step[0]);
    dff_clock_fall(&seq->step[1]);
    dff_clock_fall(&seq->step[2]);
}

void sequencer_propagate(sequencer_t *seq) {
    seq->step_out[0] = seq->step[0].q;
    seq->step_out[1] = seq->step[1].q;
    seq->step_out[2] = seq->step[2].q;

    bit s0 = seq->step_out[0];
    bit s1 = seq->step_out[1];
    bit s2 = seq->step_out[2];
    bit not_s0 = NOT(s0);
    bit not_s1 = NOT(s1);
    bit not_s2 = NOT(s2);

    seq->decoded[0] = AND(not_s2, AND(not_s1, not_s0));
    seq->decoded[1] = AND(not_s2, AND(not_s1, s0));
    seq->decoded[2] = AND(not_s2, AND(s1, not_s0));
    seq->decoded[3] = AND(not_s2, AND(s1, s0));
    seq->decoded[4] = AND(s2, AND(not_s1, not_s0));
    seq->decoded[5] = AND(s2, AND(not_s1, s0));
    seq->decoded[6] = AND(s2, AND(s1, not_s0));
    seq->decoded[7] = AND(s2, AND(s1, s0));
}

bit sequencer_is_step_bits(sequencer_t *seq, bit s0, bit s1, bit s2) {
    bit t0 = mux2(seq->decoded[0], seq->decoded[1], s0);
    bit t1 = mux2(seq->decoded[2], seq->decoded[3], s0);
    bit t2 = mux2(seq->decoded[4], seq->decoded[5], s0);
    bit t3 = mux2(seq->decoded[6], seq->decoded[7], s0);
    bit t4 = mux2(t0, t1, s1);
    bit t5 = mux2(t2, t3, s1);
    return mux2(t4, t5, s2);
}

void sequencer_get_step(sequencer_t *seq, bit *out) {
    out[0] = seq->step_out[0];
    out[1] = seq->step_out[1];
    out[2] = seq->step_out[2];
}
