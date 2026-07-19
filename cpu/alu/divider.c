#include "divider.h"
#include "comparator.h"
#include "subtractor.h"
#include "shifter.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

void copy_32(bit *src, bit *dst) {
    dst[0] = src[0]; dst[1] = src[1]; dst[2] = src[2]; dst[3] = src[3];
    dst[4] = src[4]; dst[5] = src[5]; dst[6] = src[6]; dst[7] = src[7];
    dst[8] = src[8]; dst[9] = src[9]; dst[10] = src[10]; dst[11] = src[11];
    dst[12] = src[12]; dst[13] = src[13]; dst[14] = src[14]; dst[15] = src[15];
    dst[16] = src[16]; dst[17] = src[17]; dst[18] = src[18]; dst[19] = src[19];
    dst[20] = src[20]; dst[21] = src[21]; dst[22] = src[22]; dst[23] = src[23];
    dst[24] = src[24]; dst[25] = src[25]; dst[26] = src[26]; dst[27] = src[27];
    dst[28] = src[28]; dst[29] = src[29]; dst[30] = src[30]; dst[31] = src[31];
}

void div_step(bit *remainder, bit dividend_bit, bit *divisor, bit *new_remainder, bit *quotient_bit) {
    bit shifted[32];
    bit sub_result[32];
    bit eq, lt, gt;
    bit ge;

    shl_32_1(remainder, shifted);
    shifted[0] = dividend_bit;

    cmp_32(shifted, divisor, &eq, &lt, &gt);
    ge = OR(gt, eq);

    sub_32(shifted, divisor, sub_result);

    mux2_32(shifted, sub_result, ge, new_remainder);
    *quotient_bit = ge;
}

void div_32(bit *dividend, bit *divisor, bit *quotient, bit *remainder) {
    bit r0[32], r1[32], r2[32], r3[32], r4[32], r5[32], r6[32], r7[32];
    bit r8[32], r9[32], r10[32], r11[32], r12[32], r13[32], r14[32], r15[32];
    bit r16[32], r17[32], r18[32], r19[32], r20[32], r21[32], r22[32], r23[32];
    bit r24[32], r25[32], r26[32], r27[32], r28[32], r29[32], r30[32], r31[32];
    bit r32[32];

    r0[0] = 0; r0[1] = 0; r0[2] = 0; r0[3] = 0;
    r0[4] = 0; r0[5] = 0; r0[6] = 0; r0[7] = 0;
    r0[8] = 0; r0[9] = 0; r0[10] = 0; r0[11] = 0;
    r0[12] = 0; r0[13] = 0; r0[14] = 0; r0[15] = 0;
    r0[16] = 0; r0[17] = 0; r0[18] = 0; r0[19] = 0;
    r0[20] = 0; r0[21] = 0; r0[22] = 0; r0[23] = 0;
    r0[24] = 0; r0[25] = 0; r0[26] = 0; r0[27] = 0;
    r0[28] = 0; r0[29] = 0; r0[30] = 0; r0[31] = 0;

    div_step(r0, dividend[31], divisor, r1, &quotient[31]);
    div_step(r1, dividend[30], divisor, r2, &quotient[30]);
    div_step(r2, dividend[29], divisor, r3, &quotient[29]);
    div_step(r3, dividend[28], divisor, r4, &quotient[28]);
    div_step(r4, dividend[27], divisor, r5, &quotient[27]);
    div_step(r5, dividend[26], divisor, r6, &quotient[26]);
    div_step(r6, dividend[25], divisor, r7, &quotient[25]);
    div_step(r7, dividend[24], divisor, r8, &quotient[24]);
    div_step(r8, dividend[23], divisor, r9, &quotient[23]);
    div_step(r9, dividend[22], divisor, r10, &quotient[22]);
    div_step(r10, dividend[21], divisor, r11, &quotient[21]);
    div_step(r11, dividend[20], divisor, r12, &quotient[20]);
    div_step(r12, dividend[19], divisor, r13, &quotient[19]);
    div_step(r13, dividend[18], divisor, r14, &quotient[18]);
    div_step(r14, dividend[17], divisor, r15, &quotient[17]);
    div_step(r15, dividend[16], divisor, r16, &quotient[16]);
    div_step(r16, dividend[15], divisor, r17, &quotient[15]);
    div_step(r17, dividend[14], divisor, r18, &quotient[14]);
    div_step(r18, dividend[13], divisor, r19, &quotient[13]);
    div_step(r19, dividend[12], divisor, r20, &quotient[12]);
    div_step(r20, dividend[11], divisor, r21, &quotient[11]);
    div_step(r21, dividend[10], divisor, r22, &quotient[10]);
    div_step(r22, dividend[9], divisor, r23, &quotient[9]);
    div_step(r23, dividend[8], divisor, r24, &quotient[8]);
    div_step(r24, dividend[7], divisor, r25, &quotient[7]);
    div_step(r25, dividend[6], divisor, r26, &quotient[6]);
    div_step(r26, dividend[5], divisor, r27, &quotient[5]);
    div_step(r27, dividend[4], divisor, r28, &quotient[4]);
    div_step(r28, dividend[3], divisor, r29, &quotient[3]);
    div_step(r29, dividend[2], divisor, r30, &quotient[2]);
    div_step(r30, dividend[1], divisor, r31, &quotient[1]);
    div_step(r31, dividend[0], divisor, r32, &quotient[0]);

    copy_32(r32, remainder);
}

void mod_32(bit *dividend, bit *divisor, bit *remainder) {
    bit quotient[32];
    div_32(dividend, divisor, quotient, remainder);
}
