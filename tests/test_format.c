#include "test.h"

#include "../io/format/number_formatter.h"

static void ref_format(uint32_t v, uint32_t out[NF_DIGITS]) {
    int d[NF_DIGITS];
    int i, high = 0;
    uint32_t t = v;
    for (i = 0; i < NF_DIGITS; i++) {
        d[i] = t % 10u;
        t /= 10u;
        if (d[i] != 0) high = i;
    }
    for (i = 0; i < NF_DIGITS; i++) {
        out[i] = (i <= high) ? (uint32_t)('0' + d[i]) : (uint32_t)' ';
    }
}

static void check_value(uint32_t v) {
    bit in[32], ascii[NF_DIGITS][8];
    to_bits_32(v, in);
    number_format(in, ascii);

    uint32_t exp[NF_DIGITS];
    ref_format(v, exp);

    int i;
    for (i = 0; i < NF_DIGITS; i++) {
        char label[32];
        snprintf(label, sizeof(label), "v=%u pos %d", v, i);
        CHECK_EQ_U32(from_bits_n(ascii[i], 8), exp[i], label);
    }
}

static void test_format_directed(void) {
    test_suite("format: number -> ASCII with leading-zero suppression");

    uint32_t vals[] = {0u, 1u, 9u, 10u, 42u, 100u, 243u, 1000u,
                       65535u, 999999u, 1234567890u, 4294967295u};
    int n = (int)(sizeof(vals) / sizeof(vals[0]));
    int i;
    for (i = 0; i < n; i++) check_value(vals[i]);
}

static void test_format_fuzz(void) {
    test_suite("format: randomized fuzz");

    rng_seed(0xF07A7u);
    int k;
    for (k = 0; k < 1500; k++) check_value(rng_next());
}

static void ref_format_signed(uint32_t v, uint32_t out[NF_DIGITS]) {
    int32_t s = (int32_t)v;
    int neg = s < 0;
    uint32_t mag = neg ? (uint32_t)(-(int64_t)s) : v;

    int d[NF_DIGITS];
    int i, high = 0;
    uint32_t t = mag;
    for (i = 0; i < NF_DIGITS; i++) {
        d[i] = t % 10u;
        t /= 10u;
        if (d[i] != 0) high = i;
    }
    for (i = 0; i < NF_DIGITS; i++) {
        if (i <= high) {
            out[i] = (uint32_t)('0' + d[i]);
        } else if (neg && i == high + 1) {
            out[i] = (uint32_t)'-';
        } else {
            out[i] = (uint32_t)' ';
        }
    }
}

static void check_signed(uint32_t v) {
    bit in[32], ascii[NF_DIGITS][8];
    to_bits_32(v, in);
    number_format_signed(in, ascii);

    uint32_t exp[NF_DIGITS];
    ref_format_signed(v, exp);

    int i;
    for (i = 0; i < NF_DIGITS; i++) {
        char label[32];
        snprintf(label, sizeof(label), "signed v=%d pos %d", (int)v, i);
        CHECK_EQ_U32(from_bits_n(ascii[i], 8), exp[i], label);
    }
}

static void test_format_signed(void) {
    test_suite("format: signed number with '-' sign");

    uint32_t vals[] = {0u, 1u, (uint32_t)-1, 5u, (uint32_t)-5,
                       243u, (uint32_t)-243, (uint32_t)-1000,
                       2147483647u, 0x80000000u, (uint32_t)-999999};
    int n = (int)(sizeof(vals) / sizeof(vals[0]));
    int i;
    for (i = 0; i < n; i++) check_signed(vals[i]);

    rng_seed(0x516Eu);
    int k;
    for (k = 0; k < 1500; k++) check_signed(rng_next());
}

int main(void) {
    test_format_directed();
    test_format_fuzz();
    test_format_signed();
    return test_report();
}
