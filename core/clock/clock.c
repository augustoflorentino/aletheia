#include "clock.h"
#include "../gates/gates.h"

void clock_init(hw_clock_t *clk) {
    clk->state = 0;
    clk->rising_edge = 0;
    clk->falling_edge = 0;
}

void clock_tick(hw_clock_t *clk) {
    bit prev_state = clk->state;
    clk->state = NOT(clk->state);
    clk->rising_edge = AND(NOT(prev_state), clk->state);
    clk->falling_edge = AND(prev_state, NOT(clk->state));
}

bit clock_get_state(hw_clock_t *clk) {
    return clk->state;
}

bit clock_is_rising(hw_clock_t *clk) {
    return clk->rising_edge;
}

bit clock_is_falling(hw_clock_t *clk) {
    return clk->falling_edge;
}
