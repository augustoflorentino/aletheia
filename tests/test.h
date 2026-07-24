#ifndef ALETHEIA_TEST_H
#define ALETHEIA_TEST_H

#include <stdio.h>
#include <stdint.h>

#include "../core/types.h"

static int g_pass = 0;
static int g_fail = 0;
static int g_total_pass = 0;
static int g_total_fail = 0;
static const char *g_suite = "";

static inline void test_flush(void) {
    if (g_suite[0] != '\0') {
        printf("-- %s: %d passed, %d failed --\n", g_suite, g_pass, g_fail);
    }
    g_total_pass += g_pass;
    g_total_fail += g_fail;
    g_pass = 0;
    g_fail = 0;
}

static inline void test_suite(const char *name) {
    test_flush();
    g_suite = name;
    printf("== %s ==\n", name);
}

#define CHECK(cond, ...) \
    do { \
        if (cond) { \
            g_pass++; \
        } else { \
            g_fail++; \
            printf("  FAIL [%s:%d] ", __FILE__, __LINE__); \
            printf(__VA_ARGS__); \
            printf("\n"); \
        } \
    } while (0)

#define CHECK_EQ_U32(got, exp, label) \
    do { \
        uint32_t _g = (uint32_t)(got); \
        uint32_t _e = (uint32_t)(exp); \
        CHECK(_g == _e, "%s: got 0x%08x expected 0x%08x", label, _g, _e); \
    } while (0)

#define CHECK_EQ_BIT(got, exp, label) \
    do { \
        int _g = (int)(got); \
        int _e = (int)(exp); \
        CHECK(_g == _e, "%s: got %d expected %d", label, _g, _e); \
    } while (0)

static inline int test_report(void) {
    test_flush();
    printf("== total: %d passed, %d failed ==\n\n", g_total_pass, g_total_fail);
    return g_total_fail == 0 ? 0 : 1;
}

static inline void to_bits_32(uint32_t n, bit *bits) {
    int i;
    for (i = 0; i < 32; i++) {
        bits[i] = (n >> i) & 1u;
    }
}

static inline uint32_t from_bits_32(const bit *bits) {
    uint32_t r = 0;
    int i;
    for (i = 0; i < 32; i++) {
        r |= ((uint32_t)(bits[i] & 1u)) << i;
    }
    return r;
}

static inline void to_bits_n(uint32_t n, bit *bits, int width) {
    int i;
    for (i = 0; i < width; i++) {
        bits[i] = (n >> i) & 1u;
    }
}

static inline uint32_t from_bits_n(const bit *bits, int width) {
    uint32_t r = 0;
    int i;
    for (i = 0; i < width; i++) {
        r |= ((uint32_t)(bits[i] & 1u)) << i;
    }
    return r;
}

static uint32_t g_rng_state = 0x1234567u;

static inline void rng_seed(uint32_t s) {
    g_rng_state = s ? s : 0x1234567u;
}

static inline uint32_t rng_next(void) {
    uint32_t x = g_rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    g_rng_state = x;
    return x;
}

#endif
