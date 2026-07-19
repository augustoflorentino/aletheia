#include "bin_to_bcd.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"
#include "../../cpu/alu/adder.h"

void bin_to_bcd_init(bin_to_bcd_t *conv) {
    bin_to_bcd_reset(conv);
}

void bin_to_bcd_reset(bin_to_bcd_t *conv) {
    conv->input[0] = 0; conv->input[1] = 0; conv->input[2] = 0; conv->input[3] = 0;
    conv->input[4] = 0; conv->input[5] = 0; conv->input[6] = 0; conv->input[7] = 0;
    conv->input[8] = 0; conv->input[9] = 0; conv->input[10] = 0; conv->input[11] = 0;
    conv->input[12] = 0; conv->input[13] = 0; conv->input[14] = 0; conv->input[15] = 0;
    conv->input[16] = 0; conv->input[17] = 0; conv->input[18] = 0; conv->input[19] = 0;
    conv->input[20] = 0; conv->input[21] = 0; conv->input[22] = 0; conv->input[23] = 0;
    conv->input[24] = 0; conv->input[25] = 0; conv->input[26] = 0; conv->input[27] = 0;
    conv->input[28] = 0; conv->input[29] = 0; conv->input[30] = 0; conv->input[31] = 0;

    conv->bcd[0][0] = 0; conv->bcd[0][1] = 0; conv->bcd[0][2] = 0; conv->bcd[0][3] = 0;
    conv->bcd[1][0] = 0; conv->bcd[1][1] = 0; conv->bcd[1][2] = 0; conv->bcd[1][3] = 0;
    conv->bcd[2][0] = 0; conv->bcd[2][1] = 0; conv->bcd[2][2] = 0; conv->bcd[2][3] = 0;
    conv->bcd[3][0] = 0; conv->bcd[3][1] = 0; conv->bcd[3][2] = 0; conv->bcd[3][3] = 0;
    conv->bcd[4][0] = 0; conv->bcd[4][1] = 0; conv->bcd[4][2] = 0; conv->bcd[4][3] = 0;
    conv->bcd[5][0] = 0; conv->bcd[5][1] = 0; conv->bcd[5][2] = 0; conv->bcd[5][3] = 0;
    conv->bcd[6][0] = 0; conv->bcd[6][1] = 0; conv->bcd[6][2] = 0; conv->bcd[6][3] = 0;
    conv->bcd[7][0] = 0; conv->bcd[7][1] = 0; conv->bcd[7][2] = 0; conv->bcd[7][3] = 0;
    conv->bcd[8][0] = 0; conv->bcd[8][1] = 0; conv->bcd[8][2] = 0; conv->bcd[8][3] = 0;
    conv->bcd[9][0] = 0; conv->bcd[9][1] = 0; conv->bcd[9][2] = 0; conv->bcd[9][3] = 0;
}

void bin_to_bcd_set_input(bin_to_bcd_t *conv, bit input[32]) {
    conv->input[0] = input[0]; conv->input[1] = input[1];
    conv->input[2] = input[2]; conv->input[3] = input[3];
    conv->input[4] = input[4]; conv->input[5] = input[5];
    conv->input[6] = input[6]; conv->input[7] = input[7];
    conv->input[8] = input[8]; conv->input[9] = input[9];
    conv->input[10] = input[10]; conv->input[11] = input[11];
    conv->input[12] = input[12]; conv->input[13] = input[13];
    conv->input[14] = input[14]; conv->input[15] = input[15];
    conv->input[16] = input[16]; conv->input[17] = input[17];
    conv->input[18] = input[18]; conv->input[19] = input[19];
    conv->input[20] = input[20]; conv->input[21] = input[21];
    conv->input[22] = input[22]; conv->input[23] = input[23];
    conv->input[24] = input[24]; conv->input[25] = input[25];
    conv->input[26] = input[26]; conv->input[27] = input[27];
    conv->input[28] = input[28]; conv->input[29] = input[29];
    conv->input[30] = input[30]; conv->input[31] = input[31];
}

bit bin_to_bcd_digit_gte5(bit d[4]) {
    return OR(d[3], AND(d[2], OR(d[1], d[0])));
}

void bin_to_bcd_add3_if_gte5(bit d[4], bit result[4]) {
    bit gte5 = bin_to_bcd_digit_gte5(d);

    bit three[4] = {1, 1, 0, 0};
    bit sum[4];
    bit carry_out;

    add_4bit(d, three, 0, sum, &carry_out);

    result[0] = mux2(d[0], sum[0], gte5);
    result[1] = mux2(d[1], sum[1], gte5);
    result[2] = mux2(d[2], sum[2], gte5);
    result[3] = mux2(d[3], sum[3], gte5);
}

