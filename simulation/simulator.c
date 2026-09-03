#include "simulator.h"
#include <stdio.h>
#include <time.h>

void simulator_init(simulator_t *sim) {
    board_init(&sim->board);
    scheduler_init(&sim->scheduler);
    timing_model_init(&sim->timing);
    vcd_init(&sim->vcd);
    debugger_init(&sim->debugger);
    debugger_set_pc_bits(&sim->debugger, sim->board.pc.reg.data_out);

    sim->arch = SIM_ARCH_SINGLE;
    sim->hz = 0;
    sim->dram_leak_period = 64;
    sim->physics_ticks = 0;
    sim->power_off_at = 0;
    sim->power_off_cycles = 0;
    sim->fault_count = 0;
    sim->on_output = NULL;
    sim->output_ctx = NULL;
    sim->cycles = 0;
    sim->verbose = 0;
    sim->console_enabled = 0;
    sim->console_prev_valid = 0;
}

void simulator_enable_console(simulator_t *sim) {
    uart_rx_init(&sim->console_rx);
    uart_tx_init(&sim->console_tx);
    sim->console_enabled = 1;
    sim->console_prev_valid = 0;
    sim->key_head = 0;
    sim->key_tail = 0;
    sim->key_in_flight = 0;
}

int simulator_send_key(simulator_t *sim, unsigned char key) {
    if (sim->key_tail - sim->key_head >= SIM_KEY_QUEUE) return 0;
    sim->keys[sim->key_tail++ % SIM_KEY_QUEUE] = key;
    return 1;
}

static void keyboard_tick(simulator_t *sim) {
    uart_tx_propagate(&sim->console_tx);
    int avail = sim->board.uart_rx.avail_out;
    sim->key_in_flight = sim->key_in_flight && !avail;
    int clear_to_send = !avail && !sim->key_in_flight && power_is_ready(&sim->board.power);
    int send = clear_to_send && sim->console_tx.ready && sim->key_head != sim->key_tail;
    sim->key_in_flight = sim->key_in_flight || send;
    bit data[8] = {0};
    if (send) {
        unsigned char key = sim->keys[sim->key_head++ % SIM_KEY_QUEUE];
        for (int i = 0; i < 8; i++) data[i] = (bit)((key >> i) & 1u);
    }
    uart_tx_set_data(&sim->console_tx, data);
    uart_tx_set_load(&sim->console_tx, (bit)send);
    uart_tx_set_clear_status(&sim->console_tx, 0);
    uart_tx_clock_fall(&sim->console_tx);
    uart_tx_clock_rise(&sim->console_tx);
    uart_tx_propagate(&sim->console_tx);
    sim->board.serial_in = sim->console_tx.serial_out;
}

int simulator_add_fault(simulator_t *sim, const fault_t *f) {
    if (sim->fault_count >= MAX_FAULTS) return 0;
    sim->faults[sim->fault_count++] = *f;
    return 1;
}

static void inject_faults(simulator_t *sim) {
    for (int i = 0; i < sim->fault_count; i++) {
        if (sim->faults[i].cycle == sim->cycles) fault_apply(&sim->board, &sim->faults[i]);
    }
}

static void physics_tick(simulator_t *sim) {
    long period = sim->dram_leak_period;
    sim->physics_ticks++;
    sim->board.dram_leak = (bit)(period > 0 && sim->physics_ticks % (unsigned long long)period == 0);
}

void simulator_power_cycle(simulator_t *sim, long off_cycles) {
    board_t *b = &sim->board;
    board_power_off(b);
    dram_set_write(&b->dram, 0);
    dram_set_refresh(&b->dram, 0);
    for (long i = 0; i < off_cycles; i++) {
        physics_tick(sim);
        dram_set_leak(&b->dram, b->dram_leak);
        dram_clock_fall(&b->dram);
        dram_clock_rise(&b->dram);
    }
    board_reset(b);
    board_pipe_init(b);
    simulator_power_up(sim);
}

void simulator_power_up(simulator_t *sim) {
    board_power_on(&sim->board);
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
        if (sim->on_output) {
            sim->on_output(sim->output_ctx, (int)byte);
        } else {
            putchar((int)byte);
            fflush(stdout);
        }
    }
    sim->console_prev_valid = valid;
}

