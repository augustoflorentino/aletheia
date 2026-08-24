#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/types.h"
#include "simulation/simulation.h"
#include "isa/isa.h"

static void to_bits_32(unsigned int n, bit *bits) {
    bits[0] = (n >> 0) & 1;
    bits[1] = (n >> 1) & 1;
    bits[2] = (n >> 2) & 1;
    bits[3] = (n >> 3) & 1;
    bits[4] = (n >> 4) & 1;
    bits[5] = (n >> 5) & 1;
    bits[6] = (n >> 6) & 1;
    bits[7] = (n >> 7) & 1;
    bits[8] = (n >> 8) & 1;
    bits[9] = (n >> 9) & 1;
    bits[10] = (n >> 10) & 1;
    bits[11] = (n >> 11) & 1;
    bits[12] = (n >> 12) & 1;
    bits[13] = (n >> 13) & 1;
    bits[14] = (n >> 14) & 1;
    bits[15] = (n >> 15) & 1;
    bits[16] = (n >> 16) & 1;
    bits[17] = (n >> 17) & 1;
    bits[18] = (n >> 18) & 1;
    bits[19] = (n >> 19) & 1;
    bits[20] = (n >> 20) & 1;
    bits[21] = (n >> 21) & 1;
    bits[22] = (n >> 22) & 1;
    bits[23] = (n >> 23) & 1;
    bits[24] = (n >> 24) & 1;
    bits[25] = (n >> 25) & 1;
    bits[26] = (n >> 26) & 1;
    bits[27] = (n >> 27) & 1;
    bits[28] = (n >> 28) & 1;
    bits[29] = (n >> 29) & 1;
    bits[30] = (n >> 30) & 1;
    bits[31] = (n >> 31) & 1;
}

static unsigned int from_bits_32(bit *bits) {
    unsigned int r = 0;
    for (int i = 0; i < 32; i++) {
        r |= ((unsigned int)(bits[i] & 1)) << i;
    }
    return r;
}

static void dump_state(simulator_t *sim) {
    static const int regs[] = {8, 9, 10, 11, 12, 13};
    static const char *names[] = {"$t0", "$t1", "$t2", "$t3", "$t4", "$t5"};

    regfile_propagate(&sim->board.regs);
    printf("\nRegisters:\n");
    for (int i = 0; i < 6; i++) {
        unsigned int v = from_bits_32(sim->board.regs.regs[regs[i]].data_out);
        printf("  %s = %u (0x%08x)\n", names[i], v, v);
    }

    printf("Display:\n  [");
    for (int i = 0; i < 8; i++) {
        bit a[3];
        for (int b = 0; b < 3; b++) a[b] = (i >> b) & 1;
        display_set_addr(&sim->board.disp, a);
        display_propagate(&sim->board.disp);
        unsigned int ch = 0;
        for (int b = 0; b < 8; b++) {
            ch |= ((unsigned int)(sim->board.disp.data_out[b] & 1)) << b;
        }
        putchar(ch >= 32 && ch < 127 ? (int)ch : ' ');
    }
    printf("]\n");

    printf("Memory (RAM):\n");
    for (int i = 0; i < 8; i++) {
        bit addr[8], out[32];
        for (int b = 0; b < 8; b++) addr[b] = (i >> b) & 1;
        sram256_set_addr(&sim->board.ram, addr);
        sram256_set_cs(&sim->board.ram, 1);
        sram256_propagate(&sim->board.ram);
        sram256_read(&sim->board.ram, out);
        unsigned int v = from_bits_32(out);
        printf("  ram[%d] = %u (0x%08x)\n", i, v, v);
    }
}

