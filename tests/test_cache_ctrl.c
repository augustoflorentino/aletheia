#include "test.h"

#include "../memory/cache/cache_controller.h"

static void mem_write(cache_controller_t *cc, uint32_t addr, uint32_t data) {
    bit a[32], d[32];
    to_bits_32(addr, a);
    to_bits_32(data, d);
    cc_mem_write(cc, a, d);
}

static int access(cache_controller_t *cc, uint32_t addr, uint32_t *data) {
    bit a[32];
    to_bits_32(addr, a);
    cc_access(cc, a);
    *data = from_bits_32(cc->data_out);
    return cc->hit_out;
}

static void test_read_through(void) {
    test_suite("cache_ctrl: read-through miss then hit");

    cache_controller_t cc;
    cc_init(&cc);
    cc_reset(&cc);

    mem_write(&cc, 5, 0xAAAA0000u);
    mem_write(&cc, 6, 0xBBBB1111u);

    uint32_t d;
    CHECK_EQ_BIT(access(&cc, 5, &d), 0, "first access to 5 is a miss");
    CHECK_EQ_U32(d, 0xAAAA0000u, "miss returns memory data");

    CHECK_EQ_BIT(access(&cc, 5, &d), 1, "second access to 5 is a hit");
    CHECK_EQ_U32(d, 0xAAAA0000u, "hit returns cached data");

    CHECK_EQ_BIT(access(&cc, 6, &d), 0, "first access to 6 is a miss");
    CHECK_EQ_U32(d, 0xBBBB1111u, "6 data correct");
    CHECK_EQ_BIT(access(&cc, 6, &d), 1, "second access to 6 hits");
}

static void test_conflict_eviction(void) {
    test_suite("cache_ctrl: conflict eviction via memory");

    cache_controller_t cc;
    cc_init(&cc);
    cc_reset(&cc);

    mem_write(&cc, 5, 0x50505050u);
    mem_write(&cc, 9, 0x90909090u);

    uint32_t d;
    access(&cc, 5, &d);
    CHECK_EQ_BIT(access(&cc, 5, &d), 1, "5 cached (hit)");

    CHECK_EQ_BIT(access(&cc, 9, &d), 0, "9 conflicts -> miss, evicts 5");
    CHECK_EQ_U32(d, 0x90909090u, "9 data from memory");

    CHECK_EQ_BIT(access(&cc, 5, &d), 0, "5 was evicted -> miss again");
    CHECK_EQ_U32(d, 0x50505050u, "5 refetched from memory correctly");
}

int main(void) {
    test_read_through();
    test_conflict_eviction();
    return test_report();
}
