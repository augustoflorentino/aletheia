#include "../core/gates/gates.h"
#include "../simulation/stuck.h"

static unsigned long gate_index;
static unsigned long last_count;
static long stuck_gate = -1;
static bit stuck_value;

int stuck_arm(long gate, bit value) {
    stuck_gate = gate;
    stuck_value = value;
    return 0;
}

void stuck_new_cycle(void) {
    last_count = gate_index;
    gate_index = 0;
}

unsigned long stuck_gates_last_cycle(void) {
    return last_count;
}

bit NAND(bit a, bit b) {
    bit out = (bit)((a & b) ^ 1);
    return (long)gate_index++ == stuck_gate ? stuck_value : out;
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

int wire_select(bit x) {
    return x;
}
