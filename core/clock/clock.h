#ifndef ALETHEIA_CLOCK_H
#define ALETHEIA_CLOCK_H

#include "../types.h"

typedef struct {
    bit state;
    bit rising_edge;
    bit falling_edge;
} hw_clock_t;

void clock_init(hw_clock_t *clk);
void clock_tick(hw_clock_t *clk);
bit clock_get_state(hw_clock_t *clk);
bit clock_is_rising(hw_clock_t *clk);
bit clock_is_falling(hw_clock_t *clk);

#endif
