#include "comparator.h"
#include "../../core/gates/gates.h"

bit bit_eq(bit a, bit b) {
    return NOT(XOR(a, b));
}

bit bit_lt(bit a, bit b) {
    return AND(NOT(a), b);
}

bit bit_gt(bit a, bit b) {
    return AND(a, NOT(b));
}

bit is_zero_8(bit *a) {
    bit r;
    r = OR(a[0], a[1]);
    r = OR(r, a[2]);
    r = OR(r, a[3]);
    r = OR(r, a[4]);
    r = OR(r, a[5]);
    r = OR(r, a[6]);
    r = OR(r, a[7]);
    return NOT(r);
}

bit is_zero_32(bit *a) {
    bit r;
    r = OR(a[0], a[1]);
    r = OR(r, a[2]);
    r = OR(r, a[3]);
    r = OR(r, a[4]);
    r = OR(r, a[5]);
    r = OR(r, a[6]);
    r = OR(r, a[7]);
    r = OR(r, a[8]);
    r = OR(r, a[9]);
    r = OR(r, a[10]);
    r = OR(r, a[11]);
    r = OR(r, a[12]);
    r = OR(r, a[13]);
    r = OR(r, a[14]);
    r = OR(r, a[15]);
    r = OR(r, a[16]);
    r = OR(r, a[17]);
    r = OR(r, a[18]);
    r = OR(r, a[19]);
    r = OR(r, a[20]);
    r = OR(r, a[21]);
    r = OR(r, a[22]);
    r = OR(r, a[23]);
    r = OR(r, a[24]);
    r = OR(r, a[25]);
    r = OR(r, a[26]);
    r = OR(r, a[27]);
    r = OR(r, a[28]);
    r = OR(r, a[29]);
    r = OR(r, a[30]);
    r = OR(r, a[31]);
    return NOT(r);
}

void cmp_8(bit *a, bit *b, bit *eq, bit *lt, bit *gt) {
    bit e7, e6, e5, e4, e3, e2, e1, e0;
    bit l7, l6, l5, l4, l3, l2, l1, l0;
    bit g7, g6, g5, g4, g3, g2, g1, g0;

    e7 = bit_eq(a[7], b[7]);
    e6 = bit_eq(a[6], b[6]);
    e5 = bit_eq(a[5], b[5]);
    e4 = bit_eq(a[4], b[4]);
    e3 = bit_eq(a[3], b[3]);
    e2 = bit_eq(a[2], b[2]);
    e1 = bit_eq(a[1], b[1]);
    e0 = bit_eq(a[0], b[0]);

    l7 = bit_lt(a[7], b[7]);
    l6 = bit_lt(a[6], b[6]);
    l5 = bit_lt(a[5], b[5]);
    l4 = bit_lt(a[4], b[4]);
    l3 = bit_lt(a[3], b[3]);
    l2 = bit_lt(a[2], b[2]);
    l1 = bit_lt(a[1], b[1]);
    l0 = bit_lt(a[0], b[0]);

    g7 = bit_gt(a[7], b[7]);
    g6 = bit_gt(a[6], b[6]);
    g5 = bit_gt(a[5], b[5]);
    g4 = bit_gt(a[4], b[4]);
    g3 = bit_gt(a[3], b[3]);
    g2 = bit_gt(a[2], b[2]);
    g1 = bit_gt(a[1], b[1]);
    g0 = bit_gt(a[0], b[0]);

    *eq = AND(AND(AND(e7, e6), AND(e5, e4)), AND(AND(e3, e2), AND(e1, e0)));

    *lt = OR(l7,
          OR(AND(e7, l6),
          OR(AND(AND(e7, e6), l5),
          OR(AND(AND(AND(e7, e6), e5), l4),
          OR(AND(AND(AND(AND(e7, e6), e5), e4), l3),
          OR(AND(AND(AND(AND(AND(e7, e6), e5), e4), e3), l2),
          OR(AND(AND(AND(AND(AND(AND(e7, e6), e5), e4), e3), e2), l1),
             AND(AND(AND(AND(AND(AND(AND(e7, e6), e5), e4), e3), e2), e1), l0))))))));

    *gt = OR(g7,
          OR(AND(e7, g6),
          OR(AND(AND(e7, e6), g5),
          OR(AND(AND(AND(e7, e6), e5), g4),
          OR(AND(AND(AND(AND(e7, e6), e5), e4), g3),
          OR(AND(AND(AND(AND(AND(e7, e6), e5), e4), e3), g2),
          OR(AND(AND(AND(AND(AND(AND(e7, e6), e5), e4), e3), e2), g1),
             AND(AND(AND(AND(AND(AND(AND(e7, e6), e5), e4), e3), e2), e1), g0))))))));
}

