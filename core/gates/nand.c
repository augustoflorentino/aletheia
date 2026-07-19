#include "nand.h"
#include "not.h"

bit NAND(bit a, bit b) {
    return NOT(a & b);
}

bit NAND3(bit a, bit b, bit c) {
    return NOT(a & b & c);
}

bit NAND4(bit a, bit b, bit c, bit d) {
    return NOT(a & b & c & d);
}
