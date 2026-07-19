#include "xor.h"
#include "not.h"

bit XOR(bit a, bit b) {
    return a ^ b;
}

bit XNOR(bit a, bit b) {
    return NOT(a ^ b);
}
