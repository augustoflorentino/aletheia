#include "subtractor.h"
#include "adder.h"
#include "../../core/gates/gates.h"

void not_8(bit *a, bit *out) {
    out[0] = NOT(a[0]);
    out[1] = NOT(a[1]);
    out[2] = NOT(a[2]);
    out[3] = NOT(a[3]);
    out[4] = NOT(a[4]);
    out[5] = NOT(a[5]);
    out[6] = NOT(a[6]);
    out[7] = NOT(a[7]);
}

void not_32(bit *a, bit *out) {
    out[0] = NOT(a[0]);
    out[1] = NOT(a[1]);
    out[2] = NOT(a[2]);
    out[3] = NOT(a[3]);
    out[4] = NOT(a[4]);
    out[5] = NOT(a[5]);
    out[6] = NOT(a[6]);
    out[7] = NOT(a[7]);
    out[8] = NOT(a[8]);
    out[9] = NOT(a[9]);
    out[10] = NOT(a[10]);
    out[11] = NOT(a[11]);
    out[12] = NOT(a[12]);
    out[13] = NOT(a[13]);
    out[14] = NOT(a[14]);
    out[15] = NOT(a[15]);
    out[16] = NOT(a[16]);
    out[17] = NOT(a[17]);
    out[18] = NOT(a[18]);
    out[19] = NOT(a[19]);
    out[20] = NOT(a[20]);
    out[21] = NOT(a[21]);
    out[22] = NOT(a[22]);
    out[23] = NOT(a[23]);
    out[24] = NOT(a[24]);
    out[25] = NOT(a[25]);
    out[26] = NOT(a[26]);
    out[27] = NOT(a[27]);
    out[28] = NOT(a[28]);
    out[29] = NOT(a[29]);
    out[30] = NOT(a[30]);
    out[31] = NOT(a[31]);
}

void sub_8(bit *a, bit *b, bit *result) {
    bit b_inv[8];
    not_8(b, b_inv);
    adder_8_cin(a, b_inv, 1, result);
}

void sub_32(bit *a, bit *b, bit *result) {
    bit b_inv[32];
    not_32(b, b_inv);
    adder_32_cin(a, b_inv, 1, result);
}

void inc_8(bit *a, bit *result) {
    bit one[8] = {1, 0, 0, 0, 0, 0, 0, 0};
    adder_8_cin(a, one, 0, result);
}

void inc_32(bit *a, bit *result) {
    bit one[32] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adder_32_cin(a, one, 0, result);
}

void dec_8(bit *a, bit *result) {
    bit one[8] = {1, 0, 0, 0, 0, 0, 0, 0};
    sub_8(a, one, result);
}

void dec_32(bit *a, bit *result) {
    bit one[32] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    sub_32(a, one, result);
}

void negate_32(bit *a, bit *result) {
    bit zero[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    sub_32(zero, a, result);
}
