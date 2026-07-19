#include "test.h"

#include "../io/format/number_parser.h"

static int decimal_digits(uint32_t v, unsigned char *digits) {
    unsigned char tmp[10];
    int len = 0;
    do {
        tmp[len++] = (unsigned char)(v % 10u);
        v /= 10u;
    } while (v != 0);
    int i;
    for (i = 0; i < len; i++) {
        digits[i] = tmp[len - 1 - i];
    }
    return len;
}

static int parse_number(uint32_t v, uint32_t *out, int *done) {
    unsigned char digits[10];
    int len = decimal_digits(v, digits);

    number_parser_t p;
    number_parser_init(&p);

    int i;
    for (i = 0; i < len; i++) {
        bit b[8];
        to_bits_n((uint32_t)('0' + digits[i]), b, 8);
        number_parser_set_byte(&p, b);
        number_parser_set_feed(&p, 1);
        number_parser_clock_fall(&p);
        number_parser_clock_rise(&p);
    }

    bit delim[8];
    to_bits_n((uint32_t)' ', delim, 8);
    number_parser_set_byte(&p, delim);
    number_parser_set_feed(&p, 1);
    number_parser_clock_fall(&p);
    number_parser_clock_rise(&p);

    number_parser_propagate(&p);
    *out = from_bits_32(p.value);
    *done = p.done_out;
    return 0;
}

static void test_parser_directed(void) {
    test_suite("parser: ASCII digits -> binary");

    uint32_t vals[] = {0u, 1u, 9u, 10u, 42u, 100u, 255u, 1000u,
                       65535u, 999999u, 123456789u, 4000000000u};
    int n = (int)(sizeof(vals) / sizeof(vals[0]));
    int i;
    for (i = 0; i < n; i++) {
        uint32_t got;
        int done;
        parse_number(vals[i], &got, &done);
        CHECK_EQ_U32(got, vals[i], "parsed value");
        CHECK_EQ_BIT(done, 1, "done after delimiter");
    }
}

static void test_parser_fuzz(void) {
    test_suite("parser: randomized fuzz");

    rng_seed(0x9A123u);
    int k;
    for (k = 0; k < 1500; k++) {
        uint32_t v = rng_next();
        uint32_t got;
        int done;
        parse_number(v, &got, &done);
        CHECK_EQ_U32(got, v, "parsed fuzz value");
    }
}

static void test_parser_no_feed_holds(void) {
    test_suite("parser: no feed holds / no premature done");

    number_parser_t p;
    number_parser_init(&p);
    number_parser_propagate(&p);
    CHECK_EQ_U32(from_bits_32(p.value), 0u, "initial value 0");
    CHECK_EQ_BIT(p.done_out, 0, "not done initially");

    bit b[8];
    to_bits_n((uint32_t)'7', b, 8);
    number_parser_set_byte(&p, b);
    number_parser_set_feed(&p, 0);
    number_parser_clock_fall(&p);
    number_parser_clock_rise(&p);
    number_parser_propagate(&p);
    CHECK_EQ_U32(from_bits_32(p.value), 0u, "value unchanged when feed=0");
    CHECK_EQ_BIT(p.done_out, 0, "not done when feed=0");
}

int main(void) {
    test_parser_directed();
    test_parser_fuzz();
    test_parser_no_feed_holds();
    return test_report();
}