void cmp_32(bit *a, bit *b, bit *eq, bit *lt, bit *gt) {
    bit e[32], l[32], g[32];

    e[31] = bit_eq(a[31], b[31]);
    e[30] = bit_eq(a[30], b[30]);
    e[29] = bit_eq(a[29], b[29]);
    e[28] = bit_eq(a[28], b[28]);
    e[27] = bit_eq(a[27], b[27]);
    e[26] = bit_eq(a[26], b[26]);
    e[25] = bit_eq(a[25], b[25]);
    e[24] = bit_eq(a[24], b[24]);
    e[23] = bit_eq(a[23], b[23]);
    e[22] = bit_eq(a[22], b[22]);
    e[21] = bit_eq(a[21], b[21]);
    e[20] = bit_eq(a[20], b[20]);
    e[19] = bit_eq(a[19], b[19]);
    e[18] = bit_eq(a[18], b[18]);
    e[17] = bit_eq(a[17], b[17]);
    e[16] = bit_eq(a[16], b[16]);
    e[15] = bit_eq(a[15], b[15]);
    e[14] = bit_eq(a[14], b[14]);
    e[13] = bit_eq(a[13], b[13]);
    e[12] = bit_eq(a[12], b[12]);
    e[11] = bit_eq(a[11], b[11]);
    e[10] = bit_eq(a[10], b[10]);
    e[9] = bit_eq(a[9], b[9]);
    e[8] = bit_eq(a[8], b[8]);
    e[7] = bit_eq(a[7], b[7]);
    e[6] = bit_eq(a[6], b[6]);
    e[5] = bit_eq(a[5], b[5]);
    e[4] = bit_eq(a[4], b[4]);
    e[3] = bit_eq(a[3], b[3]);
    e[2] = bit_eq(a[2], b[2]);
    e[1] = bit_eq(a[1], b[1]);
    e[0] = bit_eq(a[0], b[0]);

    l[31] = bit_lt(a[31], b[31]);
    l[30] = bit_lt(a[30], b[30]);
    l[29] = bit_lt(a[29], b[29]);
    l[28] = bit_lt(a[28], b[28]);
    l[27] = bit_lt(a[27], b[27]);
    l[26] = bit_lt(a[26], b[26]);
    l[25] = bit_lt(a[25], b[25]);
    l[24] = bit_lt(a[24], b[24]);
    l[23] = bit_lt(a[23], b[23]);
    l[22] = bit_lt(a[22], b[22]);
    l[21] = bit_lt(a[21], b[21]);
    l[20] = bit_lt(a[20], b[20]);
    l[19] = bit_lt(a[19], b[19]);
    l[18] = bit_lt(a[18], b[18]);
    l[17] = bit_lt(a[17], b[17]);
    l[16] = bit_lt(a[16], b[16]);
    l[15] = bit_lt(a[15], b[15]);
    l[14] = bit_lt(a[14], b[14]);
    l[13] = bit_lt(a[13], b[13]);
    l[12] = bit_lt(a[12], b[12]);
    l[11] = bit_lt(a[11], b[11]);
    l[10] = bit_lt(a[10], b[10]);
    l[9] = bit_lt(a[9], b[9]);
    l[8] = bit_lt(a[8], b[8]);
    l[7] = bit_lt(a[7], b[7]);
    l[6] = bit_lt(a[6], b[6]);
    l[5] = bit_lt(a[5], b[5]);
    l[4] = bit_lt(a[4], b[4]);
    l[3] = bit_lt(a[3], b[3]);
    l[2] = bit_lt(a[2], b[2]);
    l[1] = bit_lt(a[1], b[1]);
    l[0] = bit_lt(a[0], b[0]);

    g[31] = bit_gt(a[31], b[31]);
    g[30] = bit_gt(a[30], b[30]);
    g[29] = bit_gt(a[29], b[29]);
    g[28] = bit_gt(a[28], b[28]);
    g[27] = bit_gt(a[27], b[27]);
    g[26] = bit_gt(a[26], b[26]);
    g[25] = bit_gt(a[25], b[25]);
    g[24] = bit_gt(a[24], b[24]);
    g[23] = bit_gt(a[23], b[23]);
    g[22] = bit_gt(a[22], b[22]);
    g[21] = bit_gt(a[21], b[21]);
    g[20] = bit_gt(a[20], b[20]);
    g[19] = bit_gt(a[19], b[19]);
    g[18] = bit_gt(a[18], b[18]);
    g[17] = bit_gt(a[17], b[17]);
    g[16] = bit_gt(a[16], b[16]);
    g[15] = bit_gt(a[15], b[15]);
    g[14] = bit_gt(a[14], b[14]);
    g[13] = bit_gt(a[13], b[13]);
    g[12] = bit_gt(a[12], b[12]);
    g[11] = bit_gt(a[11], b[11]);
    g[10] = bit_gt(a[10], b[10]);
    g[9] = bit_gt(a[9], b[9]);
    g[8] = bit_gt(a[8], b[8]);
    g[7] = bit_gt(a[7], b[7]);
    g[6] = bit_gt(a[6], b[6]);
    g[5] = bit_gt(a[5], b[5]);
    g[4] = bit_gt(a[4], b[4]);
    g[3] = bit_gt(a[3], b[3]);
    g[2] = bit_gt(a[2], b[2]);
    g[1] = bit_gt(a[1], b[1]);
    g[0] = bit_gt(a[0], b[0]);

    bit eq_all;
    eq_all = AND(e[31], e[30]);
    eq_all = AND(eq_all, e[29]);
    eq_all = AND(eq_all, e[28]);
    eq_all = AND(eq_all, e[27]);
    eq_all = AND(eq_all, e[26]);
    eq_all = AND(eq_all, e[25]);
    eq_all = AND(eq_all, e[24]);
    eq_all = AND(eq_all, e[23]);
    eq_all = AND(eq_all, e[22]);
    eq_all = AND(eq_all, e[21]);
    eq_all = AND(eq_all, e[20]);
    eq_all = AND(eq_all, e[19]);
    eq_all = AND(eq_all, e[18]);
    eq_all = AND(eq_all, e[17]);
    eq_all = AND(eq_all, e[16]);
    eq_all = AND(eq_all, e[15]);
    eq_all = AND(eq_all, e[14]);
    eq_all = AND(eq_all, e[13]);
    eq_all = AND(eq_all, e[12]);
    eq_all = AND(eq_all, e[11]);
    eq_all = AND(eq_all, e[10]);
    eq_all = AND(eq_all, e[9]);
    eq_all = AND(eq_all, e[8]);
    eq_all = AND(eq_all, e[7]);
    eq_all = AND(eq_all, e[6]);
    eq_all = AND(eq_all, e[5]);
    eq_all = AND(eq_all, e[4]);
    eq_all = AND(eq_all, e[3]);
    eq_all = AND(eq_all, e[2]);
    eq_all = AND(eq_all, e[1]);
    eq_all = AND(eq_all, e[0]);
    *eq = eq_all;

    bit eq_from[32];
    eq_from[31] = 1;
    eq_from[30] = e[31];
    eq_from[29] = AND(eq_from[30], e[30]);
    eq_from[28] = AND(eq_from[29], e[29]);
    eq_from[27] = AND(eq_from[28], e[28]);
    eq_from[26] = AND(eq_from[27], e[27]);
    eq_from[25] = AND(eq_from[26], e[26]);
    eq_from[24] = AND(eq_from[25], e[25]);
    eq_from[23] = AND(eq_from[24], e[24]);
    eq_from[22] = AND(eq_from[23], e[23]);
    eq_from[21] = AND(eq_from[22], e[22]);
    eq_from[20] = AND(eq_from[21], e[21]);
    eq_from[19] = AND(eq_from[20], e[20]);
    eq_from[18] = AND(eq_from[19], e[19]);
    eq_from[17] = AND(eq_from[18], e[18]);
    eq_from[16] = AND(eq_from[17], e[17]);
    eq_from[15] = AND(eq_from[16], e[16]);
    eq_from[14] = AND(eq_from[15], e[15]);
    eq_from[13] = AND(eq_from[14], e[14]);
    eq_from[12] = AND(eq_from[13], e[13]);
    eq_from[11] = AND(eq_from[12], e[12]);
    eq_from[10] = AND(eq_from[11], e[11]);
    eq_from[9] = AND(eq_from[10], e[10]);
    eq_from[8] = AND(eq_from[9], e[9]);
    eq_from[7] = AND(eq_from[8], e[8]);
    eq_from[6] = AND(eq_from[7], e[7]);
    eq_from[5] = AND(eq_from[6], e[6]);
    eq_from[4] = AND(eq_from[5], e[5]);
    eq_from[3] = AND(eq_from[4], e[4]);
    eq_from[2] = AND(eq_from[3], e[3]);
    eq_from[1] = AND(eq_from[2], e[2]);
    eq_from[0] = AND(eq_from[1], e[1]);

    bit lt_result;
    lt_result = AND(eq_from[31], l[31]);
    lt_result = OR(lt_result, AND(eq_from[30], l[30]));
    lt_result = OR(lt_result, AND(eq_from[29], l[29]));
    lt_result = OR(lt_result, AND(eq_from[28], l[28]));
    lt_result = OR(lt_result, AND(eq_from[27], l[27]));
    lt_result = OR(lt_result, AND(eq_from[26], l[26]));
    lt_result = OR(lt_result, AND(eq_from[25], l[25]));
    lt_result = OR(lt_result, AND(eq_from[24], l[24]));
    lt_result = OR(lt_result, AND(eq_from[23], l[23]));
    lt_result = OR(lt_result, AND(eq_from[22], l[22]));
    lt_result = OR(lt_result, AND(eq_from[21], l[21]));
    lt_result = OR(lt_result, AND(eq_from[20], l[20]));
    lt_result = OR(lt_result, AND(eq_from[19], l[19]));
    lt_result = OR(lt_result, AND(eq_from[18], l[18]));
    lt_result = OR(lt_result, AND(eq_from[17], l[17]));
    lt_result = OR(lt_result, AND(eq_from[16], l[16]));
    lt_result = OR(lt_result, AND(eq_from[15], l[15]));
    lt_result = OR(lt_result, AND(eq_from[14], l[14]));
    lt_result = OR(lt_result, AND(eq_from[13], l[13]));
    lt_result = OR(lt_result, AND(eq_from[12], l[12]));
    lt_result = OR(lt_result, AND(eq_from[11], l[11]));
    lt_result = OR(lt_result, AND(eq_from[10], l[10]));
    lt_result = OR(lt_result, AND(eq_from[9], l[9]));
    lt_result = OR(lt_result, AND(eq_from[8], l[8]));
    lt_result = OR(lt_result, AND(eq_from[7], l[7]));
    lt_result = OR(lt_result, AND(eq_from[6], l[6]));
    lt_result = OR(lt_result, AND(eq_from[5], l[5]));
    lt_result = OR(lt_result, AND(eq_from[4], l[4]));
    lt_result = OR(lt_result, AND(eq_from[3], l[3]));
    lt_result = OR(lt_result, AND(eq_from[2], l[2]));
    lt_result = OR(lt_result, AND(eq_from[1], l[1]));
    lt_result = OR(lt_result, AND(eq_from[0], l[0]));
    *lt = lt_result;

    bit gt_result;
    gt_result = AND(eq_from[31], g[31]);
    gt_result = OR(gt_result, AND(eq_from[30], g[30]));
    gt_result = OR(gt_result, AND(eq_from[29], g[29]));
    gt_result = OR(gt_result, AND(eq_from[28], g[28]));
    gt_result = OR(gt_result, AND(eq_from[27], g[27]));
    gt_result = OR(gt_result, AND(eq_from[26], g[26]));
    gt_result = OR(gt_result, AND(eq_from[25], g[25]));
    gt_result = OR(gt_result, AND(eq_from[24], g[24]));
    gt_result = OR(gt_result, AND(eq_from[23], g[23]));
    gt_result = OR(gt_result, AND(eq_from[22], g[22]));
    gt_result = OR(gt_result, AND(eq_from[21], g[21]));
    gt_result = OR(gt_result, AND(eq_from[20], g[20]));
    gt_result = OR(gt_result, AND(eq_from[19], g[19]));
    gt_result = OR(gt_result, AND(eq_from[18], g[18]));
    gt_result = OR(gt_result, AND(eq_from[17], g[17]));
    gt_result = OR(gt_result, AND(eq_from[16], g[16]));
    gt_result = OR(gt_result, AND(eq_from[15], g[15]));
    gt_result = OR(gt_result, AND(eq_from[14], g[14]));
    gt_result = OR(gt_result, AND(eq_from[13], g[13]));
    gt_result = OR(gt_result, AND(eq_from[12], g[12]));
    gt_result = OR(gt_result, AND(eq_from[11], g[11]));
    gt_result = OR(gt_result, AND(eq_from[10], g[10]));
    gt_result = OR(gt_result, AND(eq_from[9], g[9]));
    gt_result = OR(gt_result, AND(eq_from[8], g[8]));
    gt_result = OR(gt_result, AND(eq_from[7], g[7]));
    gt_result = OR(gt_result, AND(eq_from[6], g[6]));
    gt_result = OR(gt_result, AND(eq_from[5], g[5]));
    gt_result = OR(gt_result, AND(eq_from[4], g[4]));
    gt_result = OR(gt_result, AND(eq_from[3], g[3]));
    gt_result = OR(gt_result, AND(eq_from[2], g[2]));
    gt_result = OR(gt_result, AND(eq_from[1], g[1]));
    gt_result = OR(gt_result, AND(eq_from[0], g[0]));
    *gt = gt_result;
}
