#include "test.h"

#include "../memory/dram.h"

static void dwrite(dram_16x32_t *d, uint32_t addr, uint32_t data) {
    bit a[4], v[32];
    to_bits_n(addr, a, 4);
    to_bits_32(data, v);
    dram_set_addr(d, a);
    dram_set_data(d, v);
    dram_set_write(d, 1);
    dram_set_refresh(d, 0);
    dram_set_leak(d, 0);
    dram_clock_fall(d);
    dram_clock_rise(d);
    dram_set_write(d, 0);
}

static void drefresh(dram_16x32_t *d, uint32_t addr) {
    bit a[4];
    to_bits_n(addr, a, 4);
    dram_set_addr(d, a);
    dram_set_write(d, 0);
    dram_set_refresh(d, 1);
    dram_set_leak(d, 0);
    dram_clock_fall(d);
    dram_clock_rise(d);
    dram_set_refresh(d, 0);
}

static void dleak(dram_16x32_t *d) {
    dram_set_write(d, 0);
    dram_set_refresh(d, 0);
    dram_set_leak(d, 1);
    dram_clock_fall(d);
    dram_clock_rise(d);
    dram_set_leak(d, 0);
}

static uint32_t dread(dram_16x32_t *d, uint32_t addr) {
    bit a[4];
    to_bits_n(addr, a, 4);
    dram_set_addr(d, a);
    dram_propagate(d);
    return from_bits_32(d->data_out);
}

static void test_read_write(void) {
    test_suite("dram: basic read/write");

    dram_16x32_t d;
    dram_init(&d);
    dram_reset(&d);

    dwrite(&d, 5, 0xCAFEBABEu);
    CHECK_EQ_U32(dread(&d, 5), 0xCAFEBABEu, "read after write");
    CHECK_EQ_U32(dread(&d, 6), 0u, "unwritten cell reads 0");
}

static void test_retention_one_leak(void) {
    test_suite("dram: survives one leak (refreshed in period)");

    dram_16x32_t d;
    dram_init(&d);
    dram_reset(&d);

    dwrite(&d, 3, 0x11223344u);
    dleak(&d);
    CHECK_EQ_U32(dread(&d, 3), 0x11223344u, "data survives first leak");
}

static void test_loss_without_refresh(void) {
    test_suite("dram: lost without refresh (two leaks)");

    dram_16x32_t d;
    dram_init(&d);
    dram_reset(&d);

    dwrite(&d, 7, 0xDEADBEEFu);
    dleak(&d);
    dleak(&d);
    CHECK_EQ_U32(dread(&d, 7), 0u, "data lost after 2 leaks w/o refresh");
}

static void test_refresh_retains(void) {
    test_suite("dram: refresh retains across many leaks");

    dram_16x32_t d;
    dram_init(&d);
    dram_reset(&d);

    dwrite(&d, 9, 0x55AA55AAu);
    int i;
    for (i = 0; i < 8; i++) {
        drefresh(&d, 9);
        dleak(&d);
    }
    CHECK_EQ_U32(dread(&d, 9), 0x55AA55AAu, "data retained with periodic refresh");
}

static void test_selective_refresh(void) {
    test_suite("dram: only refreshed row survives");

    dram_16x32_t d;
    dram_init(&d);
    dram_reset(&d);

    dwrite(&d, 2, 0xAAAA0000u);
    dwrite(&d, 4, 0xBBBB1111u);
    drefresh(&d, 4);
    dleak(&d);
    CHECK_EQ_U32(dread(&d, 4), 0xBBBB1111u, "refreshed row 4 survives");
    CHECK_EQ_U32(dread(&d, 2), 0xAAAA0000u, "row 2 written same period also survives");

    dleak(&d);
    CHECK_EQ_U32(dread(&d, 4), 0u, "row 4 lost after 2nd leak w/o refresh");
}

int main(void) {
    test_read_write();
    test_retention_one_leak();
    test_loss_without_refresh();
    test_refresh_retains();
    test_selective_refresh();
    return test_report();
}
