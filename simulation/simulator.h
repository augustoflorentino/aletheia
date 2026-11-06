#ifndef ALETHEIA_SIMULATOR_H
#define ALETHEIA_SIMULATOR_H

#include "../core/types.h"
#include "../system/board.h"
#include "scheduler.h"
#include "timing.h"
#include "waveform.h"
#include "debugger.h"
#include "fault.h"
#include "power_trace.h"

#define SIM_KEY_QUEUE 256

typedef enum { SIM_ARCH_SINGLE,
               SIM_ARCH_MULTI,
               SIM_ARCH_PIPE } sim_arch_t;

typedef struct {
    board_t board;
    sim_arch_t arch;
    scheduler_t scheduler;
    timing_model_t timing;
    vcd_writer_t vcd;
    bit vcd_clk;
    debugger_t debugger;

    sim_time_t cycles;
    int verbose;

    uart_rx_t console_rx;
    int console_enabled;
    int console_prev_valid;
    uart_tx_t console_tx;
    unsigned char keys[SIM_KEY_QUEUE];
    unsigned key_head;
    unsigned key_tail;
    int key_in_flight;
    void (*on_output)(void *ctx, int byte);
    void *output_ctx;
    fault_t faults[MAX_FAULTS];
    int fault_count;
    long hz;
    long dram_leak_period;
    unsigned long long physics_ticks;
    unsigned long long power_off_at;
    long power_off_cycles;
    power_trace_t *power;
    unsigned long long slot_at;
    unsigned slot_regs[4];
    double pace_start;
    sim_time_t pace_cycles;
} simulator_t;

void simulator_init(simulator_t *sim);
void simulator_reset(simulator_t *sim);
void rom_load_at(rom_1kx32_t *rom, unsigned index, bit *word);
void simulator_load_rom(simulator_t *sim, bit program[][32], int count);
void simulator_set_verbose(simulator_t *sim, int verbose);
void simulator_set_arch(simulator_t *sim, sim_arch_t arch);
void simulator_set_hz(simulator_t *sim, long hz);
void simulator_enable_console(simulator_t *sim);
int simulator_send_key(simulator_t *sim, unsigned char key);
void simulator_power_up(simulator_t *sim);
void simulator_power_cycle(simulator_t *sim, long off_cycles);
int simulator_add_fault(simulator_t *sim, const fault_t *f);
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
