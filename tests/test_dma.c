#include "test.h"

#include "../system/board.h"

static void ram_store(board_t *b, uint32_t addr, uint32_t val) {
    bit a[32], d[32];
    to_bits_32(addr, a);
    to_bits_32(val, d);
    ram_set_addr(&b->ram, a);
    ram_set_data(&b->ram, d);
    ram_set_write(&b->ram, 1);
    ram_set_cs(&b->ram, 1);
    ram_clock_fall(&b->ram);
    ram_clock_rise(&b->ram);
    ram_set_write(&b->ram, 0);
}

static uint32_t ram_peek(board_t *b, uint32_t addr) {
    bit a[32], o[32];
    to_bits_32(addr, a);
    ram_set_addr(&b->ram, a);
    ram_set_write(&b->ram, 0);
    ram_set_cs(&b->ram, 1);
    ram_propagate(&b->ram);
    ram_read(&b->ram, o);
    return from_bits_32(o);
}

static void dma_prog(board_t *b, uint32_t reg, uint32_t value) {
    bit a[3], d[32];
    to_bits_n(reg, a, 3);
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

static void test_block_copy(void) {
    test_suite("dma: block copy src->dst with bus arbitration");

    board_t b;
    board_init(&b);

    ram_store(&b, 2, 0xAAAA0001u);
    ram_store(&b, 3, 0xBBBB0002u);
    ram_store(&b, 4, 0xCCCC0003u);
    ram_store(&b, 5, 0xDDDD0004u);

    dma_prog(&b, 0, 2);
    dma_prog(&b, 1, 8);
    dma_prog(&b, 2, 3);
    dma_prog(&b, 3, 1);
    dma_set_dreq(&b.dma, 1);

    int c;
    int saw_dack = 0;
    for (c = 0; c < 12; c++) {
        board_dma_step(&b);
        dma_propagate(&b.dma);
        if (dma_get_dack(&b.dma)) saw_dack = 1;
    }

    CHECK(saw_dack, "arbiter granted the bus to the DMA (dack seen)");
    CHECK_EQ_U32(ram_peek(&b, 8), 0xAAAA0001u, "ram[8] = ram[2] copied");
    CHECK_EQ_U32(ram_peek(&b, 9), 0xBBBB0002u, "ram[9] = ram[3] copied");
    CHECK_EQ_U32(ram_peek(&b, 10), 0xCCCC0003u, "ram[10] = ram[4] copied");
    CHECK_EQ_U32(ram_peek(&b, 11), 0u, "ram[11] untouched (count stopped at 3)");

    dma_propagate(&b.dma);
    CHECK_EQ_BIT(dma_get_tc(&b.dma), 1, "terminal count (tc) reached");
}

static void test_no_enable_no_transfer(void) {
    test_suite("dma: without enable it does not transfer");

    board_t b;
    board_init(&b);
    ram_store(&b, 2, 0x12345678u);

    dma_prog(&b, 0, 2);
    dma_prog(&b, 1, 8);
    dma_prog(&b, 2, 3);
    dma_set_dreq(&b.dma, 1);

    int c;
    for (c = 0; c < 12; c++) board_dma_step(&b);

    CHECK_EQ_U32(ram_peek(&b, 8), 0u, "ram[8] untouched (DMA disabled)");
}

static void test_dreq_drop_stops_transfer(void) {
    test_suite("dma: dropping DREQ stops the transfer on the next cycle");

    board_t b;
    board_init(&b);
    ram_store(&b, 2, 0x10u);
    ram_store(&b, 3, 0x20u);
    ram_store(&b, 4, 0x30u);

    dma_prog(&b, 0, 2);
    dma_prog(&b, 1, 8);
    dma_prog(&b, 2, 3);
    dma_prog(&b, 3, 1);
    dma_set_dreq(&b.dma, 1);

    int c;
    for (c = 0; c < 12 && ram_peek(&b, 8) == 0u; c++) board_dma_step(&b);
    dma_set_dreq(&b.dma, 0);
    for (c = 0; c < 6; c++) board_dma_step(&b);

    CHECK_EQ_U32(ram_peek(&b, 8), 0x10u, "first word copied");
    CHECK_EQ_U32(ram_peek(&b, 9), 0u, "no transfer after DREQ dropped");
}

int main(void) {
    test_block_copy();
    test_no_enable_no_transfer();
    test_dreq_drop_stops_transfer();
    return test_report();
}
