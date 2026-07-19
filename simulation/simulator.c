#include "simulator.h"
#include <stdio.h>

void simulator_init(simulator_t *sim) {
    board_init(&sim->board);
    scheduler_init(&sim->scheduler);
    timing_model_init(&sim->timing);
    vcd_init(&sim->vcd);
    debugger_init(&sim->debugger);

    sim->cycles = 0;
    sim->verbose = 0;
    sim->console_enabled = 0;
    sim->console_prev_valid = 0;
}

void simulator_enable_console(simulator_t *sim) {
    uart_rx_init(&sim->console_rx);
    sim->console_enabled = 1;
    sim->console_prev_valid = 0;
}

static void console_tick(simulator_t *sim) {
    uart_rx_set_serial(&sim->console_rx, sim->board.uart.serial_out);
    uart_rx_clock_fall(&sim->console_rx);
    uart_rx_clock_rise(&sim->console_rx);
    uart_rx_propagate(&sim->console_rx);

    int valid = sim->console_rx.valid_out;
    if (valid && !sim->console_prev_valid) {
        unsigned int byte = 0;
        for (int i = 0; i < 8; i++) {
            byte |= ((unsigned int)(sim->console_rx.data_out[i] & 1)) << i;
        }
        putchar((int)byte);
        fflush(stdout);
    }
    sim->console_prev_valid = valid;
}

void simulator_reset(simulator_t *sim) {
    board_reset(&sim->board);
    scheduler_reset(&sim->scheduler);
    debugger_reset(&sim->debugger);

    sim->cycles = 0;
}

void simulator_load_rom(simulator_t *sim, bit program[][32], int count) {
    for (int i = 0; i < count && i < ROM256_SIZE; i++) {
        rom256_load_word(&sim->board.rom, i, program[i]);
    }
}

void simulator_set_verbose(simulator_t *sim, int verbose) {
    sim->verbose = verbose;
}

int simulator_open_vcd(simulator_t *sim, const char *filename) {
    if (!vcd_open(&sim->vcd, filename)) {
        return 0;
    }

    vcd_write_header(&sim->vcd);
    vcd_set_timescale(&sim->vcd, "1ns");

    vcd_begin_module(&sim->vcd, "board");
    vcd_add_signal(&sim->vcd, "clk", 1, &sim->board.clock_out);
    vcd_add_signal(&sim->vcd, "rst", 1, &sim->board.reset_out);
    vcd_add_signal(&sim->vcd, "halt", 1, &sim->board.halt);
    vcd_end_module(&sim->vcd);

    vcd_write_initial(&sim->vcd);

    return 1;
}

void simulator_close_vcd(simulator_t *sim) {
    vcd_close(&sim->vcd);
}

void simulator_clock_tick(simulator_t *sim) {
    board_step(&sim->board);
}

void simulator_propagate(simulator_t *sim) {
    board_propagate(&sim->board);
}

void simulator_run_cycle(simulator_t *sim) {
    simulator_clock_tick(sim);
    if (sim->console_enabled) {
        console_tick(sim);
    }
    simulator_propagate(sim);

    vcd_update(&sim->vcd, sim->cycles * sim->timing.clock_period);

    sim->cycles++;
}

void simulator_run(simulator_t *sim, sim_time_t max_cycles) {
    board_power_on(&sim->board);

    sim_time_t start_cycle = sim->cycles;
    sim_time_t end_cycle = start_cycle + max_cycles;

    while (sim->cycles < end_cycle) {
        if (debugger_check(&sim->debugger)) {
            break;
        }

        simulator_run_cycle(sim);

        if (sim->board.halt) {
            break;
        }

        if (sim->verbose && (sim->cycles % 1000) == 0) {
            printf("Cycle: %llu\n", sim->cycles);
        }
    }

    if (sim->console_enabled && sim->board.halt) {
        for (int i = 0; i < 12; i++) {
            simulator_run_cycle(sim);
        }
    }
}

void simulator_step(simulator_t *sim) {
    simulator_run_cycle(sim);
    debugger_check(&sim->debugger);
}

void simulator_halt(simulator_t *sim) {
    sim->board.halt = 1;
    scheduler_halt(&sim->scheduler);
}

int simulator_is_running(simulator_t *sim) {
    return board_is_running(&sim->board) && !debugger_should_break(&sim->debugger);
}

sim_time_t simulator_get_cycles(simulator_t *sim) {
    return sim->cycles;
}
