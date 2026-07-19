#include "test.h"

#include "../memory/cache/replacement.h"

static uint32_t victim(fifo_replacement_t *f) {
    fifo_propagate(f);
    return from_bits_n(f->victim, 2);
}

static void advance(fifo_replacement_t *f) {
    fifo_set_advance(f, 1);
    fifo_clock_fall(f);
    fifo_clock_rise(f);
    fifo_set_advance(f, 0);
}

static void test_round_robin(void) {
    test_suite("replacement: FIFO round-robin 4 ways");

    fifo_replacement_t f;
    fifo_init(&f);
    fifo_reset(&f);

    CHECK_EQ_U32(victim(&f), 0u, "initial victim 0");
    advance(&f);
    CHECK_EQ_U32(victim(&f), 1u, "after 1 advance -> 1");
    advance(&f);
    CHECK_EQ_U32(victim(&f), 2u, "after 2 -> 2");
    advance(&f);
    CHECK_EQ_U32(victim(&f), 3u, "after 3 -> 3");
    advance(&f);
    CHECK_EQ_U32(victim(&f), 0u, "wraps to 0");
    advance(&f);
    CHECK_EQ_U32(victim(&f), 1u, "continues to 1");
}

static void test_hold(void) {
    test_suite("replacement: holds when not advancing");

    fifo_replacement_t f;
    fifo_init(&f);
    fifo_reset(&f);

    advance(&f);
    CHECK_EQ_U32(victim(&f), 1u, "at 1");

    fifo_set_advance(&f, 0);
    fifo_clock_fall(&f);
    fifo_clock_rise(&f);
    CHECK_EQ_U32(victim(&f), 1u, "holds at 1 without advance");
}

int main(void) {
    test_round_robin();
    test_hold();
    return test_report();
}
