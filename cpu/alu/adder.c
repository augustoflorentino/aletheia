#include "adder.h"
#include "../../core/gates/gates.h"

void half_adder(bit a, bit b, bit *sum, bit *carry) {
    *sum = XOR(a, b);
    *carry = AND(a, b);
}

void full_adder(bit a, bit b, bit cin, bit *sum, bit *cout) {
    bit s1, c1, c2;
    half_adder(a, b, &s1, &c1);
    half_adder(s1, cin, sum, &c2);
    *cout = OR(c1, c2);
}

void add_4bit(bit *a, bit *b, bit cin, bit *result, bit *cout) {
    bit c[5];
    c[0] = cin;
    full_adder(a[0], b[0], c[0], &result[0], &c[1]);
    full_adder(a[1], b[1], c[1], &result[1], &c[2]);
    full_adder(a[2], b[2], c[2], &result[2], &c[3]);
    full_adder(a[3], b[3], c[3], &result[3], &c[4]);
    *cout = c[4];
}

void adder_8(bit *a, bit *b, bit *result, bit *carries) {
    carries[0] = 0;
    full_adder(a[0], b[0], 0, &result[0], &carries[1]);
    full_adder(a[1], b[1], carries[1], &result[1], &carries[2]);
    full_adder(a[2], b[2], carries[2], &result[2], &carries[3]);
    full_adder(a[3], b[3], carries[3], &result[3], &carries[4]);
    full_adder(a[4], b[4], carries[4], &result[4], &carries[5]);
    full_adder(a[5], b[5], carries[5], &result[5], &carries[6]);
    full_adder(a[6], b[6], carries[6], &result[6], &carries[7]);
    full_adder(a[7], b[7], carries[7], &result[7], &carries[8]);
}

void adder_8_cin(bit *a, bit *b, bit cin, bit *result) {
    bit c[9];
    c[0] = cin;
    full_adder(a[0], b[0], c[0], &result[0], &c[1]);
    full_adder(a[1], b[1], c[1], &result[1], &c[2]);
    full_adder(a[2], b[2], c[2], &result[2], &c[3]);
    full_adder(a[3], b[3], c[3], &result[3], &c[4]);
    full_adder(a[4], b[4], c[4], &result[4], &c[5]);
    full_adder(a[5], b[5], c[5], &result[5], &c[6]);
    full_adder(a[6], b[6], c[6], &result[6], &c[7]);
    full_adder(a[7], b[7], c[7], &result[7], &c[8]);
}

void adder_32(bit *a, bit *b, bit *result, bit *carries) {
    carries[0] = 0;
    full_adder(a[0], b[0], 0, &result[0], &carries[1]);
    full_adder(a[1], b[1], carries[1], &result[1], &carries[2]);
    full_adder(a[2], b[2], carries[2], &result[2], &carries[3]);
    full_adder(a[3], b[3], carries[3], &result[3], &carries[4]);
    full_adder(a[4], b[4], carries[4], &result[4], &carries[5]);
    full_adder(a[5], b[5], carries[5], &result[5], &carries[6]);
    full_adder(a[6], b[6], carries[6], &result[6], &carries[7]);
    full_adder(a[7], b[7], carries[7], &result[7], &carries[8]);
    full_adder(a[8], b[8], carries[8], &result[8], &carries[9]);
    full_adder(a[9], b[9], carries[9], &result[9], &carries[10]);
    full_adder(a[10], b[10], carries[10], &result[10], &carries[11]);
    full_adder(a[11], b[11], carries[11], &result[11], &carries[12]);
    full_adder(a[12], b[12], carries[12], &result[12], &carries[13]);
    full_adder(a[13], b[13], carries[13], &result[13], &carries[14]);
    full_adder(a[14], b[14], carries[14], &result[14], &carries[15]);
    full_adder(a[15], b[15], carries[15], &result[15], &carries[16]);
    full_adder(a[16], b[16], carries[16], &result[16], &carries[17]);
    full_adder(a[17], b[17], carries[17], &result[17], &carries[18]);
    full_adder(a[18], b[18], carries[18], &result[18], &carries[19]);
    full_adder(a[19], b[19], carries[19], &result[19], &carries[20]);
    full_adder(a[20], b[20], carries[20], &result[20], &carries[21]);
    full_adder(a[21], b[21], carries[21], &result[21], &carries[22]);
    full_adder(a[22], b[22], carries[22], &result[22], &carries[23]);
    full_adder(a[23], b[23], carries[23], &result[23], &carries[24]);
    full_adder(a[24], b[24], carries[24], &result[24], &carries[25]);
    full_adder(a[25], b[25], carries[25], &result[25], &carries[26]);
    full_adder(a[26], b[26], carries[26], &result[26], &carries[27]);
    full_adder(a[27], b[27], carries[27], &result[27], &carries[28]);
    full_adder(a[28], b[28], carries[28], &result[28], &carries[29]);
    full_adder(a[29], b[29], carries[29], &result[29], &carries[30]);
    full_adder(a[30], b[30], carries[30], &result[30], &carries[31]);
    full_adder(a[31], b[31], carries[31], &result[31], &carries[32]);
}

void adder_32_cin(bit *a, bit *b, bit cin, bit *result) {
    bit c[33];
    c[0] = cin;
    full_adder(a[0], b[0], c[0], &result[0], &c[1]);
    full_adder(a[1], b[1], c[1], &result[1], &c[2]);
    full_adder(a[2], b[2], c[2], &result[2], &c[3]);
    full_adder(a[3], b[3], c[3], &result[3], &c[4]);
    full_adder(a[4], b[4], c[4], &result[4], &c[5]);
    full_adder(a[5], b[5], c[5], &result[5], &c[6]);
    full_adder(a[6], b[6], c[6], &result[6], &c[7]);
    full_adder(a[7], b[7], c[7], &result[7], &c[8]);
    full_adder(a[8], b[8], c[8], &result[8], &c[9]);
    full_adder(a[9], b[9], c[9], &result[9], &c[10]);
    full_adder(a[10], b[10], c[10], &result[10], &c[11]);
    full_adder(a[11], b[11], c[11], &result[11], &c[12]);
    full_adder(a[12], b[12], c[12], &result[12], &c[13]);
    full_adder(a[13], b[13], c[13], &result[13], &c[14]);
    full_adder(a[14], b[14], c[14], &result[14], &c[15]);
    full_adder(a[15], b[15], c[15], &result[15], &c[16]);
    full_adder(a[16], b[16], c[16], &result[16], &c[17]);
    full_adder(a[17], b[17], c[17], &result[17], &c[18]);
    full_adder(a[18], b[18], c[18], &result[18], &c[19]);
    full_adder(a[19], b[19], c[19], &result[19], &c[20]);
    full_adder(a[20], b[20], c[20], &result[20], &c[21]);
    full_adder(a[21], b[21], c[21], &result[21], &c[22]);
    full_adder(a[22], b[22], c[22], &result[22], &c[23]);
    full_adder(a[23], b[23], c[23], &result[23], &c[24]);
    full_adder(a[24], b[24], c[24], &result[24], &c[25]);
    full_adder(a[25], b[25], c[25], &result[25], &c[26]);
    full_adder(a[26], b[26], c[26], &result[26], &c[27]);
    full_adder(a[27], b[27], c[27], &result[27], &c[28]);
    full_adder(a[28], b[28], c[28], &result[28], &c[29]);
    full_adder(a[29], b[29], c[29], &result[29], &c[30]);
    full_adder(a[30], b[30], c[30], &result[30], &c[31]);
    full_adder(a[31], b[31], c[31], &result[31], &c[32]);
}
