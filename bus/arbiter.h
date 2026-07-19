#ifndef ALETHEIA_ARBITER_H
#define ALETHEIA_ARBITER_H

#include "../core/types.h"
#include "../core/sequential/d_flip_flop.h"

#define NUM_BUS_MASTERS 4

typedef struct {
    d_flip_flop_t grant[NUM_BUS_MASTERS];
    d_flip_flop_t busy;

    bit request[NUM_BUS_MASTERS];
    bit grant_out[NUM_BUS_MASTERS];
    bit busy_out;
    bit any_grant;

    bit priority[NUM_BUS_MASTERS][2];
} arbiter_t;

void arbiter_init(arbiter_t *arb);
void arbiter_reset(arbiter_t *arb);
void arbiter_set_request(arbiter_t *arb, bit r0, bit r1, bit r2, bit r3);
void arbiter_set_priority(arbiter_t *arb, bit *p0, bit *p1, bit *p2, bit *p3);
void arbiter_clock_rise(arbiter_t *arb);
void arbiter_clock_fall(arbiter_t *arb);
void arbiter_propagate(arbiter_t *arb);

bit arbiter_get_grant(arbiter_t *arb, bit s0, bit s1);
bit arbiter_is_busy(arbiter_t *arb);

#endif
