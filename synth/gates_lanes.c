#include "../core/gates/gates.h"
#include "lanes.h"
#include <stdlib.h>

typedef struct {
    long gate;
    bit force;
    bit value;
} lane_fault_t;

static lane_fault_t faults[LANE_COUNT + 1] = {{-1, 0, 0}};
static int fault_count;
static int cursor;
static unsigned long gate_index;
static unsigned long last_count;
static bit diverged;

bit lanes_of(bit x) {
    return x == 1 ? LANE_MACHINES : x & LANE_MACHINES;
}

static int by_gate(const void *a, const void *b) {
    long x = ((const lane_fault_t *)a)->gate, y = ((const lane_fault_t *)b)->gate;
    return (x > y) - (x < y);
}

int lanes_arm(int lane, long gate, int value) {
    if (lane < 1 || lane >= LANE_COUNT || gate < 0) return -1;
    bit mask = 1ull << lane;
    for (int i = 0; i < fault_count; i++) {
        if (faults[i].gate != gate) continue;
        faults[i].force |= mask;
        faults[i].value |= value ? mask : 0;
        return 0;
    }
    faults[fault_count++] = (lane_fault_t){gate, mask, value ? mask : 0};
    qsort(faults, (size_t)fault_count, sizeof faults[0], by_gate);
    faults[fault_count].gate = -1;
    return 0;
}

void lanes_new_cycle(void) {
    last_count = gate_index;
    gate_index = 0;
    cursor = 0;
}

unsigned long lanes_gates_last_cycle(void) {
    return last_count;
}

void lanes_reset(void) {
    fault_count = 0;
    faults[0].gate = -1;
    diverged = 0;
}

bit lanes_diverged(void) {
    return diverged;
}

int wire_select(bit x) {
    x = lanes_of(x);
    int pick = __builtin_popcountll(x) > LANE_COUNT / 2;
    diverged |= x ^ (pick ? LANE_MACHINES : 0);
    return pick;
}

bit NAND(bit a, bit b) {
    bit out = ~(lanes_of(a) & lanes_of(b)) & LANE_MACHINES;
    if ((long)gate_index++ != faults[cursor].gate) return out;
    lane_fault_t *f = &faults[cursor++];
    return (out & ~f->force) | f->value;
}

bit NOT(bit a) {
    return NAND(a, a);
}
bit AND(bit a, bit b) {
    return NOT(NAND(a, b));
}
bit OR(bit a, bit b) {
    return NAND(NOT(a), NOT(b));
}
bit NOR(bit a, bit b) {
    return NOT(OR(a, b));
}
bit XNOR(bit a, bit b) {
    return NOT(XOR(a, b));
}

bit XOR(bit a, bit b) {
    bit c = NAND(a, b);
    return NAND(NAND(a, c), NAND(b, c));
}

bit AND3(bit a, bit b, bit c) {
    return AND(AND(a, b), c);
}
bit AND4(bit a, bit b, bit c, bit d) {
    return AND(AND(a, b), AND(c, d));
}
bit OR3(bit a, bit b, bit c) {
    return OR(OR(a, b), c);
}
bit OR4(bit a, bit b, bit c, bit d) {
    return OR(OR(a, b), OR(c, d));
}
bit NAND3(bit a, bit b, bit c) {
    return NOT(AND3(a, b, c));
}
bit NAND4(bit a, bit b, bit c, bit d) {
    return NOT(AND4(a, b, c, d));
}
bit NOR3(bit a, bit b, bit c) {
    return NOT(OR3(a, b, c));
}
bit NOR4(bit a, bit b, bit c, bit d) {
    return NOT(OR4(a, b, c, d));
}