static void copy_shift_reg_to_bcd(bin_to_bcd_t *conv) {
    conv->bcd[0][0] = conv->shift_reg[32];
    conv->bcd[0][1] = conv->shift_reg[33];
    conv->bcd[0][2] = conv->shift_reg[34];
    conv->bcd[0][3] = conv->shift_reg[35];

    conv->bcd[1][0] = conv->shift_reg[36];
    conv->bcd[1][1] = conv->shift_reg[37];
    conv->bcd[1][2] = conv->shift_reg[38];
    conv->bcd[1][3] = conv->shift_reg[39];

    conv->bcd[2][0] = conv->shift_reg[40];
    conv->bcd[2][1] = conv->shift_reg[41];
    conv->bcd[2][2] = conv->shift_reg[42];
    conv->bcd[2][3] = conv->shift_reg[43];

    conv->bcd[3][0] = conv->shift_reg[44];
    conv->bcd[3][1] = conv->shift_reg[45];
    conv->bcd[3][2] = conv->shift_reg[46];
    conv->bcd[3][3] = conv->shift_reg[47];

    conv->bcd[4][0] = conv->shift_reg[48];
    conv->bcd[4][1] = conv->shift_reg[49];
    conv->bcd[4][2] = conv->shift_reg[50];
    conv->bcd[4][3] = conv->shift_reg[51];

    conv->bcd[5][0] = conv->shift_reg[52];
    conv->bcd[5][1] = conv->shift_reg[53];
    conv->bcd[5][2] = conv->shift_reg[54];
    conv->bcd[5][3] = conv->shift_reg[55];

    conv->bcd[6][0] = conv->shift_reg[56];
    conv->bcd[6][1] = conv->shift_reg[57];
    conv->bcd[6][2] = conv->shift_reg[58];
    conv->bcd[6][3] = conv->shift_reg[59];

    conv->bcd[7][0] = conv->shift_reg[60];
    conv->bcd[7][1] = conv->shift_reg[61];
    conv->bcd[7][2] = conv->shift_reg[62];
    conv->bcd[7][3] = conv->shift_reg[63];

    conv->bcd[8][0] = conv->shift_reg[64];
    conv->bcd[8][1] = conv->shift_reg[65];
    conv->bcd[8][2] = conv->shift_reg[66];
    conv->bcd[8][3] = conv->shift_reg[67];

    conv->bcd[9][0] = conv->shift_reg[68];
    conv->bcd[9][1] = conv->shift_reg[69];
    conv->bcd[9][2] = conv->shift_reg[70];
    conv->bcd[9][3] = conv->shift_reg[71];
}

