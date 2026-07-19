#include "test.h"

#include "../system/board.h"
#include "../isa/isa.h"

static void load(board_t *b, const uint32_t *words, int n) {
    bit w[32];
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < 32; j++) w[j] = (words[i] >> j) & 1u;
        rom256_load_word(&b->rom, i, w);
    }
}

static void load_at(board_t *b, int base, const uint32_t *words, int n) {
    bit w[32];
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < 32; j++) w[j] = (words[i] >> j) & 1u;
        rom256_load_word(&b->rom, base + i, w);
    }
}

static uint32_t reg(board_t *b, int i) {
    regfile_propagate(&b->regs);
    return from_bits_32(b->regs.regs[i].data_out);
}

static void test_timer_interrupt(void) {
    test_suite("irq e2e: timer -> handler -> RETI -> main program continues");

    board_t b;
    board_init(&b);

    uint32_t main_prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 1),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0x134),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 5),
        encode_i_type(OP_SW, REG_ZERO, REG_T1, 0x130),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T2, 1),
        encode_i_type(OP_SW, REG_ZERO, REG_T2, 0x132),
        encode_i_type(OP_ADDI, REG_T3, REG_T3, 1),
        encode_j_type(OP_J, 6),
    };
    uint32_t handler[] = {
        encode_i_type(OP_ADDI, REG_T4, REG_T4, 1),
        encode_i_type(OP_LW, REG_ZERO, REG_K0, 0x133),
        encode_i_type(OP_ADDI, REG_ZERO, REG_K1, 5),
        encode_i_type(OP_SW, REG_ZERO, REG_K1, 0x130),
        encode_j_type(OP_RETI, 0),
    };
    load(&b, main_prog, 8);
    load_at(&b, 0x70, handler, 5);

    int c;
    for (c = 0; c < 40; c++) board_step(&b);
    uint32_t t3_mid = reg(&b, REG_T3);
    uint32_t t4_mid = reg(&b, REG_T4);
    for (c = 0; c < 40; c++) board_step(&b);

    CHECK(t4_mid >= 1u, "handler ran at least 1x in 40 steps (got %u)", t4_mid);
    CHECK(reg(&b, REG_T4) > t4_mid, "interrupts continue after re-arming (got %u -> %u)",
          t4_mid, reg(&b, REG_T4));
    CHECK(reg(&b, REG_T3) > t3_mid, "main program continued after RETI (got %u -> %u)",
          t3_mid, reg(&b, REG_T3));

    for (c = 0; c < 20 && b.exc.in_exception.q; c++) board_step(&b);
    CHECK_EQ_BIT(b.exc.in_exception.q, 0, "in_exception cleared after RETI");
    CHECK_EQ_BIT(b.pic.ic.in_service[0].q, 0, "in_service cleared after RETI");

    pc_propagate(&b.pc);
    uint32_t pc_now = from_bits_32(b.pc.reg.data_out);
    CHECK(pc_now >= 6u && pc_now <= 7u, "PC returned to the main loop (got %u)", pc_now);
}

static void test_masked_irq_ignored(void) {
    test_suite("irq e2e: masked IRQ does not interrupt");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 3),
        encode_i_type(OP_SW, REG_ZERO, REG_T1, 0x130),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T2, 1),
        encode_i_type(OP_SW, REG_ZERO, REG_T2, 0x132),
        encode_i_type(OP_ADDI, REG_T3, REG_T3, 1),
        encode_j_type(OP_J, 4),
    };
    uint32_t handler[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T4, 99),
        encode_j_type(OP_RETI, 0),
    };
    load(&b, prog, 6);
    load_at(&b, 0x70, handler, 2);

    int c;
    for (c = 0; c < 30; c++) board_step(&b);

    CHECK_EQ_U32(reg(&b, REG_T4), 0u, "handler never ran (mask 0)");
    CHECK_EQ_BIT(b.exc.in_exception.q, 0, "no exception taken");
    CHECK_EQ_BIT(b.pic.ic.pending[0].q, 1, "IRQ stayed pending in the PIC");
}

static void test_epc_points_into_loop(void) {
    test_suite("irq e2e: EPC points to the interrupted instruction");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 1),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0x134),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 4),
        encode_i_type(OP_SW, REG_ZERO, REG_T1, 0x130),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T2, 1),
        encode_i_type(OP_SW, REG_ZERO, REG_T2, 0x132),
        encode_i_type(OP_ADDI, REG_T3, REG_T3, 1),
        encode_j_type(OP_J, 6),
    };
    uint32_t handler[] = {
        0xFC000000u,
    };
    load(&b, prog, 8);
    load_at(&b, 0x70, handler, 1);

    int c;
    for (c = 0; c < 30; c++) board_step(&b);

    CHECK_EQ_BIT(b.halt, 1, "handler with HALT froze the machine");
    uint32_t epc = from_bits_32(b.exc.return_addr);
    CHECK(epc >= 6u && epc <= 7u, "EPC inside the interrupted loop (got %u)", epc);
    uint32_t cause;
    bit cbits[4];
    exc_get_cause(&b.exc, cbits);
    cause = from_bits_n(cbits, 4);
    CHECK_EQ_U32(cause, (uint32_t)EXC_INTERRUPT, "cause = EXC_INTERRUPT");
}

int main(void) {
    test_timer_interrupt();
    test_masked_irq_ignored();
    test_epc_points_into_loop();
    return test_report();
}
