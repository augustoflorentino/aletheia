#ifndef ALETHEIA_TIMING_H
#define ALETHEIA_TIMING_H

#include "scheduler.h"

typedef struct {
    sim_time_t propagation_delay;
    sim_time_t setup_time;
    sim_time_t hold_time;
    sim_time_t clock_to_q;
} timing_params_t;

typedef struct {
    timing_params_t gate_timing;
    timing_params_t flipflop_timing;
    timing_params_t memory_timing;
    sim_time_t clock_period;
    sim_time_t clock_high_time;
    sim_time_t clock_low_time;
} timing_model_t;

void timing_model_init(timing_model_t *tm);
void timing_model_set_gate_delay(timing_model_t *tm, sim_time_t delay);
void timing_model_set_flipflop_timing(timing_model_t *tm, sim_time_t setup, sim_time_t hold, sim_time_t clk_q);
void timing_model_set_memory_timing(timing_model_t *tm, sim_time_t access_time);
void timing_model_set_clock(timing_model_t *tm, sim_time_t period, sim_time_t high_time);

sim_time_t timing_get_propagation_delay(timing_model_t *tm);
sim_time_t timing_get_setup_time(timing_model_t *tm);
sim_time_t timing_get_hold_time(timing_model_t *tm);
sim_time_t timing_get_clock_to_q(timing_model_t *tm);
sim_time_t timing_get_memory_access(timing_model_t *tm);

int timing_check_setup(timing_model_t *tm, sim_time_t data_stable_time, sim_time_t clock_edge);
int timing_check_hold(timing_model_t *tm, sim_time_t data_change_time, sim_time_t clock_edge);

#endif
