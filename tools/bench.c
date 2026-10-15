#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../system/board.h"
#include "../simulation/simulator.h"
#include "../isa/encoding.h"

#ifdef ALETHEIA_NETLIST
#include "../synth/netlist.h"
#endif

static const char *arch_names[] = {"single", "multi", "pipe"};
static void (*const steps[])(board_t *) = {board_step, board_step_mc, board_step_pipe};

static void load_loop(board_t *b) {
    unsigned prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 1),
        encode_r_type(REG_T0, REG_T1, REG_T0, 0, FUNCT_ADD),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 4),
        encode_i_type(OP_LW, REG_ZERO, REG_T2, 4),
        encode_i_type(OP_BEQ, REG_ZERO, REG_ZERO, (unsigned)-4 & 0xFFFFu),
    };
    for (unsigned i = 0; i < sizeof prog / sizeof prog[0]; i++) {
        bit w[32];
        for (int k = 0; k < 32; k++) w[k] = (bit)((prog[i] >> k) & 1u);
        rom_load_at(&b->rom, i, w);
    }
}

static board_t board;

#ifdef ALETHEIA_NETLIST
static void measure(int arch, long cycles) {
    (void)cycles;
    board_init(&board);
    board_pipe_init(&board);
    nl_reset();
    load_loop(&board);
    int before = nl_nands();
    steps[arch](&board);
    printf("%-7s %10d NAND2 evaluated per cycle\n", arch_names[arch], nl_nands() - before);
}
#else
static void measure(int arch, long cycles) {
    board_init(&board);
    board_pipe_init(&board);
    load_loop(&board);
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);
    for (long c = 0; c < cycles; c++) steps[arch](&board);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double s = (double)(t1.tv_sec - t0.tv_sec) + (double)(t1.tv_nsec - t0.tv_nsec) / 1e9;
    printf("%-7s %8ld cycles in %6.2f s  %10.0f cycles/s\n", arch_names[arch], cycles, s, (double)cycles / s);
}
#endif

int main(int argc, char **argv) {
    long cycles = argc > 1 ? strtol(argv[1], NULL, 10) : 5000;
    for (int arch = 0; arch < 3; arch++) measure(arch, cycles);
    return 0;
}
