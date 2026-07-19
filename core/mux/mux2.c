#include "mux2.h"
#include "../gates/gates.h"

bit mux2(bit a, bit b, bit sel) {
    return OR(AND(a, NOT(sel)), AND(b, sel));
}

void mux2_8(bit *a, bit *b, bit sel, bit *out) {
    out[0] = mux2(a[0], b[0], sel);
    out[1] = mux2(a[1], b[1], sel);
    out[2] = mux2(a[2], b[2], sel);
    out[3] = mux2(a[3], b[3], sel);
    out[4] = mux2(a[4], b[4], sel);
    out[5] = mux2(a[5], b[5], sel);
    out[6] = mux2(a[6], b[6], sel);
    out[7] = mux2(a[7], b[7], sel);
}

void mux2_32(bit *a, bit *b, bit sel, bit *out) {
    out[0] = mux2(a[0], b[0], sel);
    out[1] = mux2(a[1], b[1], sel);
    out[2] = mux2(a[2], b[2], sel);
    out[3] = mux2(a[3], b[3], sel);
    out[4] = mux2(a[4], b[4], sel);
    out[5] = mux2(a[5], b[5], sel);
    out[6] = mux2(a[6], b[6], sel);
    out[7] = mux2(a[7], b[7], sel);
    out[8] = mux2(a[8], b[8], sel);
    out[9] = mux2(a[9], b[9], sel);
    out[10] = mux2(a[10], b[10], sel);
    out[11] = mux2(a[11], b[11], sel);
    out[12] = mux2(a[12], b[12], sel);
    out[13] = mux2(a[13], b[13], sel);
    out[14] = mux2(a[14], b[14], sel);
    out[15] = mux2(a[15], b[15], sel);
    out[16] = mux2(a[16], b[16], sel);
    out[17] = mux2(a[17], b[17], sel);
    out[18] = mux2(a[18], b[18], sel);
    out[19] = mux2(a[19], b[19], sel);
    out[20] = mux2(a[20], b[20], sel);
    out[21] = mux2(a[21], b[21], sel);
    out[22] = mux2(a[22], b[22], sel);
    out[23] = mux2(a[23], b[23], sel);
    out[24] = mux2(a[24], b[24], sel);
    out[25] = mux2(a[25], b[25], sel);
    out[26] = mux2(a[26], b[26], sel);
    out[27] = mux2(a[27], b[27], sel);
    out[28] = mux2(a[28], b[28], sel);
    out[29] = mux2(a[29], b[29], sel);
    out[30] = mux2(a[30], b[30], sel);
    out[31] = mux2(a[31], b[31], sel);
}
