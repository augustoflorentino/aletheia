#ifndef ALETHEIA_TGATE_H
#define ALETHEIA_TGATE_H

#include "scheduler.h"
#include "../core/gates/gates.h"

typedef struct {
    bit v;
    sim_time_t t;
} tsig;

static sim_time_t g_gate_delay = 1;
static sim_time_t g_critical_path = 0;

static inline void tgate_set_delay(sim_time_t d) {
    g_gate_delay = d;
}
static inline void tgate_reset_critical(void) {
    g_critical_path = 0;
}
static inline sim_time_t tgate_critical(void) {
    return g_critical_path;
}

static inline sim_time_t tmax(sim_time_t a, sim_time_t b) {
    return a > b ? a : b;
}

static inline tsig ts(bit v) {
    tsig s;
    s.v = v;
    s.t = 0;
    return s;
}

static inline tsig ts_at(bit v, sim_time_t t) {
    tsig s;
    s.v = v;
    s.t = t;
    return s;
}

static inline tsig t_track(bit v, sim_time_t t) {
    tsig s;
    s.v = v;
    s.t = t;
    if (t > g_critical_path) g_critical_path = t;
    return s;
}

static inline tsig t_and(tsig a, tsig b) {
    return t_track(AND(a.v, b.v), tmax(a.t, b.t) + g_gate_delay);
}

static inline tsig t_or(tsig a, tsig b) {
    return t_track(OR(a.v, b.v), tmax(a.t, b.t) + g_gate_delay);
}

static inline tsig t_xor(tsig a, tsig b) {
    return t_track(XOR(a.v, b.v), tmax(a.t, b.t) + g_gate_delay);
}

static inline tsig t_not(tsig a) {
    return t_track(NOT(a.v), a.t + g_gate_delay);
}

static inline tsig t_nand(tsig a, tsig b) {
    return t_track(NAND(a.v, b.v), tmax(a.t, b.t) + g_gate_delay);
}

static inline tsig t_nor(tsig a, tsig b) {
    return t_track(NOR(a.v, b.v), tmax(a.t, b.t) + g_gate_delay);
}

static inline tsig t_mux(tsig a, tsig b, tsig sel) {
    tsig ns = t_not(sel);
    tsig a0 = t_and(a, ns);
    tsig b1 = t_and(b, sel);
    return t_or(a0, b1);
}

static inline void t_half_adder(tsig a, tsig b, tsig *sum, tsig *carry) {
    *sum = t_xor(a, b);
    *carry = t_and(a, b);
}

static inline void t_full_adder(tsig a, tsig b, tsig cin, tsig *sum, tsig *cout) {
    tsig s1, c1, c2;
    t_half_adder(a, b, &s1, &c1);
    t_half_adder(s1, cin, sum, &c2);
    *cout = t_or(c1, c2);
}

#endif
