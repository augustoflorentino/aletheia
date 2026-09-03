#include "test.h"
#include "../simulation/simulator.h"

#include "../system/board.h"
#include "../isa/isa.h"

#define OP_HALT 0x3F

static void load(board_t *b, uint32_t *words, int n) {
    bit prog[16][32];
    int i, j;
    for (i = 0; i < 16; i++) {
        uint32_t w = (i < n) ? words[i] : 0u;
        for (j = 0; j < 32; j++) prog[i][j] = (w >> j) & 1u;
    }
    for (i = 0; i < 16; i++) rom_load_at(&b->rom, (unsigned)(i), prog[i]);
}

static uint32_t reg(board_t *b, int i) {
    regfile_propagate(&b->regs);
    return from_bits_32(b->regs.regs[i].data_out);
}

static void test_halt_freezes(void) {
    test_suite("halt: HALT freezes the machine");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 5),
        (uint32_t)OP_HALT << 26,
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 99),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 42),
    };
    load(&b, prog, 4);

    int c;
    for (c = 0; c < 8; c++) board_step(&b);

    CHECK_EQ_U32(reg(&b, REG_T0), 5u, "$t0 = 5 (addi after HALT did not run)");
    CHECK_EQ_U32(reg(&b, REG_T1), 0u, "$t1 = 0 (instruction after HALT frozen)");
    CHECK_EQ_BIT(b.halt, 1, "halt flag set");
}

static void test_no_halt_runs(void) {
    test_suite("halt: without HALT all instructions run");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 5),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 7),
    };
    load(&b, prog, 2);

    int c;
    for (c = 0; c < 6; c++) board_step(&b);

    CHECK_EQ_U32(reg(&b, REG_T0), 5u, "$t0 = 5");
    CHECK_EQ_U32(reg(&b, REG_T1), 7u, "$t1 = 7");
    CHECK_EQ_BIT(b.halt, 0, "halt flag not set");
}

int main(void) {
    test_halt_freezes();
    test_no_halt_runs();
    return test_report();
}
