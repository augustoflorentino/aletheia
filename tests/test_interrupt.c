#include "test.h"

#include "../cpu/interrupt/interrupt_controller.h"
#include "../cpu/interrupt/exception_handler.h"

static int ref_lowest_active(const int *active) {
    int i;
    for (i = 0; i < NUM_INTERRUPTS; i++) {
        if (active[i]) return i;
    }
    return -1;
}

static void force_state(interrupt_controller_t *ic, uint32_t pending,
                        uint32_t mask, uint32_t in_service, int global_en) {
    int i;
    for (i = 0; i < NUM_INTERRUPTS; i++) {
        ic->pending[i].q = (pending >> i) & 1u;
        ic->mask[i].q = (mask >> i) & 1u;
        ic->in_service[i].q = (in_service >> i) & 1u;
    }
    ic->global_enable.q = (bit)global_en;
}

static void test_priority_encoder(void) {
    test_suite("interrupt: priority encoder (exhaustive active)");

    interrupt_controller_t ic;
    intc_init(&ic);

    uint32_t pat;
    for (pat = 0; pat < 256; pat++) {
        force_state(&ic, pat, 0xFFu, 0x00u, 1);
        intc_propagate(&ic);

        int active[NUM_INTERRUPTS];
        int i;
        for (i = 0; i < NUM_INTERRUPTS; i++) active[i] = (pat >> i) & 1u;
        int low = ref_lowest_active(active);

        bit vec[3];
        intc_get_vector(&ic, vec);
        uint32_t vnum = from_bits_n(vec, 3);

        CHECK_EQ_BIT(intc_has_pending(&ic), low >= 0, "has_pending");
        CHECK_EQ_BIT(intc_get_irq_out(&ic), low >= 0, "irq_out (ge=1)");
        if (low >= 0) {
            CHECK_EQ_U32(vnum, (uint32_t)low, "vector index");
        }
    }
}

static void test_mask_and_service_gating(void) {
    test_suite("interrupt: mask / in_service / global enable");

    interrupt_controller_t ic;
    intc_init(&ic);

    rng_seed(0x1C7Au);
    int k;
    for (k = 0; k < 4000; k++) {
        uint32_t pending = rng_next() & 0xFFu;
        uint32_t mask = rng_next() & 0xFFu;
        uint32_t in_service = rng_next() & 0xFFu;
        int ge = rng_next() & 1;

        force_state(&ic, pending, mask, in_service, ge);
        intc_propagate(&ic);

        int active[NUM_INTERRUPTS];
        int i;
        for (i = 0; i < NUM_INTERRUPTS; i++) {
            uint32_t same_or_higher = (2u << i) - 1u;
            active[i] = ((pending >> i) & 1u) && ((mask >> i) & 1u) && !(in_service & same_or_higher);
        }
        int low = ref_lowest_active(active);

        CHECK_EQ_BIT(intc_has_pending(&ic), low >= 0, "has_pending gated");
        CHECK_EQ_BIT(intc_get_irq_out(&ic), (low >= 0) && ge, "irq_out gated by ge");
        if (low >= 0) {
            bit vec[3];
            intc_get_vector(&ic, vec);
            CHECK_EQ_U32(from_bits_n(vec, 3), (uint32_t)low, "vector gated");
        }
    }
}