void bin_to_bcd_shift_once(bin_to_bcd_t *conv) {
    bit d0[4], d1[4], d2[4], d3[4], d4[4];
    bit d5[4], d6[4], d7[4], d8[4], d9[4];
    bit r0[4], r1[4], r2[4], r3[4], r4[4];
    bit r5[4], r6[4], r7[4], r8[4], r9[4];

    d0[0] = conv->shift_reg[32]; d0[1] = conv->shift_reg[33];
    d0[2] = conv->shift_reg[34]; d0[3] = conv->shift_reg[35];
    d1[0] = conv->shift_reg[36]; d1[1] = conv->shift_reg[37];
    d1[2] = conv->shift_reg[38]; d1[3] = conv->shift_reg[39];
    d2[0] = conv->shift_reg[40]; d2[1] = conv->shift_reg[41];
    d2[2] = conv->shift_reg[42]; d2[3] = conv->shift_reg[43];
    d3[0] = conv->shift_reg[44]; d3[1] = conv->shift_reg[45];
    d3[2] = conv->shift_reg[46]; d3[3] = conv->shift_reg[47];
    d4[0] = conv->shift_reg[48]; d4[1] = conv->shift_reg[49];
    d4[2] = conv->shift_reg[50]; d4[3] = conv->shift_reg[51];
    d5[0] = conv->shift_reg[52]; d5[1] = conv->shift_reg[53];
    d5[2] = conv->shift_reg[54]; d5[3] = conv->shift_reg[55];
    d6[0] = conv->shift_reg[56]; d6[1] = conv->shift_reg[57];
    d6[2] = conv->shift_reg[58]; d6[3] = conv->shift_reg[59];
    d7[0] = conv->shift_reg[60]; d7[1] = conv->shift_reg[61];
    d7[2] = conv->shift_reg[62]; d7[3] = conv->shift_reg[63];
    d8[0] = conv->shift_reg[64]; d8[1] = conv->shift_reg[65];
    d8[2] = conv->shift_reg[66]; d8[3] = conv->shift_reg[67];
    d9[0] = conv->shift_reg[68]; d9[1] = conv->shift_reg[69];
    d9[2] = conv->shift_reg[70]; d9[3] = conv->shift_reg[71];

    bin_to_bcd_add3_if_gte5(d0, r0);
    bin_to_bcd_add3_if_gte5(d1, r1);
    bin_to_bcd_add3_if_gte5(d2, r2);
    bin_to_bcd_add3_if_gte5(d3, r3);
    bin_to_bcd_add3_if_gte5(d4, r4);
    bin_to_bcd_add3_if_gte5(d5, r5);
    bin_to_bcd_add3_if_gte5(d6, r6);
    bin_to_bcd_add3_if_gte5(d7, r7);
    bin_to_bcd_add3_if_gte5(d8, r8);
    bin_to_bcd_add3_if_gte5(d9, r9);

    conv->shift_reg[32] = r0[0]; conv->shift_reg[33] = r0[1];
    conv->shift_reg[34] = r0[2]; conv->shift_reg[35] = r0[3];
    conv->shift_reg[36] = r1[0]; conv->shift_reg[37] = r1[1];
    conv->shift_reg[38] = r1[2]; conv->shift_reg[39] = r1[3];
    conv->shift_reg[40] = r2[0]; conv->shift_reg[41] = r2[1];
    conv->shift_reg[42] = r2[2]; conv->shift_reg[43] = r2[3];
    conv->shift_reg[44] = r3[0]; conv->shift_reg[45] = r3[1];
    conv->shift_reg[46] = r3[2]; conv->shift_reg[47] = r3[3];
    conv->shift_reg[48] = r4[0]; conv->shift_reg[49] = r4[1];
    conv->shift_reg[50] = r4[2]; conv->shift_reg[51] = r4[3];
    conv->shift_reg[52] = r5[0]; conv->shift_reg[53] = r5[1];
    conv->shift_reg[54] = r5[2]; conv->shift_reg[55] = r5[3];
    conv->shift_reg[56] = r6[0]; conv->shift_reg[57] = r6[1];
    conv->shift_reg[58] = r6[2]; conv->shift_reg[59] = r6[3];
    conv->shift_reg[60] = r7[0]; conv->shift_reg[61] = r7[1];
    conv->shift_reg[62] = r7[2]; conv->shift_reg[63] = r7[3];
    conv->shift_reg[64] = r8[0]; conv->shift_reg[65] = r8[1];
    conv->shift_reg[66] = r8[2]; conv->shift_reg[67] = r8[3];
    conv->shift_reg[68] = r9[0]; conv->shift_reg[69] = r9[1];
    conv->shift_reg[70] = r9[2]; conv->shift_reg[71] = r9[3];

    bit new_reg[72];

    new_reg[71] = conv->shift_reg[70];
    new_reg[70] = conv->shift_reg[69];
    new_reg[69] = conv->shift_reg[68];
    new_reg[68] = conv->shift_reg[67];
    new_reg[67] = conv->shift_reg[66];
    new_reg[66] = conv->shift_reg[65];
    new_reg[65] = conv->shift_reg[64];
    new_reg[64] = conv->shift_reg[63];
    new_reg[63] = conv->shift_reg[62];
    new_reg[62] = conv->shift_reg[61];
    new_reg[61] = conv->shift_reg[60];
    new_reg[60] = conv->shift_reg[59];
    new_reg[59] = conv->shift_reg[58];
    new_reg[58] = conv->shift_reg[57];
    new_reg[57] = conv->shift_reg[56];
    new_reg[56] = conv->shift_reg[55];
    new_reg[55] = conv->shift_reg[54];
    new_reg[54] = conv->shift_reg[53];
    new_reg[53] = conv->shift_reg[52];
    new_reg[52] = conv->shift_reg[51];
    new_reg[51] = conv->shift_reg[50];
    new_reg[50] = conv->shift_reg[49];
    new_reg[49] = conv->shift_reg[48];
    new_reg[48] = conv->shift_reg[47];
    new_reg[47] = conv->shift_reg[46];
    new_reg[46] = conv->shift_reg[45];
    new_reg[45] = conv->shift_reg[44];
    new_reg[44] = conv->shift_reg[43];
    new_reg[43] = conv->shift_reg[42];
    new_reg[42] = conv->shift_reg[41];
    new_reg[41] = conv->shift_reg[40];
    new_reg[40] = conv->shift_reg[39];
    new_reg[39] = conv->shift_reg[38];
    new_reg[38] = conv->shift_reg[37];
    new_reg[37] = conv->shift_reg[36];
    new_reg[36] = conv->shift_reg[35];
    new_reg[35] = conv->shift_reg[34];
    new_reg[34] = conv->shift_reg[33];
    new_reg[33] = conv->shift_reg[32];
    new_reg[32] = conv->shift_reg[31];
    new_reg[31] = conv->shift_reg[30];
    new_reg[30] = conv->shift_reg[29];
    new_reg[29] = conv->shift_reg[28];
    new_reg[28] = conv->shift_reg[27];
    new_reg[27] = conv->shift_reg[26];
    new_reg[26] = conv->shift_reg[25];
    new_reg[25] = conv->shift_reg[24];
    new_reg[24] = conv->shift_reg[23];
    new_reg[23] = conv->shift_reg[22];
    new_reg[22] = conv->shift_reg[21];
    new_reg[21] = conv->shift_reg[20];
    new_reg[20] = conv->shift_reg[19];
    new_reg[19] = conv->shift_reg[18];
    new_reg[18] = conv->shift_reg[17];
    new_reg[17] = conv->shift_reg[16];
    new_reg[16] = conv->shift_reg[15];
    new_reg[15] = conv->shift_reg[14];
    new_reg[14] = conv->shift_reg[13];
    new_reg[13] = conv->shift_reg[12];
    new_reg[12] = conv->shift_reg[11];
    new_reg[11] = conv->shift_reg[10];
    new_reg[10] = conv->shift_reg[9];
    new_reg[9] = conv->shift_reg[8];
    new_reg[8] = conv->shift_reg[7];
    new_reg[7] = conv->shift_reg[6];
    new_reg[6] = conv->shift_reg[5];
    new_reg[5] = conv->shift_reg[4];
    new_reg[4] = conv->shift_reg[3];
    new_reg[3] = conv->shift_reg[2];
    new_reg[2] = conv->shift_reg[1];
    new_reg[1] = conv->shift_reg[0];
    new_reg[0] = 0;

    conv->shift_reg[0] = new_reg[0]; conv->shift_reg[1] = new_reg[1];
    conv->shift_reg[2] = new_reg[2]; conv->shift_reg[3] = new_reg[3];
    conv->shift_reg[4] = new_reg[4]; conv->shift_reg[5] = new_reg[5];
    conv->shift_reg[6] = new_reg[6]; conv->shift_reg[7] = new_reg[7];
    conv->shift_reg[8] = new_reg[8]; conv->shift_reg[9] = new_reg[9];
    conv->shift_reg[10] = new_reg[10]; conv->shift_reg[11] = new_reg[11];
    conv->shift_reg[12] = new_reg[12]; conv->shift_reg[13] = new_reg[13];
    conv->shift_reg[14] = new_reg[14]; conv->shift_reg[15] = new_reg[15];
    conv->shift_reg[16] = new_reg[16]; conv->shift_reg[17] = new_reg[17];
    conv->shift_reg[18] = new_reg[18]; conv->shift_reg[19] = new_reg[19];
    conv->shift_reg[20] = new_reg[20]; conv->shift_reg[21] = new_reg[21];
    conv->shift_reg[22] = new_reg[22]; conv->shift_reg[23] = new_reg[23];
    conv->shift_reg[24] = new_reg[24]; conv->shift_reg[25] = new_reg[25];
    conv->shift_reg[26] = new_reg[26]; conv->shift_reg[27] = new_reg[27];
    conv->shift_reg[28] = new_reg[28]; conv->shift_reg[29] = new_reg[29];
    conv->shift_reg[30] = new_reg[30]; conv->shift_reg[31] = new_reg[31];
    conv->shift_reg[32] = new_reg[32]; conv->shift_reg[33] = new_reg[33];
    conv->shift_reg[34] = new_reg[34]; conv->shift_reg[35] = new_reg[35];
    conv->shift_reg[36] = new_reg[36]; conv->shift_reg[37] = new_reg[37];
    conv->shift_reg[38] = new_reg[38]; conv->shift_reg[39] = new_reg[39];
    conv->shift_reg[40] = new_reg[40]; conv->shift_reg[41] = new_reg[41];
    conv->shift_reg[42] = new_reg[42]; conv->shift_reg[43] = new_reg[43];
    conv->shift_reg[44] = new_reg[44]; conv->shift_reg[45] = new_reg[45];
    conv->shift_reg[46] = new_reg[46]; conv->shift_reg[47] = new_reg[47];
    conv->shift_reg[48] = new_reg[48]; conv->shift_reg[49] = new_reg[49];
    conv->shift_reg[50] = new_reg[50]; conv->shift_reg[51] = new_reg[51];
    conv->shift_reg[52] = new_reg[52]; conv->shift_reg[53] = new_reg[53];
    conv->shift_reg[54] = new_reg[54]; conv->shift_reg[55] = new_reg[55];
    conv->shift_reg[56] = new_reg[56]; conv->shift_reg[57] = new_reg[57];
    conv->shift_reg[58] = new_reg[58]; conv->shift_reg[59] = new_reg[59];
    conv->shift_reg[60] = new_reg[60]; conv->shift_reg[61] = new_reg[61];
    conv->shift_reg[62] = new_reg[62]; conv->shift_reg[63] = new_reg[63];
    conv->shift_reg[64] = new_reg[64]; conv->shift_reg[65] = new_reg[65];
    conv->shift_reg[66] = new_reg[66]; conv->shift_reg[67] = new_reg[67];
    conv->shift_reg[68] = new_reg[68]; conv->shift_reg[69] = new_reg[69];
    conv->shift_reg[70] = new_reg[70]; conv->shift_reg[71] = new_reg[71];
}

