#ifndef ALETHEIA_SIMULATOR_H
#define ALETHEIA_SIMULATOR_H

#include "../core/types.h"
#include "../system/board.h"
#include "scheduler.h"
#include "timing.h"
#include "waveform.h"
#include "debugger.h"

typedef struct {
    board_t board;
    scheduler_t scheduler;
    timing_model_t timing;
    vcd_writer_t vcd;
    debugger_t debugger;

    sim_time_t cycles;
    int verbose;

    uart_rx_t console_rx;
    int console_enabled;
    int console_prev_valid;
} simulator_t;

void simulator_init(simulator_t *sim);
void simulator_reset(simulator_t *sim);
void simulator_load_rom(simulator_t *sim, bit program[][32], int count);
void simulator_set_verbose(simulator_t *sim, int verbose);
void simulator_enable_console(simulator_t *sim);
int simulator_open_vcd(simulator_t *sim, const char *filename);
void simulator_close_vcd(simulator_t *sim);
void simulator_clock_tick(simulator_t *sim);
void simulator_propagate(simulator_t *sim);
void simulator_run_cycle(simulator_t *sim);
void simulator_run(simulator_t *sim, sim_time_t max_cycles);
void simulator_step(simulator_t *sim);
void simulator_halt(simulator_t *sim);
int simulator_is_running(simulator_t *sim);
sim_time_t simulator_get_cycles(simulator_t *sim);

#endif
