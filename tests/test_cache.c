#include "test.h"

#include "../memory/cache/l1_cache.h"

static uint32_t make_addr(uint32_t index, uint32_t tag) {
    return (tag << 2) | (index & 3u);
}

static void cache_fill(l1_cache_t *c, uint32_t addr, uint32_t data) {
    bit a[32], d[32];
    to_bits_32(addr, a);
    to_bits_32(data, d);
    l1_set_addr(c, a);
    l1_set_data(c, d);
    l1_set_write(c, 1);
    l1_clock_fall(c);
    l1_clock_rise(c);
    l1_set_write(c, 0);
}

static int cache_read(l1_cache_t *c, uint32_t addr, uint32_t *data) {
    bit a[32];
    to_bits_32(addr, a);
    l1_set_addr(c, a);
    l1_set_write(c, 0);
    l1_propagate(c);
    *data = from_bits_32(c->data_out);
    return c->hit;
}

static void test_cold_miss(void) {
    test_suite("cache: cold cache misses");

    l1_cache_t c;
    l1_init(&c);
    l1_reset(&c);

    uint32_t d;
    CHECK_EQ_BIT(cache_read(&c, make_addr(0, 0x100), &d), 0, "cold read index 0 miss");
    CHECK_EQ_BIT(cache_read(&c, make_addr(3, 0x055), &d), 0, "cold read index 3 miss");
}

static void test_fill_hit(void) {
    test_suite("cache: fill then hit");

    l1_cache_t c;
    l1_init(&c);
    l1_reset(&c);

    cache_fill(&c, make_addr(0, 0x100), 0xAAAA5555u);
    uint32_t d;
    CHECK_EQ_BIT(cache_read(&c, make_addr(0, 0x100), &d), 1, "hit after fill");
    CHECK_EQ_U32(d, 0xAAAA5555u, "data correct on hit");

    CHECK_EQ_BIT(cache_read(&c, make_addr(0, 0x200), &d), 0, "same index diff tag -> miss");
    CHECK_EQ_BIT(cache_read(&c, make_addr(1, 0x100), &d), 0, "diff index unfilled -> miss");
}

static void test_all_lines(void) {
    test_suite("cache: all 4 lines independent");

    l1_cache_t c;
    l1_init(&c);
    l1_reset(&c);

    cache_fill(&c, make_addr(0, 0x11), 0x1000u);
    cache_fill(&c, make_addr(1, 0x22), 0x2000u);
    cache_fill(&c, make_addr(2, 0x33), 0x3000u);
    cache_fill(&c, make_addr(3, 0x44), 0x4000u);

    uint32_t d;
    CHECK(cache_read(&c, make_addr(0, 0x11), &d) && d == 0x1000u, "line 0 hit");
    CHECK(cache_read(&c, make_addr(1, 0x22), &d) && d == 0x2000u, "line 1 hit");
    CHECK(cache_read(&c, make_addr(2, 0x33), &d) && d == 0x3000u, "line 2 hit");
    CHECK(cache_read(&c, make_addr(3, 0x44), &d) && d == 0x4000u, "line 3 hit");
}

static void test_eviction(void) {
    test_suite("cache: eviction (direct-mapped conflict)");

    l1_cache_t c;
    l1_init(&c);
    l1_reset(&c);

    cache_fill(&c, make_addr(2, 0xABC), 0xDEAD0000u);
    uint32_t d;
    CHECK(cache_read(&c, make_addr(2, 0xABC), &d) && d == 0xDEAD0000u, "original tag hits");

    cache_fill(&c, make_addr(2, 0xDEF), 0xBEEF1111u);
    CHECK_EQ_BIT(cache_read(&c, make_addr(2, 0xABC), &d), 0, "evicted tag misses");
    CHECK(cache_read(&c, make_addr(2, 0xDEF), &d) && d == 0xBEEF1111u, "new tag hits");
}

int main(void) {
    test_cold_miss();
    test_fill_hit();
    test_all_lines();
    test_eviction();
    return test_report();
}
