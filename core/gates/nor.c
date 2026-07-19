#include "nor.h"
#include "not.h"

bit NOR(bit a, bit b) {
    return NOT(a | b);
}

bit NOR3(bit a, bit b, bit c) {
    return NOT(a | b | c);
}

bit NOR4(bit a, bit b, bit c, bit d) {
    return NOT(a | b | c | d);
}
