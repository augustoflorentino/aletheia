#include "test.h"

#include "../io/convert/bin_to_bcd.h"
#include "../io/convert/bcd_to_bin.h"
#include "../io/convert/bcd_to_ascii.h"
#include "../io/convert/ascii_to_bcd.h"

static uint32_t bcd_to_value(bit bcd[BCD_DIGITS][4]) {
    uint64_t v = 0;
    uint64_t place = 1;
    int i;
    for (i = 0; i < BCD_DIGITS; i++) {
        v += (uint64_t)from_bits_n(bcd[i], 4) * place;
        place *= 10;
    }
    return (uint32_t)v;
}

static void test_bin_to_bcd(void) {
    test_suite("bcd: binary -> BCD (double dabble)");

    uint32_t vals[] = {0u, 5u, 9u, 10u, 99u, 100u, 243u, 1000u,
                       65535u, 999999u, 4294967295u, 1234567890u};
    int n = (int)(sizeof(vals) / sizeof(vals[0]));
    int i;
    for (i = 0; i < n; i++) {
        bin_to_bcd_t c;
        bin_to_bcd_init(&c);
        bit in[32];
        to_bits_32(vals[i], in);
        bin_to_bcd_set_input(&c, in);
        bin_to_bcd_convert(&c);
        CHECK_EQ_U32(bcd_to_value(c.bcd), vals[i], "bin_to_bcd value");
    }

    rng_seed(0xBCD01u);
    int k;
    for (k = 0; k < 2000; k++) {
        uint32_t v = rng_next();
        bin_to_bcd_t c;
        bin_to_bcd_init(&c);
        bit in[32];
        to_bits_32(v, in);
        bin_to_bcd_set_input(&c, in);
        bin_to_bcd_convert(&c);
        CHECK_EQ_U32(bcd_to_value(c.bcd), v, "bin_to_bcd fuzz");
    }
}

static void test_bcd_to_bin_roundtrip(void) {
    test_suite("bcd: BCD -> binary roundtrip");

    rng_seed(0xB2B99u);
    int k;
    for (k = 0; k < 2000; k++) {
        uint32_t v = rng_next();
        bin_to_bcd_t f;
        bin_to_bcd_init(&f);
        bit in[32];
        to_bits_32(v, in);
        bin_to_bcd_set_input(&f, in);
        bin_to_bcd_convert(&f);

        bcd_to_bin_t g;
        bcd_to_bin_init(&g);
        bcd_to_bin_set_bcd(&g, f.bcd);
        bcd_to_bin_convert(&g);
        CHECK_EQ_U32(from_bits_32(g.result), v, "bcd->bin roundtrip");
    }
}

static void test_bcd_ascii(void) {
    test_suite("bcd: BCD <-> ASCII digits");

    int d;
    for (d = 0; d <= 9; d++) {
        bit bcd[4], ascii[8], back[4];
        to_bits_n((uint32_t)d, bcd, 4);
        bcd_digit_to_ascii(bcd, ascii);
        CHECK_EQ_U32(from_bits_n(ascii, 8), 0x30u + (uint32_t)d, "bcd->ascii");

        ascii_digit_to_bcd(ascii, back);
        CHECK_EQ_U32(from_bits_n(back, 4), (uint32_t)d, "ascii->bcd");
        CHECK_EQ_BIT(ascii_is_digit(ascii), 1, "ascii_is_digit for digit");
    }

    bit letter[8];
    to_bits_n((uint32_t)'A', letter, 8);
    CHECK_EQ_BIT(ascii_is_digit(letter), 0, "ascii_is_digit for non-digit");
}

static void test_full_format_path(void) {
    test_suite("bcd: full number -> ASCII string path");

    uint32_t v = 243u;
    bin_to_bcd_t c;
    bin_to_bcd_init(&c);
    bit in[32];
    to_bits_32(v, in);
    bin_to_bcd_set_input(&c, in);
    bin_to_bcd_convert(&c);

    bit ascii[BCD_DIGITS][8];
    bcd_to_ascii_string(c.bcd, ascii);

    CHECK_EQ_U32(from_bits_n(ascii[0], 8), (uint32_t)'3', "digit ones = '3'");
    CHECK_EQ_U32(from_bits_n(ascii[1], 8), (uint32_t)'4', "digit tens = '4'");
    CHECK_EQ_U32(from_bits_n(ascii[2], 8), (uint32_t)'2', "digit hundreds = '2'");
}

int main(void) {
    test_bin_to_bcd();
    test_bcd_to_bin_roundtrip();
    test_bcd_ascii();
    test_full_format_path();
    return test_report();
}