void bin_to_bcd_convert(bin_to_bcd_t *conv) {
    conv->shift_reg[0] = conv->input[0]; conv->shift_reg[1] = conv->input[1];
    conv->shift_reg[2] = conv->input[2]; conv->shift_reg[3] = conv->input[3];
    conv->shift_reg[4] = conv->input[4]; conv->shift_reg[5] = conv->input[5];
    conv->shift_reg[6] = conv->input[6]; conv->shift_reg[7] = conv->input[7];
    conv->shift_reg[8] = conv->input[8]; conv->shift_reg[9] = conv->input[9];
    conv->shift_reg[10] = conv->input[10]; conv->shift_reg[11] = conv->input[11];
    conv->shift_reg[12] = conv->input[12]; conv->shift_reg[13] = conv->input[13];
    conv->shift_reg[14] = conv->input[14]; conv->shift_reg[15] = conv->input[15];
    conv->shift_reg[16] = conv->input[16]; conv->shift_reg[17] = conv->input[17];
    conv->shift_reg[18] = conv->input[18]; conv->shift_reg[19] = conv->input[19];
    conv->shift_reg[20] = conv->input[20]; conv->shift_reg[21] = conv->input[21];
    conv->shift_reg[22] = conv->input[22]; conv->shift_reg[23] = conv->input[23];
    conv->shift_reg[24] = conv->input[24]; conv->shift_reg[25] = conv->input[25];
    conv->shift_reg[26] = conv->input[26]; conv->shift_reg[27] = conv->input[27];
    conv->shift_reg[28] = conv->input[28]; conv->shift_reg[29] = conv->input[29];
    conv->shift_reg[30] = conv->input[30]; conv->shift_reg[31] = conv->input[31];

    conv->shift_reg[32] = 0; conv->shift_reg[33] = 0;
    conv->shift_reg[34] = 0; conv->shift_reg[35] = 0;
    conv->shift_reg[36] = 0; conv->shift_reg[37] = 0;
    conv->shift_reg[38] = 0; conv->shift_reg[39] = 0;
    conv->shift_reg[40] = 0; conv->shift_reg[41] = 0;
    conv->shift_reg[42] = 0; conv->shift_reg[43] = 0;
    conv->shift_reg[44] = 0; conv->shift_reg[45] = 0;
    conv->shift_reg[46] = 0; conv->shift_reg[47] = 0;
    conv->shift_reg[48] = 0; conv->shift_reg[49] = 0;
    conv->shift_reg[50] = 0; conv->shift_reg[51] = 0;
    conv->shift_reg[52] = 0; conv->shift_reg[53] = 0;
    conv->shift_reg[54] = 0; conv->shift_reg[55] = 0;
    conv->shift_reg[56] = 0; conv->shift_reg[57] = 0;
    conv->shift_reg[58] = 0; conv->shift_reg[59] = 0;
    conv->shift_reg[60] = 0; conv->shift_reg[61] = 0;
    conv->shift_reg[62] = 0; conv->shift_reg[63] = 0;
    conv->shift_reg[64] = 0; conv->shift_reg[65] = 0;
    conv->shift_reg[66] = 0; conv->shift_reg[67] = 0;
    conv->shift_reg[68] = 0; conv->shift_reg[69] = 0;
    conv->shift_reg[70] = 0; conv->shift_reg[71] = 0;

    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);
    bin_to_bcd_shift_once(conv);

    copy_shift_reg_to_bcd(conv);
}
