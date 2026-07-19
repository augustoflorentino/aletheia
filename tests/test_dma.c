#include "test.h"

#include "../system/board.h"

static void ram_write(board_t *b, uint32_t addr, uint32_t val) {
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

static uint32_t ram_read(board_t *b, uint32_t addr) {
    bit a[8], o[32];
    to_bits_n(addr, a, 8);
    sram256_set_addr(&b->ram, a);
    sram256_set_write(&b->ram, 0);
    sram256_set_cs(&b->ram, 1);
    sram256_propagate(&b->ram);
    sram256_read(&b->ram, o);
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

    ram_write(&b, 2, 0xAAAA0001u);
    ram_write(&b, 3, 0xBBBB0002u);
    ram_write(&b, 4, 0xCCCC0003u);

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
    CHECK_EQ_U32(ram_read(&b, 8), 0xAAAA0001u, "ram[8] = ram[2] copied");
    CHECK_EQ_U32(ram_read(&b, 9), 0xBBBB0002u, "ram[9] = ram[3] copied");
    CHECK_EQ_U32(ram_read(&b, 10), 0xCCCC0003u, "ram[10] = ram[4] copied");
    CHECK_EQ_U32(ram_read(&b, 11), 0u, "ram[11] untouched (count stopped at 3)");

    dma_propagate(&b.dma);
    CHECK_EQ_BIT(dma_get_tc(&b.dma), 1, "terminal count (tc) reached");
}

static void test_no_enable_no_transfer(void) {
    test_suite("dma: without enable it does not transfer");

    board_t b;
    board_init(&b);
    ram_write(&b, 2, 0x12345678u);

    dma_prog(&b, 0, 2);
    dma_prog(&b, 1, 8);
    dma_prog(&b, 2, 3);
    dma_set_dreq(&b.dma, 1);

    int c;
    for (c = 0; c < 12; c++) board_dma_step(&b);

    CHECK_EQ_U32(ram_read(&b, 8), 0u, "ram[8] untouched (DMA disabled)");
}

int main(void) {
    test_block_copy();
    test_no_enable_no_transfer();
    return test_report();
}