static void test_dynamic_ack(void) {
    test_suite("interrupt: raise / service / preempt / ack");

    interrupt_controller_t ic;
    intc_reset(&ic);

    bit mask[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    intc_set_mask(&ic, mask);
    intc_set_global_enable(&ic, 1);
    intc_clock_fall(&ic);
    intc_clock_rise(&ic);

    bit irq[8] = {0, 0, 0, 0, 0, 1, 0, 0};
    intc_set_irq(&ic, irq);
    intc_clock_fall(&ic);
    intc_clock_rise(&ic);
    intc_propagate(&ic);

    CHECK_EQ_BIT(intc_get_irq_out(&ic), 1, "irq_out asserted after raise");
    bit vec[3];
    intc_get_vector(&ic, vec);
    CHECK_EQ_U32(from_bits_n(vec, 3), 5u, "vector points to line 5");

    bit no_irq[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    bit num5[3], num2[3], none[3] = {0, 0, 0};
    to_bits_n(5u, num5, 3);
    to_bits_n(2u, num2, 3);
    intc_set_irq(&ic, no_irq);
    intc_set_service(&ic, 1, num5);
    intc_clock_fall(&ic);
    intc_clock_rise(&ic);
    intc_propagate(&ic);
    CHECK_EQ_BIT(intc_get_irq_out(&ic), 0, "irq_out cleared once in service");

    intc_set_service(&ic, 0, none);
    intc_set_irq(&ic, irq);
    intc_clock_fall(&ic);
    intc_clock_rise(&ic);
    intc_set_irq(&ic, no_irq);
    intc_clock_fall(&ic);
    intc_clock_rise(&ic);
    intc_propagate(&ic);
    CHECK_EQ_BIT(intc_get_irq_out(&ic), 0, "same line waits while in service");

    bit irq7[8] = {0, 0, 0, 0, 0, 0, 0, 1};
    intc_set_irq(&ic, irq7);
    intc_clock_fall(&ic);
    intc_clock_rise(&ic);
    intc_set_irq(&ic, no_irq);
    intc_propagate(&ic);
    CHECK_EQ_BIT(intc_get_irq_out(&ic), 0, "lower priority does not preempt");

    bit irq2[8] = {0, 0, 1, 0, 0, 0, 0, 0};
    intc_set_irq(&ic, irq2);
    intc_clock_fall(&ic);
    intc_clock_rise(&ic);
    intc_set_irq(&ic, no_irq);
    intc_propagate(&ic);
    CHECK_EQ_BIT(intc_get_irq_out(&ic), 1, "higher priority preempts");
    intc_get_vector(&ic, vec);
    CHECK_EQ_U32(from_bits_n(vec, 3), 2u, "preempting vector is line 2");
    intc_set_service(&ic, 1, num2);
    intc_clock_fall(&ic);
    intc_clock_rise(&ic);
    intc_set_service(&ic, 0, none);

    bit top[3], any;
    intc_get_top_service(&ic, top, &any);
    CHECK_EQ_U32(from_bits_n(top, 3), 2u, "ack targets highest priority in service");
    intc_set_ack(&ic, 1, top);
    intc_clock_fall(&ic);
    intc_clock_rise(&ic);
    intc_set_ack(&ic, 0, top);
    intc_propagate(&ic);
    intc_get_vector(&ic, vec);
    CHECK_EQ_BIT(intc_get_irq_out(&ic), 0, "line 5 still blocks line 7");

    intc_get_top_service(&ic, top, &any);
    CHECK_EQ_U32(from_bits_n(top, 3), 5u, "next ack targets line 5");
    intc_set_ack(&ic, 1, top);
    intc_clock_fall(&ic);
    intc_clock_rise(&ic);
    intc_set_ack(&ic, 0, top);
    intc_propagate(&ic);
    CHECK_EQ_BIT(intc_get_irq_out(&ic), 1, "pulse received during handler is served after ack");
    intc_get_vector(&ic, vec);
    CHECK_EQ_U32(from_bits_n(vec, 3), 5u, "pending second pulse on line 5 wins over line 7");
}

static void assert_handler(exception_handler_t *eh, uint32_t code) {
    exc_propagate(eh);
    CHECK_EQ_BIT(exc_get_exception_taken(eh), 1, "exception_taken");
    bit haddr[32];
    exc_get_handler_addr(eh, haddr);
    CHECK_EQ_U32(from_bits_32(haddr), code << 4, "handler_addr = code<<4");
}

static void test_exception_cause(void) {
    test_suite("exception: cause encoding / handler address");

    exception_handler_t eh;

    exc_init(&eh);
    exc_reset(&eh);
    exc_set_reset(&eh, 1);
    assert_handler(&eh, EXC_RESET);

    exc_init(&eh);
    exc_reset(&eh);
    exc_set_undefined(&eh, 1);
    assert_handler(&eh, EXC_UNDEFINED);

    exc_init(&eh);
    exc_reset(&eh);
    exc_set_syscall(&eh, 1);
    assert_handler(&eh, EXC_SYSCALL);

    exc_init(&eh);
    exc_reset(&eh);
    exc_set_break(&eh, 1);
    assert_handler(&eh, EXC_BREAK);

    exc_init(&eh);
    exc_reset(&eh);
    exc_set_overflow(&eh, 1);
    assert_handler(&eh, EXC_OVERFLOW);

    exc_init(&eh);
    exc_reset(&eh);
    exc_set_addr_error(&eh, 1);
    assert_handler(&eh, EXC_ADDR_ERROR);

    exc_init(&eh);
    exc_reset(&eh);
    exc_set_bus_error(&eh, 1);
    assert_handler(&eh, EXC_BUS_ERROR);

    bit vec0[3] = {0, 0, 0};
    exc_init(&eh);
    exc_reset(&eh);
    exc_set_interrupt(&eh, 1, vec0);
    assert_handler(&eh, EXC_INTERRUPT);
}

static void test_exception_priority(void) {
    test_suite("exception: priority between simultaneous sources");

    exception_handler_t eh;
    bit vec0[3] = {0, 0, 0};

    exc_init(&eh);
    exc_set_undefined(&eh, 1);
    exc_set_interrupt(&eh, 1, vec0);
    assert_handler(&eh, EXC_UNDEFINED);

    exc_init(&eh);
    exc_set_addr_error(&eh, 1);
    exc_set_bus_error(&eh, 1);
    assert_handler(&eh, EXC_ADDR_ERROR);

    exc_init(&eh);
    exc_set_syscall(&eh, 1);
    exc_set_overflow(&eh, 1);
    assert_handler(&eh, EXC_SYSCALL);
}

static void test_exception_nested_and_epc(void) {
    test_suite("exception: nesting / epc save");

    exception_handler_t eh;
    bit vec0[3] = {0, 0, 0};
    exc_init(&eh);
    exc_reset(&eh);

    eh.in_exception.q = 1;
    exc_set_interrupt(&eh, 1, vec0);
    exc_propagate(&eh);
    CHECK_EQ_BIT(exc_get_exception_taken(&eh), 0, "interrupt masked inside handler");

    exc_init(&eh);
    bit pc0[32], pc1[32], ret0[32];
    to_bits_32(0x40u, pc0);
    to_bits_32(0x75u, pc1);
    exc_set_pc(&eh, pc0);
    exc_set_addr_error(&eh, 1);
    exc_clock_fall(&eh);
    exc_clock_rise(&eh);
    exc_set_pc(&eh, pc1);
    exc_set_addr_error(&eh, 0);
    exc_set_undefined(&eh, 1);
    exc_propagate(&eh);
    CHECK_EQ_BIT(exc_get_exception_taken(&eh), 1, "synchronous exception taken inside handler");
    exc_clock_fall(&eh);
    exc_clock_rise(&eh);
    exc_get_return_addr(&eh, ret0);
    CHECK_EQ_U32(from_bits_32(ret0), 0x40u, "nested exception keeps original epc");
    bit c0[4];
    exc_get_cause(&eh, c0);
    CHECK_EQ_U32(from_bits_n(c0, 4), EXC_UNDEFINED, "nested exception updates cause");

    exc_init(&eh);
    exc_reset(&eh);
    bit pc[32];
    to_bits_32(0x00400020u, pc);
    exc_set_pc(&eh, pc);
    exc_set_syscall(&eh, 1);
    exc_clock_fall(&eh);
    exc_clock_rise(&eh);
    exc_propagate(&eh);

    bit ret[32];
    exc_get_return_addr(&eh, ret);
    CHECK_EQ_U32(from_bits_32(ret), 0x00400020u, "epc holds faulting pc");

    bit cause[4];
    exc_get_cause(&eh, cause);
    CHECK_EQ_U32(from_bits_n(cause, 4), EXC_SYSCALL, "cause latched = syscall");
}

int main(void) {
    test_priority_encoder();
    test_mask_and_service_gating();
    test_dynamic_ack();
    test_exception_cause();
    test_exception_priority();
    test_exception_nested_and_epc();
    return test_report();
}
