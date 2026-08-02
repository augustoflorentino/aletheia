#ifndef ALETHEIA_TIMER_H
#define ALETHEIA_TIMER_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"

typedef struct {
    d_flip_flop_t counter[16];
    d_flip_flop_t reload[16];
    d_flip_flop_t control;
    d_flip_flop_t control_reload;
    d_flip_flop_t irq_pending;

    bit counter_in[16];
    bit reload_in[16];
    bit control_in;
    bit control_reload_in;

    bit counter_out[16];
    bit reload_out[16];

    bit chip_select;
    bit read;
    bit write;
    bit addr[2];

    bit tick;
    bit enabled;
    bit auto_reload;
    bit irq;
    bit zero;
} hw_timer_t;

void timer_init(hw_timer_t *tmr);
void timer_reset(hw_timer_t *tmr);
void timer_set_cs(hw_timer_t *tmr, bit cs);
void timer_set_read(hw_timer_t *tmr, bit rd);
void timer_set_write(hw_timer_t *tmr, bit wr);
void timer_set_addr(hw_timer_t *tmr, bit a0, bit a1);
void timer_set_data(hw_timer_t *tmr, bit *data);
void timer_set_tick(hw_timer_t *tmr, bit tick);
void timer_clock_rise(hw_timer_t *tmr);
void timer_clock_fall(hw_timer_t *tmr);
void timer_propagate(hw_timer_t *tmr);
void timer_read_data(hw_timer_t *tmr, bit *out);
bit timer_get_irq(hw_timer_t *tmr);
bit timer_get_zero(hw_timer_t *tmr);

#endif
