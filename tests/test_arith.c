#include "test.h"

#include "../cpu/alu/subtractor.h"
#include "../cpu/alu/comparator.h"
#include "../cpu/alu/shifter.h"
#include "../cpu/alu/multiplier.h"
#include "../cpu/alu/divider.h"

static void test_subtractor(void) {
    test_suite("arith: subtractor / inc / dec / negate");

    rng_seed(0xBEEF01u);
    int k;
    for (k = 0; k < 3000; k++) {
        uint32_t a = rng_next();
        uint32_t b = rng_next();
        bit ba[32], bb[32], r[32];
        to_bits_32(a, ba);
        to_bits_32(b, bb);

        sub_32(ba, bb, r);
        CHECK_EQ_U32(from_bits_32(r), a - b, "sub_32");

        inc_32(ba, r);
        CHECK_EQ_U32(from_bits_32(r), a + 1u, "inc_32");

        dec_32(ba, r);
        CHECK_EQ_U32(from_bits_32(r), a - 1u, "dec_32");

        negate_32(ba, r);
        CHECK_EQ_U32(from_bits_32(r), (uint32_t)(-(int32_t)a), "negate_32");
    }
}

static void test_comparator(void) {
    test_suite("arith: comparator (unsigned)");

    rng_seed(0xC0FFEEu);
    int k;
    for (k = 0; k < 4000; k++) {
        uint32_t a = rng_next();
        uint32_t b = rng_next();
        if ((k % 7) == 0) b = a;
        bit ba[32], bb[32], eq, lt, gt;
        to_bits_32(a, ba);
        to_bits_32(b, bb);
        cmp_32(ba, bb, &eq, &lt, &gt);
        CHECK_EQ_BIT(eq, a == b, "cmp eq");
        CHECK_EQ_BIT(lt, a < b, "cmp lt");
        CHECK_EQ_BIT(gt, a > b, "cmp gt");
    }

    bit z[32];
    to_bits_32(0u, z);
    CHECK_EQ_BIT(is_zero_32(z), 1, "is_zero_32(0)");
    to_bits_32(0x00010000u, z);
    CHECK_EQ_BIT(is_zero_32(z), 0, "is_zero_32(nonzero)");
}

static void test_shifter(void) {
    test_suite("arith: shifter (logical)");

    rng_seed(0x5417u);
    int k;
    for (k = 0; k < 3000; k++) {
        uint32_t a = rng_next();
        uint32_t n = rng_next() & 31u;
        bit ba[32], bn[32], r[32];
        to_bits_32(a, ba);
        to_bits_32(n, bn);

        shl_32(ba, bn, r);
        CHECK_EQ_U32(from_bits_32(r), a << (n & 31u), "shl_32");

        shr_32(ba, bn, r);
        CHECK_EQ_U32(from_bits_32(r), a >> (n & 31u), "shr_32");
    }
}

static void test_multiplier(void) {
    test_suite("arith: multiplier (16x16)");

    rng_seed(0x1234u);
    int k;
    for (k = 0; k < 3000; k++) {
        uint32_t a = rng_next() & 0xFFFFu;
        uint32_t b = rng_next() & 0xFFFFu;
        bit ba[32], bb[32], r[32];
        to_bits_32(a, ba);
        to_bits_32(b, bb);
        multiply_simple(ba, bb, r);
        CHECK_EQ_U32(from_bits_32(r), a * b, "multiply_simple");
    }
}

static void test_divider(void) {
    test_suite("arith: divider / mod");

    rng_seed(0x9911u);
    int k;
    for (k = 0; k < 3000; k++) {
        uint32_t a = rng_next();
        uint32_t b = rng_next();
        if (b == 0) b = 1;
        bit ba[32], bb[32], q[32], rem[32];
        to_bits_32(a, ba);
        to_bits_32(b, bb);
        div_32(ba, bb, q, rem);
        CHECK_EQ_U32(from_bits_32(q), a / b, "div_32 quotient");
        CHECK_EQ_U32(from_bits_32(rem), a % b, "div_32 remainder");
    }
}

int main(void) {
    test_subtractor();
    test_comparator();
    test_shifter();
    test_multiplier();
    test_divider();
    return test_report();
}