void simulator_reset(simulator_t *sim) {
    board_reset(&sim->board);
    scheduler_reset(&sim->scheduler);
    debugger_reset(&sim->debugger);

    sim->cycles = 0;
}

void rom_load_at(rom_1kx32_t *rom, unsigned index, bit *word) {
    bit addr[10];
    for (int k = 0; k < 10; k++) addr[k] = (bit)((index >> k) & 1u);
    rom1k_program(rom, addr, word);
}

void simulator_load_rom(simulator_t *sim, bit program[][32], int count) {
    for (int i = 0; i < count && i < ROM_WORDS; i++) {
        rom_load_at(&sim->board.rom, (unsigned)i, program[i]);
    }
}

void simulator_set_arch(simulator_t *sim, sim_arch_t arch) {
    sim->arch = arch;
    board_pipe_init(&sim->board);
}

static double now_seconds(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
}

void simulator_set_hz(simulator_t *sim, long hz) {
    sim->hz = hz;
    sim->pace_start = now_seconds();
    sim->pace_cycles = 0;
}

static void pace(simulator_t *sim) {
    sim->pace_cycles++;
    double due = sim->pace_start + (double)sim->pace_cycles / (double)sim->hz;
    double wait = due - now_seconds();
    if (wait <= 0) return;
    struct timespec t = {(time_t)wait, (long)((wait - (double)(time_t)wait) * 1e9)};
    nanosleep(&t, NULL);
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

    board_t *b = &sim->board;
    sim->vcd_clk = 0;
    vcd_begin_module(&sim->vcd, "board");
    vcd_add_signal(&sim->vcd, "clk", 1, &sim->vcd_clk);
    vcd_add_signal(&sim->vcd, "halt", 1, &b->halt);
    vcd_add_signal(&sim->vcd, "pc", 32, b->pc.reg.data_out);
    vcd_add_signal(&sim->vcd, "uart_tx", 1, &b->uart.serial_out);
    vcd_add_signal(&sim->vcd, "irq", 1, &b->pic.int_request);
    vcd_add_signal(&sim->vcd, "in_exception", 1, &b->exc.in_exception.q);
    vcd_add_signal(&sim->vcd, "mmu_en", 1, &b->mmu_en.q);
    vcd_begin_module(&sim->vcd, "regs");
    for (int i = 1; i < 32; i++) {
        char name[8];
        snprintf(name, sizeof name, "r%d", i);
        vcd_add_signal(&sim->vcd, name, 32, b->regs.regs[i].data_out);
    }
    vcd_end_module(&sim->vcd);
    vcd_end_module(&sim->vcd);

    vcd_write_initial(&sim->vcd);

    return 1;
}

void simulator_close_vcd(simulator_t *sim) {
    vcd_close(&sim->vcd);
}

void simulator_clock_tick(simulator_t *sim) {
    static void (*const step[])(board_t *) = {board_step, board_step_mc, board_step_pipe};
    board_clock(&sim->board, step[sim->arch]);
}

void simulator_propagate(simulator_t *sim) {
    board_propagate(&sim->board);
}

void simulator_run_cycle(simulator_t *sim) {
    if (sim->console_enabled) {
        keyboard_tick(sim);
    }
    inject_faults(sim);
    physics_tick(sim);
    simulator_clock_tick(sim);
    if (sim->console_enabled) {
        console_tick(sim);
    }
    simulator_propagate(sim);

    sim_time_t t = sim->cycles * sim->timing.clock_period;
    sim->vcd_clk = 1;
    vcd_update(&sim->vcd, t);
    sim->vcd_clk = 0;
    vcd_update(&sim->vcd, t + sim->timing.clock_period / 2);

    sim->cycles++;
    if (sim->power_off_cycles > 0 && sim->cycles == sim->power_off_at) {
        simulator_power_cycle(sim, sim->power_off_cycles);
    }
    if (sim->hz > 0) pace(sim);
}

void simulator_run(simulator_t *sim, sim_time_t max_cycles) {
    simulator_power_up(sim);

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
