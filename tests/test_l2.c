#include "test.h"

#include "../memory/cache/l2_cache.h"

static uint32_t make_addr(uint32_t index, uint32_t tag) {
    return (tag << 3) | (index & 7u);
}

static void fill(l2_cache_t *c, uint32_t addr, uint32_t data) {
    bit a[32], d[32];
    to_bits_32(addr, a);
    to_bits_32(data, d);
    l2_set_addr(c, a);
    l2_set_data(c, d);
    l2_set_write(c, 1);
    l2_clock_fall(c);
    l2_clock_rise(c);
    l2_set_write(c, 0);
}

static int read(l2_cache_t *c, uint32_t addr, uint32_t *data) {
    bit a[32];
    to_bits_32(addr, a);
    l2_set_addr(c, a);
    l2_set_write(c, 0);
    l2_propagate(c);
    *data = from_bits_32(c->data_out);
    return c->hit;
}

static void test_l2_all_lines(void) {
    test_suite("l2: 8 lines fill/hit");

    l2_cache_t c;
    l2_init(&c);
    l2_reset(&c);

    int i;
    uint32_t d;
    for (i = 0; i < 8; i++) {
        CHECK_EQ_BIT(read(&c, make_addr((uint32_t)i, 0x10), &d), 0, "cold miss");
    }
    for (i = 0; i < 8; i++) fill(&c, make_addr((uint32_t)i, 0x10), 0x1000u + (uint32_t)i);
    for (i = 0; i < 8; i++) {
        CHECK(read(&c, make_addr((uint32_t)i, 0x10), &d) && d == 0x1000u + (uint32_t)i,
              "line hits with correct data");
    }
}

static void test_l2_eviction(void) {
    test_suite("l2: eviction");

    l2_cache_t c;
    l2_init(&c);
    l2_reset(&c);

    fill(&c, make_addr(5, 0xAA), 0x11111111u);
    uint32_t d;
    CHECK(read(&c, make_addr(5, 0xAA), &d) && d == 0x11111111u, "cached");
    fill(&c, make_addr(5, 0xBB), 0x22222222u);
    CHECK_EQ_BIT(read(&c, make_addr(5, 0xAA), &d), 0, "old tag evicted");
    CHECK(read(&c, make_addr(5, 0xBB), &d) && d == 0x22222222u, "new tag hits");
}

int main(void) {
    test_l2_all_lines();
    test_l2_eviction();
    return test_report();
}
