#include "shifter.h"
#include "../../core/mux/mux.h"

void shl_32_1(bit *a, bit *out) {
    out[0] = 0;
    out[1] = a[0]; out[2] = a[1]; out[3] = a[2]; out[4] = a[3];
    out[5] = a[4]; out[6] = a[5]; out[7] = a[6]; out[8] = a[7];
    out[9] = a[8]; out[10] = a[9]; out[11] = a[10]; out[12] = a[11];
    out[13] = a[12]; out[14] = a[13]; out[15] = a[14]; out[16] = a[15];
    out[17] = a[16]; out[18] = a[17]; out[19] = a[18]; out[20] = a[19];
    out[21] = a[20]; out[22] = a[21]; out[23] = a[22]; out[24] = a[23];
    out[25] = a[24]; out[26] = a[25]; out[27] = a[26]; out[28] = a[27];
    out[29] = a[28]; out[30] = a[29]; out[31] = a[30];
}

void shl_32_2(bit *a, bit *out) {
    out[0] = 0; out[1] = 0;
    out[2] = a[0]; out[3] = a[1]; out[4] = a[2]; out[5] = a[3];
    out[6] = a[4]; out[7] = a[5]; out[8] = a[6]; out[9] = a[7];
    out[10] = a[8]; out[11] = a[9]; out[12] = a[10]; out[13] = a[11];
    out[14] = a[12]; out[15] = a[13]; out[16] = a[14]; out[17] = a[15];
    out[18] = a[16]; out[19] = a[17]; out[20] = a[18]; out[21] = a[19];
    out[22] = a[20]; out[23] = a[21]; out[24] = a[22]; out[25] = a[23];
    out[26] = a[24]; out[27] = a[25]; out[28] = a[26]; out[29] = a[27];
    out[30] = a[28]; out[31] = a[29];
}

void shl_32_4(bit *a, bit *out) {
    out[0] = 0; out[1] = 0; out[2] = 0; out[3] = 0;
    out[4] = a[0]; out[5] = a[1]; out[6] = a[2]; out[7] = a[3];
    out[8] = a[4]; out[9] = a[5]; out[10] = a[6]; out[11] = a[7];
    out[12] = a[8]; out[13] = a[9]; out[14] = a[10]; out[15] = a[11];
    out[16] = a[12]; out[17] = a[13]; out[18] = a[14]; out[19] = a[15];
    out[20] = a[16]; out[21] = a[17]; out[22] = a[18]; out[23] = a[19];
    out[24] = a[20]; out[25] = a[21]; out[26] = a[22]; out[27] = a[23];
    out[28] = a[24]; out[29] = a[25]; out[30] = a[26]; out[31] = a[27];
}

void shl_32_8(bit *a, bit *out) {
    out[0] = 0; out[1] = 0; out[2] = 0; out[3] = 0;
    out[4] = 0; out[5] = 0; out[6] = 0; out[7] = 0;
    out[8] = a[0]; out[9] = a[1]; out[10] = a[2]; out[11] = a[3];
    out[12] = a[4]; out[13] = a[5]; out[14] = a[6]; out[15] = a[7];
    out[16] = a[8]; out[17] = a[9]; out[18] = a[10]; out[19] = a[11];
    out[20] = a[12]; out[21] = a[13]; out[22] = a[14]; out[23] = a[15];
    out[24] = a[16]; out[25] = a[17]; out[26] = a[18]; out[27] = a[19];
    out[28] = a[20]; out[29] = a[21]; out[30] = a[22]; out[31] = a[23];
}

void shl_32_16(bit *a, bit *out) {
    out[0] = 0; out[1] = 0; out[2] = 0; out[3] = 0;
    out[4] = 0; out[5] = 0; out[6] = 0; out[7] = 0;
    out[8] = 0; out[9] = 0; out[10] = 0; out[11] = 0;
    out[12] = 0; out[13] = 0; out[14] = 0; out[15] = 0;
    out[16] = a[0]; out[17] = a[1]; out[18] = a[2]; out[19] = a[3];
    out[20] = a[4]; out[21] = a[5]; out[22] = a[6]; out[23] = a[7];
    out[24] = a[8]; out[25] = a[9]; out[26] = a[10]; out[27] = a[11];
    out[28] = a[12]; out[29] = a[13]; out[30] = a[14]; out[31] = a[15];
}

void shr_32_1(bit *a, bit *out) {
    out[0] = a[1]; out[1] = a[2]; out[2] = a[3]; out[3] = a[4];
    out[4] = a[5]; out[5] = a[6]; out[6] = a[7]; out[7] = a[8];
    out[8] = a[9]; out[9] = a[10]; out[10] = a[11]; out[11] = a[12];
    out[12] = a[13]; out[13] = a[14]; out[14] = a[15]; out[15] = a[16];
    out[16] = a[17]; out[17] = a[18]; out[18] = a[19]; out[19] = a[20];
    out[20] = a[21]; out[21] = a[22]; out[22] = a[23]; out[23] = a[24];
    out[24] = a[25]; out[25] = a[26]; out[26] = a[27]; out[27] = a[28];
    out[28] = a[29]; out[29] = a[30]; out[30] = a[31]; out[31] = 0;
}

