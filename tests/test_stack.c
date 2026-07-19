#include "test.h"

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
    for (i = 0; i < 16; i++) rom256_load_word(&b->rom, i, prog[i]);
}

static uint32_t reg(board_t *b, int i) {
    regfile_propagate(&b->regs);
    return from_bits_32(b->regs.regs[i].data_out);
}

static uint32_t ram(board_t *b, uint32_t addr) {
    bit a[8], out[32];
    to_bits_n(addr, a, 8);
    sram256_set_addr(&b->ram, a);
    sram256_set_write(&b->ram, 0);
    sram256_set_cs(&b->ram, 1);
    sram256_propagate(&b->ram);
    sram256_read(&b->ram, out);
    return from_bits_32(out);
}

static void test_nested_calls(void) {
    test_suite("stack: nested calls with $ra saved on the stack");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_SP, 15),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 5),
        encode_j_type(OP_JAL, 4),
        (uint32_t)OP_HALT << 26,
        encode_i_type(OP_ADDI, REG_SP, REG_SP, -1),
        encode_i_type(OP_SW, REG_SP, REG_RA, 0),
        encode_j_type(OP_JAL, 11),
        encode_i_type(OP_ADDI, REG_T1, REG_T1, 1),
        encode_i_type(OP_LW, REG_SP, REG_RA, 0),
        encode_i_type(OP_ADDI, REG_SP, REG_SP, 1),
        encode_r_type(REG_RA, 0, 0, 0, FUNCT_JR),
        encode_r_type(REG_T0, REG_T0, REG_T1, 0, FUNCT_ADD),
        encode_r_type(REG_RA, 0, 0, 0, FUNCT_JR),
    };
    load(&b, prog, 13);

    int c;
    for (c = 0; c < 24; c++) board_step(&b);

    CHECK_EQ_U32(reg(&b, REG_T0), 5u, "$t0 = 5 (argument)");
    CHECK_EQ_U32(reg(&b, REG_T1), 11u, "$t1 = 11 (inner doubled -> 10, outer +1)");
    CHECK_EQ_U32(reg(&b, REG_SP), 15u, "$sp restored to the top (pop done)");
    CHECK_EQ_U32(reg(&b, REG_RA), 3u, "$ra restored from the stack (outer return)");
    CHECK_EQ_U32(ram(&b, 14), 3u, "stack held the $ra of the outer call");
    CHECK_EQ_BIT(b.halt, 1, "returned to the top and stopped at HALT");
}

int main(void) {
    test_nested_calls();
    return test_report();
}
