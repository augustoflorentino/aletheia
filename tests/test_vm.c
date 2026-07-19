#include "test.h"

#include "../system/board.h"
#include "../isa/isa.h"

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

static void run(board_t *b, int steps) {
    int c;
    for (c = 0; c < steps; c++) board_step(b);
}

static void test_translation_and_alias(void) {
    test_suite("vm: translation on, two VPNs aliased to the same frame");

    board_t b;
    board_init(&b);

    uint32_t prog[64];
    int n = 0;
    prog[n++] = encode_i_type(OP_SW, REG_ZERO, REG_ZERO, 0x135);
    prog[n++] = encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 1);
    prog[n++] = encode_i_type(OP_SW, REG_ZERO, REG_T0, 0x136);
    prog[n++] = encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 42);
    prog[n++] = encode_i_type(OP_SW, REG_ZERO, REG_T1, 7);
    prog[n++] = encode_i_type(OP_LW, REG_ZERO, REG_T2, 7);
    prog[n++] = encode_i_type(OP_ADDI, REG_ZERO, REG_K0, 1);
    int d;
    for (d = 0; d < 20; d++)
        prog[n++] = encode_r_type(REG_K0, REG_K0, REG_K0, 0, FUNCT_ADD);
    prog[n++] = encode_r_type(REG_K0, REG_K0, REG_K1, 0, FUNCT_ADD);
    prog[n++] = encode_r_type(REG_K1, REG_K0, REG_K1, 0, FUNCT_ADD);
    prog[n++] = encode_i_type(OP_SW, REG_ZERO, REG_K1, 0x135);
    prog[n++] = encode_i_type(OP_ADDI, REG_ZERO, REG_T3, 77);
    prog[n++] = encode_i_type(OP_SW, REG_ZERO, REG_T3, 0x3007);
    prog[n++] = encode_i_type(OP_LW, REG_ZERO, REG_T4, 0x3007);
    prog[n++] = encode_i_type(OP_LW, REG_ZERO, REG_T5, 7);
    prog[n++] = 0xFC000000u;
    load_at(&b, 0, prog, n);

    run(&b, n + 5);

    CHECK_EQ_BIT(b.halt, 1, "program finished");
    CHECK_EQ_U32(reg(&b, REG_T2), 42u, "translated lw reads the written value");
    CHECK_EQ_U32(reg(&b, REG_T4), 77u, "access via VPN 3 works");
    CHECK_EQ_U32(reg(&b, REG_T5), 77u, "VPN 0 and VPN 3 point to the same physical frame");
    CHECK_EQ_U32(ram(&b, 7), 77u, "physical frame 7 holds the last value");

    bit vaddr[32];
    to_bits_32(7u, vaddr);
    mmu_set_vaddr(&b.mmu, vaddr);
    mmu_propagate(&b.mmu);
    CHECK_EQ_BIT(b.mmu.tlb_hit, 1, "TLB was filled in hardware on the first access");
}

static void test_page_fault_handler_maps_and_retries(void) {
    test_suite("vm: page fault -> handler maps the page -> RETI re-executes");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_SW, REG_ZERO, REG_ZERO, 0x135),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 1),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 0x136),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 55),
        encode_i_type(OP_SW, REG_ZERO, REG_T1, 0x1004),
        encode_i_type(OP_LW, REG_ZERO, REG_T2, 0x1004),
        encode_i_type(OP_LW, REG_ZERO, REG_T3, 4),
        0xFC000000u,
    };
    uint32_t handler[32];
    int n = 0;
    handler[n++] = encode_i_type(OP_ADDI, REG_ZERO, REG_K0, 1);
    int d;
    for (d = 0; d < 20; d++)
        handler[n++] = encode_r_type(REG_K0, REG_K0, REG_K0, 0, FUNCT_ADD);
    handler[n++] = encode_i_type(OP_SW, REG_ZERO, REG_K0, 0x135);
    handler[n++] = encode_j_type(OP_RETI, 0);
    load_at(&b, 0, prog, 8);
    load_at(&b, 0x50, handler, n);

    run(&b, 8 + n + 10);

    CHECK_EQ_BIT(b.halt, 1, "program finished");
    CHECK_EQ_U32(reg(&b, REG_T2), 55u, "the missing sw was re-executed after the handler");
    CHECK_EQ_U32(reg(&b, REG_T3), 55u, "physical frame 4 received the write");
    CHECK_EQ_BIT(b.exc.in_exception.q, 0, "in_exception cleared after RETI");

    bit cbits[4];
    exc_get_cause(&b.exc, cbits);
    CHECK_EQ_U32(from_bits_n(cbits, 4), (uint32_t)EXC_ADDR_ERROR, "cause = EXC_ADDR_ERROR");
}

static void test_mmu_off_is_identity(void) {
    test_suite("vm: with MMU off nothing changes");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 9),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 12),
        encode_i_type(OP_LW, REG_ZERO, REG_T1, 12),
        0xFC000000u,
    };
    load_at(&b, 0, prog, 4);

    run(&b, 6);

    CHECK_EQ_U32(reg(&b, REG_T1), 9u, "direct physical access works");
    CHECK_EQ_BIT(b.exc.in_exception.q, 0, "no exception without MMU");
}

int main(void) {
    test_translation_and_alias();
    test_page_fault_handler_maps_and_retries();
    test_mmu_off_is_identity();
    return test_report();
}
