#include "timing.h"

void timing_model_init(timing_model_t *tm) {
    tm->gate_timing.propagation_delay = 1;
    tm->gate_timing.setup_time = 0;
    tm->gate_timing.hold_time = 0;
    tm->gate_timing.clock_to_q = 0;

    tm->flipflop_timing.propagation_delay = 1;
    tm->flipflop_timing.setup_time = 2;
    tm->flipflop_timing.hold_time = 1;
    tm->flipflop_timing.clock_to_q = 3;

    tm->memory_timing.propagation_delay = 5;
    tm->memory_timing.setup_time = 0;
    tm->memory_timing.hold_time = 0;
    tm->memory_timing.clock_to_q = 0;

    tm->clock_period = 10;
    tm->clock_high_time = 5;
    tm->clock_low_time = 5;
}

void timing_model_set_gate_delay(timing_model_t *tm, sim_time_t delay) {
    tm->gate_timing.propagation_delay = delay;
}

void timing_model_set_flipflop_timing(timing_model_t *tm, sim_time_t setup, sim_time_t hold, sim_time_t clk_q) {
    tm->flipflop_timing.setup_time = setup;
    tm->flipflop_timing.hold_time = hold;
    tm->flipflop_timing.clock_to_q = clk_q;
}

void timing_model_set_memory_timing(timing_model_t *tm, sim_time_t access_time) {
    tm->memory_timing.propagation_delay = access_time;
}

void timing_model_set_clock(timing_model_t *tm, sim_time_t period, sim_time_t high_time) {
    tm->clock_period = period;
    tm->clock_high_time = high_time;
    tm->clock_low_time = period - high_time;
}

sim_time_t timing_get_propagation_delay(timing_model_t *tm) {
    return tm->gate_timing.propagation_delay;
}

sim_time_t timing_get_setup_time(timing_model_t *tm) {
    return tm->flipflop_timing.setup_time;
}

sim_time_t timing_get_hold_time(timing_model_t *tm) {
    return tm->flipflop_timing.hold_time;
}

sim_time_t timing_get_clock_to_q(timing_model_t *tm) {
    return tm->flipflop_timing.clock_to_q;
}

sim_time_t timing_get_memory_access(timing_model_t *tm) {
    return tm->memory_timing.propagation_delay;
}

int timing_check_setup(timing_model_t *tm, sim_time_t data_stable_time, sim_time_t clock_edge) {
    if (clock_edge >= data_stable_time + tm->flipflop_timing.setup_time) {
        return 1;
    }
    return 0;
}

int timing_check_hold(timing_model_t *tm, sim_time_t data_stable_time, sim_time_t clock_edge) {
    if (data_stable_time >= clock_edge + tm->flipflop_timing.hold_time) {
        return 1;
    }
    return 0;
}