static void print_usage(const char *prog) {
    printf("Aletheia - a gate-level hardware simulator\n\n");
    printf("Usage: %s [options]\n\n", prog);
    printf("Options:\n");
    printf("  -h, --help          Show this help\n");
    printf("  -c, --cycles N      Run N cycles (default: 100)\n");
    printf("  -v, --verbose       Verbose mode\n");
    printf("  -w, --vcd FILE      Write a VCD waveform file\n");
    printf("  -a, --asm FILE      Load an assembly program\n");
    printf("  -d, --disasm WORD   Disassemble one instruction (hex)\n");
    printf("\n");
}

static void run_demo_program(simulator_t *sim) {
    bit program[16][32];

    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 32; j++) {
            program[i][j] = 0;
        }
    }

    unsigned int instructions[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 5),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 10),
        encode_r_type(REG_T0, REG_T1, REG_T2, 0, FUNCT_ADD),
        encode_i_type(OP_SW, REG_ZERO, REG_T2, 0),
        0};

    printf("Demo program:\n");
    for (int i = 0; i < 5; i++) {
        char disasm[MAX_DISASM_LEN];
        disassemble_instruction(instructions[i], disasm, MAX_DISASM_LEN);
        printf("  [%d] 0x%08x  %s\n", i, instructions[i], disasm);
        to_bits_32(instructions[i], program[i]);
    }
    printf("\n");

    simulator_load_rom(sim, program, 5);
}

int main(int argc, char **argv) {
    simulator_t sim;
    int max_cycles = 100;
    int verbose = 0;
    const char *vcd_file = NULL;
    const char *asm_file = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--cycles") == 0) {
            if (i + 1 < argc) {
                max_cycles = atoi(argv[++i]);
            }
            continue;
        }
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            verbose = 1;
            continue;
        }
        if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--vcd") == 0) {
            if (i + 1 < argc) {
                vcd_file = argv[++i];
            }
            continue;
        }
        if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--asm") == 0) {
            if (i + 1 < argc) {
                asm_file = argv[++i];
            }
            continue;
        }
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--disasm") == 0) {
            if (i + 1 < argc) {
                unsigned int word = (unsigned int)strtoul(argv[++i], NULL, 16);
                char disasm[MAX_DISASM_LEN];
                disassemble_instruction(word, disasm, MAX_DISASM_LEN);
                printf("0x%08x: %s\n", word, disasm);
                return 0;
            }
            continue;
        }
    }

    printf("===========================================\n");
    printf("  ALETHEIA - Gate-Level Hardware Simulator\n");
    printf("===========================================\n\n");

    simulator_init(&sim);
    simulator_set_verbose(&sim, verbose);
    simulator_enable_console(&sim);

    if (asm_file) {
        printf("Loading program: %s\n", asm_file);
        assembler_t as;
        assembler_init(&as);
        unsigned int program_words[256];
        int count = assembler_assemble_file(&as, asm_file, program_words, 256);
        if (count < 0) {
            fprintf(stderr, "%s\n", as.error);
            return 1;
        }
        printf("Instructions loaded: %d\n\n", count);

        bit program[256][32];
        for (int i = 0; i < count; i++) {
            to_bits_32(program_words[i], program[i]);
            char disasm[MAX_DISASM_LEN];
            disassemble_instruction(program_words[i], disasm, MAX_DISASM_LEN);
            printf("  [%d] 0x%08x  %s\n", i, program_words[i], disasm);
        }
        printf("\n");

        simulator_load_rom(&sim, program, count);
    } else {
        run_demo_program(&sim);
    }

    if (vcd_file) {
        printf("Writing waveform: %s\n", vcd_file);
        if (!simulator_open_vcd(&sim, vcd_file)) {
            printf("Failed to create VCD file\n");
            return 1;
        }
    }

    printf("Starting simulation (%d cycles)...\n\n", max_cycles);

    simulator_run(&sim, max_cycles);

    printf("Simulation finished.\n");
    printf("Cycles executed: %llu\n", simulator_get_cycles(&sim));

    dump_state(&sim);

    if (vcd_file) {
        simulator_close_vcd(&sim);
        printf("Waveform saved to: %s\n", vcd_file);
    }

    return 0;
}
