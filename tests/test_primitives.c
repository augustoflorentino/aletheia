#include "test.h"

#include "../core/sequential/sr_latch.h"
#include "../core/sequential/d_latch.h"
#include "../core/sequential/jk_flip_flop.h"
#include "../core/sequential/t_flip_flop.h"
#include "../core/clock/divider.h"
#include "../io/devices/timer.h"
#include "../io/port.h"

static void sr(sr_latch_t *l, bit s, bit r) {
    sr_latch_update(l, s, r);
    sr_latch_update(l, s, r);
}

static void test_sr_latch(void) {
    test_suite("prim: SR latch");
    sr_latch_t l;
    sr_latch_reset(&l);
    sr(&l, 1, 0);
    CHECK_EQ_BIT(l.q, 1, "set -> q=1");
    sr(&l, 0, 0);
    CHECK_EQ_BIT(l.q, 1, "hold -> q=1");
    sr(&l, 0, 1);
    CHECK_EQ_BIT(l.q, 0, "reset -> q=0");
    sr(&l, 0, 0);
    CHECK_EQ_BIT(l.q, 0, "hold -> q=0");
}

static void test_d_latch(void) {
    test_suite("prim: D latch (transparent)");
    d_latch_t l;
    d_latch_reset(&l);
    d_latch_update(&l, 1, 1);
    CHECK_EQ_BIT(l.q, 1, "enable=1 -> q=d=1");
    d_latch_update(&l, 0, 0);
    CHECK_EQ_BIT(l.q, 1, "enable=0 -> holds 1");
    d_latch_update(&l, 0, 1);
    CHECK_EQ_BIT(l.q, 0, "enable=1 -> q=d=0");
}

static void jk(jk_flip_flop_t *ff, bit j, bit k) {
    jkff_set_jk(ff, j, k);
    jkff_clock_fall(ff);
    jkff_clock_rise(ff);
}

static void test_jk(void) {
    test_suite("prim: JK flip-flop");
    jk_flip_flop_t ff;
    jkff_reset(&ff);
    jk(&ff, 1, 0);
    CHECK_EQ_BIT(ff.q, 1, "J=1,K=0 -> set");
    jk(&ff, 0, 0);
    CHECK_EQ_BIT(ff.q, 1, "J=0,K=0 -> hold");
    jk(&ff, 1, 1);
    CHECK_EQ_BIT(ff.q, 0, "J=1,K=1 -> toggle (1->0)");
    jk(&ff, 1, 1);
    CHECK_EQ_BIT(ff.q, 1, "J=1,K=1 -> toggle (0->1)");
    jk(&ff, 0, 1);
    CHECK_EQ_BIT(ff.q, 0, "J=0,K=1 -> reset");
}

static void tff(t_flip_flop_t *ff, bit t) {
    tff_set_t(ff, t);
    tff_clock_fall(ff);
    tff_clock_rise(ff);
}

static void test_tff(void) {
    test_suite("prim: T flip-flop");
    t_flip_flop_t ff;
    tff_reset(&ff);
    tff(&ff, 1);
    CHECK_EQ_BIT(ff.q, 1, "T=1 -> toggle 0->1");
    tff(&ff, 0);
    CHECK_EQ_BIT(ff.q, 1, "T=0 -> hold");
    tff(&ff, 1);
    CHECK_EQ_BIT(ff.q, 0, "T=1 -> toggle 1->0");
}

static void test_divider(void) {
    test_suite("prim: clock divider (ripple)");
    clock_divider_t d;
    divider_init(&d);

    int prev2 = -1, toggles2 = 0;
    int i;
    for (i = 0; i < 16; i++) {
        divider_clock_fall(&d);
        divider_clock_rise(&d);
        if (prev2 >= 0 && d.div2 != prev2) toggles2++;
        prev2 = d.div2;
        if (i == 3) CHECK_EQ_BIT(d.div16, 0, "div16=0 before the 8th clock");
        if (i == 7) CHECK_EQ_BIT(d.div16, 1, "div16 toggles on the 8th clock (/16)");
    }
    CHECK(toggles2 >= 7, "div2 toggles every clock (>=7 transitions in 16)");
    CHECK_EQ_BIT(d.div16, 0, "div16 returns to 0 after 16 clocks (full period)");
}

static void test_timer(void) {
    test_suite("prim: timer countdown -> zero");
    hw_timer_t t;
    timer_init(&t);

    bit three[16] = {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    timer_set_addr(&t, 0, 0);
    timer_set_data(&t, three);
    timer_set_cs(&t, 1);
    timer_set_write(&t, 1);
    timer_clock_fall(&t);
    timer_clock_rise(&t);

    bit en[16] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    timer_set_addr(&t, 0, 1);
    timer_set_data(&t, en);
    timer_clock_fall(&t);
    timer_clock_rise(&t);
    timer_set_write(&t, 0);
    timer_set_cs(&t, 0);

    int i;
    for (i = 0; i < 3; i++) {
        timer_clock_fall(&t);
        timer_clock_rise(&t);
    }
    timer_propagate(&t);
    CHECK_EQ_BIT(t.zero, 1, "counter reached zero after 3 cycles");
}

static void test_port(void) {
    test_suite("prim: io_port write/read");
    io_port_t p;
    port_init(&p);

    bit dir[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    port_set_addr(&p, 1, 0);
    port_set_data(&p, dir);
    port_set_cs(&p, 1);
    port_set_write(&p, 1);
    port_clock_fall(&p);
    port_clock_rise(&p);

    bit val[8] = {1, 0, 1, 0, 1, 0, 1, 0};
    port_set_addr(&p, 0, 0);
    port_set_data(&p, val);
    port_clock_fall(&p);
    port_clock_rise(&p);
    port_set_write(&p, 0);

    port_propagate(&p);
    bit out[8];
    port_read_external(&p, out);
    CHECK_EQ_U32(from_bits_n(out, 8), 0x55u, "external pins reflect 0x55");
}

int main(void) {
    test_sr_latch();
    test_d_latch();
    test_jk();
    test_tff();
    test_divider();
    test_timer();
    test_port();
    return test_report();
}
