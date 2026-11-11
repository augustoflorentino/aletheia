#include "../system/board.h"
#include "../isa/assembler.h"
#include "../synth/lanes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define OUT_MAX 256

typedef struct {
    long gate;
    int value;
    char out[OUT_MAX];
    int len;
    long halted_at;
} lane_t;

static board_t board;
static uart_tx_t keyboard;
static uart_rx_t console;
static lane_t lane[LANE_COUNT];
static const char *keys;
static int key_head[LANE_COUNT];
static bit in_flight;
static bit prev_valid;

static int lane_on(bit mask, int l) {
    return (int)((lanes_of(mask) >> l) & 1);
}

static void keyboard_tick(void) {
    uart_tx_propagate(&keyboard);
    bit avail = lanes_of(board.uart_rx.avail_out);
    in_flight &= ~avail;
    bit clear = ~avail & ~in_flight & lanes_of(keyboard.ready) & lanes_of(board.power.ready) & LANE_MACHINES;
    bit send = 0, data[8] = {0};
    int key_count = (int)strlen(keys);
    for (int l = 1; l < LANE_COUNT; l++) {
        if (!((clear >> l) & 1) || key_head[l] >= key_count) continue;
        unsigned char k = (unsigned char)keys[key_head[l]++];
        send |= 1ull << l;
        for (int i = 0; i < 8; i++) data[i] |= (bit)((k >> i) & 1u) << l;
    }
    in_flight |= send;
    uart_tx_set_data(&keyboard, data);
    uart_tx_set_load(&keyboard, send);
    uart_tx_set_clear_status(&keyboard, 0);
    uart_tx_clock_fall(&keyboard);
    uart_tx_clock_rise(&keyboard);
    uart_tx_propagate(&keyboard);
    board.serial_in = keyboard.serial_out;
}

static void console_tick(void) {
    uart_rx_set_serial(&console, board.uart.serial_out);
    uart_rx_clock_fall(&console);
    uart_rx_clock_rise(&console);
    uart_rx_propagate(&console);
    bit valid = lanes_of(console.valid_out);
    bit fresh = valid & ~prev_valid;
    prev_valid = valid;
    for (int l = 1; l < LANE_COUNT; l++) {
        if (!((fresh >> l) & 1) || lane[l].len >= OUT_MAX - 1) continue;
        int byte = 0;
        for (int i = 0; i < 8; i++) byte |= lane_on(console.data_out[i], l) << i;
        lane[l].out[lane[l].len++] = (char)byte;
    }
}

static void run_cycle(void (*step)(board_t *), long cycle) {
    keyboard_tick();
    board.dram_leak = (bit)(((cycle + 1) % 64) == 0);
    lanes_new_cycle();
    board_clock(&board, step);
    console_tick();
    board_propagate(&board);
    bit halt = lanes_of(board.halt);
    for (int l = 1; l < LANE_COUNT; l++)
        if (lane[l].halted_at < 0 && ((halt >> l) & 1)) lane[l].halted_at = cycle + 1;
}

static void load(const unsigned *words, int count) {
    for (int i = 0; i < count; i++) {
        bit addr[10], data[32];
        for (int k = 0; k < 10; k++) addr[k] = (bit)((i >> k) & 1);
        for (int k = 0; k < 32; k++) data[k] = (bit)((words[i] >> k) & 1u);
        rom1k_program(&board.rom, addr, data);
    }
}

static void run_batch(const unsigned *words, int count, void (*step)(board_t *), long cycles, int used) {
    board_init(&board);
    board_pipe_init(&board);
    load(words, count);
    uart_tx_init(&keyboard);
    uart_rx_init(&console);
    memset(key_head, 0, sizeof key_head);
    in_flight = 0;
    prev_valid = 0;
    board_power_on(&board);
    bit all = 0;
    for (int l = 1; l <= used; l++) all |= 1ull << l;
    long c = 0, drain = 0;
    for (; c < cycles && drain < 12; c++) {
        run_cycle(step, c);
        drain += (lanes_of(board.halt) & all) == all;
    }
}

static void print_lane(const lane_t *l, int diverged) {
    printf("%ld:%d %s %ld \"", l->gate, l->value, diverged ? "diverged" : "ok", l->halted_at);
    for (int i = 0; i < l->len; i++) {
        unsigned char ch = (unsigned char)l->out[i];
        if (ch == '\n') printf("\\n");
        else if (ch >= 32 && ch < 127 && ch != '"' && ch != '\\') putchar(ch);
        else printf("\\x%02x", ch);
    }
    printf("\"\n");
}

int main(int argc, char **argv) {
    if (argc < 5) {
        fprintf(stderr, "usage: %s PROGRAM.asm single|multi|pipe CYCLES KEYS < faults (GATE:VALUE per line)\n", argv[0]);
        return 1;
    }
    static void (*const steps[])(board_t *) = {board_step, board_step_mc, board_step_pipe};
    int arch = strcmp(argv[2], "multi") == 0 ? 1 : strcmp(argv[2], "pipe") == 0 ? 2
                                                                                : 0;
    long cycles = atol(argv[3]);
    keys = argv[4];
    static unsigned words[ROM_WORDS];
    assembler_t as;
    assembler_init(&as);
    int count = assembler_assemble_file(&as, argv[1], words, ROM_WORDS);
    if (count < 0) {
        fprintf(stderr, "%s\n", as.error);
        return 1;
    }
    long gate;
    int value, used = 0, batches = 0;
    clock_t t0 = clock();
    for (;;) {
        int more = scanf("%ld:%d", &gate, &value) == 2;
        if (more) {
            used++;
            lane[used] = (lane_t){.gate = gate, .value = value, .halted_at = -1};
            lanes_arm(used, gate, value);
        }
        if (used == 0 && !more) break;
        if (used < LANE_COUNT - 1 && more) continue;
        run_batch(words, count, steps[arch], cycles, used);
        batches++;
        for (int l = 1; l <= used; l++) print_lane(&lane[l], (int)((lanes_diverged() >> l) & 1));
        fflush(stdout);
        used = 0;
        lanes_reset();
        if (!more) break;
    }
    fprintf(stderr, "%d batches, %.1f s cpu\n", batches, (double)(clock() - t0) / CLOCKS_PER_SEC);
    return 0;
}
