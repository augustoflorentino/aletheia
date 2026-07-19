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

static void test_call_return(void) {
    test_suite("jal/jr: subroutine call and return");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 5),
        encode_j_type(OP_JAL, 4),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T2, 99),
        (uint32_t)OP_HALT << 26,
        encode_r_type(REG_T0, REG_T0, REG_T1, 0, FUNCT_ADD),
        encode_r_type(REG_RA, 0, 0, 0, FUNCT_JR),
    };
    load(&b, prog, 6);

    int c;
    for (c = 0; c < 12; c++) board_step(&b);

    CHECK_EQ_U32(reg(&b, REG_T0), 5u, "$t0 = 5 (argument)");
    CHECK_EQ_U32(reg(&b, REG_T1), 10u, "$t1 = 10 (subroutine executed)");
    CHECK_EQ_U32(reg(&b, REG_RA), 2u, "$ra = 2 (JAL return address)");
    CHECK_EQ_U32(reg(&b, REG_T2), 99u, "$t2 = 99 (returned and continued after the JAL)");
    CHECK_EQ_BIT(b.halt, 1, "reached HALT");
}

static void test_two_calls(void) {
    test_suite("jal/jr: subroutine reused across two calls");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 3),
        encode_j_type(OP_JAL, 6),
        encode_r_type(REG_T1, REG_ZERO, REG_T2, 0, FUNCT_ADD),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 10),
        encode_j_type(OP_JAL, 6),
        (uint32_t)OP_HALT << 26,
        encode_r_type(REG_T0, REG_T0, REG_T1, 0, FUNCT_ADD),
        encode_r_type(REG_RA, 0, 0, 0, FUNCT_JR),
    };
    load(&b, prog, 8);

    int c;
    for (c = 0; c < 20; c++) board_step(&b);

    CHECK_EQ_U32(reg(&b, REG_T2), 6u, "$t2 = 6 (result of the 1st call)");
    CHECK_EQ_U32(reg(&b, REG_T0), 10u, "$t0 = 10 (2nd argument)");
    CHECK_EQ_U32(reg(&b, REG_T1), 20u, "$t1 = 20 (result of the 2nd call)");
    CHECK_EQ_U32(reg(&b, REG_RA), 5u, "$ra = 5 (return of the 2nd call)");
    CHECK_EQ_BIT(b.halt, 1, "reached HALT");
}

static void test_jr_does_not_write_reg(void) {
    test_suite("jal/jr: JR does not write a register");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 42),
        encode_r_type(REG_ZERO, REG_ZERO, REG_T0, 0, FUNCT_JR),
    };
    load(&b, prog, 2);

    board_step(&b);
    board_step(&b);

    CHECK_EQ_U32(reg(&b, REG_T0), 42u, "$t0 preserved (JR did not write)");
}

int main(void) {
    test_call_return();
    test_two_calls();
    test_jr_does_not_write_reg();
    return test_report();
}
