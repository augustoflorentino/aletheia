#include "mux4.h"
#include "mux2.h"

bit mux4(bit a, bit b, bit c, bit d, bit sel0, bit sel1) {
    bit ab = mux2(a, b, sel0);
    bit cd = mux2(c, d, sel0);
    return mux2(ab, cd, sel1);
}

void mux4_8(bit *a, bit *b, bit *c, bit *d, bit sel0, bit sel1, bit *out) {
    out[0] = mux4(a[0], b[0], c[0], d[0], sel0, sel1);
    out[1] = mux4(a[1], b[1], c[1], d[1], sel0, sel1);
    out[2] = mux4(a[2], b[2], c[2], d[2], sel0, sel1);
    out[3] = mux4(a[3], b[3], c[3], d[3], sel0, sel1);
    out[4] = mux4(a[4], b[4], c[4], d[4], sel0, sel1);
    out[5] = mux4(a[5], b[5], c[5], d[5], sel0, sel1);
    out[6] = mux4(a[6], b[6], c[6], d[6], sel0, sel1);
    out[7] = mux4(a[7], b[7], c[7], d[7], sel0, sel1);
}

void mux4_32(bit *a, bit *b, bit *c, bit *d, bit sel0, bit sel1, bit *out) {
    out[0] = mux4(a[0], b[0], c[0], d[0], sel0, sel1);
    out[1] = mux4(a[1], b[1], c[1], d[1], sel0, sel1);
    out[2] = mux4(a[2], b[2], c[2], d[2], sel0, sel1);
    out[3] = mux4(a[3], b[3], c[3], d[3], sel0, sel1);
    out[4] = mux4(a[4], b[4], c[4], d[4], sel0, sel1);
    out[5] = mux4(a[5], b[5], c[5], d[5], sel0, sel1);
    out[6] = mux4(a[6], b[6], c[6], d[6], sel0, sel1);
    out[7] = mux4(a[7], b[7], c[7], d[7], sel0, sel1);
    out[8] = mux4(a[8], b[8], c[8], d[8], sel0, sel1);
    out[9] = mux4(a[9], b[9], c[9], d[9], sel0, sel1);
    out[10] = mux4(a[10], b[10], c[10], d[10], sel0, sel1);
    out[11] = mux4(a[11], b[11], c[11], d[11], sel0, sel1);
    out[12] = mux4(a[12], b[12], c[12], d[12], sel0, sel1);
    out[13] = mux4(a[13], b[13], c[13], d[13], sel0, sel1);
    out[14] = mux4(a[14], b[14], c[14], d[14], sel0, sel1);
    out[15] = mux4(a[15], b[15], c[15], d[15], sel0, sel1);
    out[16] = mux4(a[16], b[16], c[16], d[16], sel0, sel1);
    out[17] = mux4(a[17], b[17], c[17], d[17], sel0, sel1);
    out[18] = mux4(a[18], b[18], c[18], d[18], sel0, sel1);
    out[19] = mux4(a[19], b[19], c[19], d[19], sel0, sel1);
    out[20] = mux4(a[20], b[20], c[20], d[20], sel0, sel1);
    out[21] = mux4(a[21], b[21], c[21], d[21], sel0, sel1);
    out[22] = mux4(a[22], b[22], c[22], d[22], sel0, sel1);
    out[23] = mux4(a[23], b[23], c[23], d[23], sel0, sel1);
    out[24] = mux4(a[24], b[24], c[24], d[24], sel0, sel1);
    out[25] = mux4(a[25], b[25], c[25], d[25], sel0, sel1);
    out[26] = mux4(a[26], b[26], c[26], d[26], sel0, sel1);
    out[27] = mux4(a[27], b[27], c[27], d[27], sel0, sel1);
    out[28] = mux4(a[28], b[28], c[28], d[28], sel0, sel1);
    out[29] = mux4(a[29], b[29], c[29], d[29], sel0, sel1);
    out[30] = mux4(a[30], b[30], c[30], d[30], sel0, sel1);
    out[31] = mux4(a[31], b[31], c[31], d[31], sel0, sel1);
}
