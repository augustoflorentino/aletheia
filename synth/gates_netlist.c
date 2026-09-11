#include "netlist.h"
#include "../core/gates/gates.h"

bit NAND(bit a, bit b) {
    return nl_nand(a, b);
}
bit NOT(bit a) {
    return nl_nand(a, a);
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
    (void)x;
    return 0;
}
