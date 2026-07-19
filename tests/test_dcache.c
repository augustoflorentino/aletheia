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

static void ram_poke(board_t *b, uint32_t addr, uint32_t val) {
    bit a[8], d[32];
    to_bits_n(addr, a, 8);
    to_bits_32(val, d);
    sram256_set_addr(&b->ram, a);
    sram256_set_data(&b->ram, d);
    sram256_set_write(&b->ram, 1);
    sram256_set_cs(&b->ram, 1);
    sram256_clock_fall(&b->ram);
    sram256_clock_rise(&b->ram);
    sram256_set_write(&b->ram, 0);
}

static void dma_prog(board_t *b, uint32_t r, uint32_t value) {
    bit a[3], d[32];
    to_bits_n(r, a, 3);
    to_bits_32(value, d);
    dma_set_addr(&b->dma, a);
    dma_set_cpu_data(&b->dma, d);
    dma_set_cs(&b->dma, 1);
    dma_set_write(&b->dma, 1);
    dma_clock_fall(&b->dma);
    dma_clock_rise(&b->dma);
    dma_set_write(&b->dma, 0);
    dma_set_cs(&b->dma, 0);
}

static void test_hit_serves_data(void) {
    test_suite("dcache: hit serves data from the cache, not from SRAM");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 5),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 3),
        encode_i_type(OP_LW, REG_ZERO, REG_T1, 3),
        encode_i_type(OP_LW, REG_ZERO, REG_T3, 4),
        encode_i_type(OP_LW, REG_ZERO, REG_T4, 4),
        encode_i_type(OP_LW, REG_ZERO, REG_T5, 3),
        0xFC000000u,
    };
    load_at(&b, 0, prog, 7);

    ram_poke(&b, 4, 0x55u);
    ram_poke(&b, 10, 0x77u);

    board_step(&b);
    board_step(&b);
    ram_poke(&b, 3, 0xDEAD0001u);
    board_step(&b);
    CHECK_EQ_U32(reg(&b, REG_T1), 5u, "hit after store: came from L1, not from the poisoned SRAM");

    board_step(&b);
    CHECK_EQ_U32(reg(&b, REG_T3), 0x55u, "miss fills from SRAM");
    ram_poke(&b, 4, 0xBEEF0002u);
    board_step(&b);
    CHECK_EQ_U32(reg(&b, REG_T4), 0x55u, "hit after fill: came from L1");

    dma_prog(&b, 0, 10);
    dma_prog(&b, 1, 3);
    dma_prog(&b, 2, 1);
    dma_prog(&b, 3, 1);
    dma_set_dreq(&b.dma, 1);
    int c;
    for (c = 0; c < 6; c++) board_dma_step(&b);
    dma_set_dreq(&b.dma, 0);

    ram_poke(&b, 3, 0xBAD00003u);
    board_step(&b);
    CHECK_EQ_U32(reg(&b, REG_T5), 0x77u, "DMA snoop updated the L1 line");
}

static void test_io_not_cached(void) {
    test_suite("dcache: I/O space does not go through the cache");

    board_t b;
    board_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_LW, REG_ZERO, REG_T0, 0x100),
        encode_i_type(OP_LW, REG_ZERO, REG_T1, 0x100),
        0xFC000000u,
    };
    load_at(&b, 0, prog, 3);

    int c;
    for (c = 0; c < 5; c++) board_step(&b);

    CHECK_EQ_U32(reg(&b, REG_T0), 1u, "UART status read directly (ready)");
    CHECK_EQ_U32(reg(&b, REG_T1), 1u, "second read also direct");
    CHECK_EQ_BIT(b.l1.lines[0].valid.q, 0, "L1 line 0 was not filled by I/O");
}

int main(void) {
    test_hit_serves_data();
    test_io_not_cached();
    return test_report();
}