void shr_32_2(bit *a, bit *out) {
    out[0] = a[2]; out[1] = a[3]; out[2] = a[4]; out[3] = a[5];
    out[4] = a[6]; out[5] = a[7]; out[6] = a[8]; out[7] = a[9];
    out[8] = a[10]; out[9] = a[11]; out[10] = a[12]; out[11] = a[13];
    out[12] = a[14]; out[13] = a[15]; out[14] = a[16]; out[15] = a[17];
    out[16] = a[18]; out[17] = a[19]; out[18] = a[20]; out[19] = a[21];
    out[20] = a[22]; out[21] = a[23]; out[22] = a[24]; out[23] = a[25];
    out[24] = a[26]; out[25] = a[27]; out[26] = a[28]; out[27] = a[29];
    out[28] = a[30]; out[29] = a[31]; out[30] = 0; out[31] = 0;
}

void shr_32_4(bit *a, bit *out) {
    out[0] = a[4]; out[1] = a[5]; out[2] = a[6]; out[3] = a[7];
    out[4] = a[8]; out[5] = a[9]; out[6] = a[10]; out[7] = a[11];
    out[8] = a[12]; out[9] = a[13]; out[10] = a[14]; out[11] = a[15];
    out[12] = a[16]; out[13] = a[17]; out[14] = a[18]; out[15] = a[19];
    out[16] = a[20]; out[17] = a[21]; out[18] = a[22]; out[19] = a[23];
    out[20] = a[24]; out[21] = a[25]; out[22] = a[26]; out[23] = a[27];
    out[24] = a[28]; out[25] = a[29]; out[26] = a[30]; out[27] = a[31];
    out[28] = 0; out[29] = 0; out[30] = 0; out[31] = 0;
}

void shr_32_8(bit *a, bit *out) {
    out[0] = a[8]; out[1] = a[9]; out[2] = a[10]; out[3] = a[11];
    out[4] = a[12]; out[5] = a[13]; out[6] = a[14]; out[7] = a[15];
    out[8] = a[16]; out[9] = a[17]; out[10] = a[18]; out[11] = a[19];
    out[12] = a[20]; out[13] = a[21]; out[14] = a[22]; out[15] = a[23];
    out[16] = a[24]; out[17] = a[25]; out[18] = a[26]; out[19] = a[27];
    out[20] = a[28]; out[21] = a[29]; out[22] = a[30]; out[23] = a[31];
    out[24] = 0; out[25] = 0; out[26] = 0; out[27] = 0;
    out[28] = 0; out[29] = 0; out[30] = 0; out[31] = 0;
}

void shr_32_16(bit *a, bit *out) {
    out[0] = a[16]; out[1] = a[17]; out[2] = a[18]; out[3] = a[19];
    out[4] = a[20]; out[5] = a[21]; out[6] = a[22]; out[7] = a[23];
    out[8] = a[24]; out[9] = a[25]; out[10] = a[26]; out[11] = a[27];
    out[12] = a[28]; out[13] = a[29]; out[14] = a[30]; out[15] = a[31];
    out[16] = 0; out[17] = 0; out[18] = 0; out[19] = 0;
    out[20] = 0; out[21] = 0; out[22] = 0; out[23] = 0;
    out[24] = 0; out[25] = 0; out[26] = 0; out[27] = 0;
    out[28] = 0; out[29] = 0; out[30] = 0; out[31] = 0;
}

void shl_32(bit *a, bit *n, bit *out) {
    bit t0[32], t1[32], t2[32], t3[32];
    bit s1[32], s2[32], s4[32], s8[32], s16[32];

    shl_32_1(a, s1);
    mux2_32(a, s1, n[0], t0);

    shl_32_2(t0, s2);
    mux2_32(t0, s2, n[1], t1);

    shl_32_4(t1, s4);
    mux2_32(t1, s4, n[2], t2);

    shl_32_8(t2, s8);
    mux2_32(t2, s8, n[3], t3);

    shl_32_16(t3, s16);
    mux2_32(t3, s16, n[4], out);
}

void shr_32(bit *a, bit *n, bit *out) {
    bit t0[32], t1[32], t2[32], t3[32];
    bit s1[32], s2[32], s4[32], s8[32], s16[32];

    shr_32_1(a, s1);
    mux2_32(a, s1, n[0], t0);

    shr_32_2(t0, s2);
    mux2_32(t0, s2, n[1], t1);

    shr_32_4(t1, s4);
    mux2_32(t1, s4, n[2], t2);

    shr_32_8(t2, s8);
    mux2_32(t2, s8, n[3], t3);

    shr_32_16(t3, s16);
    mux2_32(t3, s16, n[4], out);
}
