#include "bcd_to_bin.h"
#include "../../core/gates/gates.h"
#include "../../cpu/alu/adder.h"

void bcd_to_bin_init(bcd_to_bin_t *conv) {
    bcd_to_bin_reset(conv);
}

void bcd_to_bin_reset(bcd_to_bin_t *conv) {
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

    conv->result[0] = 0; conv->result[1] = 0; conv->result[2] = 0; conv->result[3] = 0;
    conv->result[4] = 0; conv->result[5] = 0; conv->result[6] = 0; conv->result[7] = 0;
    conv->result[8] = 0; conv->result[9] = 0; conv->result[10] = 0; conv->result[11] = 0;
    conv->result[12] = 0; conv->result[13] = 0; conv->result[14] = 0; conv->result[15] = 0;
    conv->result[16] = 0; conv->result[17] = 0; conv->result[18] = 0; conv->result[19] = 0;
    conv->result[20] = 0; conv->result[21] = 0; conv->result[22] = 0; conv->result[23] = 0;
    conv->result[24] = 0; conv->result[25] = 0; conv->result[26] = 0; conv->result[27] = 0;
    conv->result[28] = 0; conv->result[29] = 0; conv->result[30] = 0; conv->result[31] = 0;
}

void bcd_to_bin_set_bcd(bcd_to_bin_t *conv, bit bcd[BCD_DIGITS][4]) {
    conv->bcd[0][0] = bcd[0][0]; conv->bcd[0][1] = bcd[0][1];
    conv->bcd[0][2] = bcd[0][2]; conv->bcd[0][3] = bcd[0][3];
    conv->bcd[1][0] = bcd[1][0]; conv->bcd[1][1] = bcd[1][1];
    conv->bcd[1][2] = bcd[1][2]; conv->bcd[1][3] = bcd[1][3];
    conv->bcd[2][0] = bcd[2][0]; conv->bcd[2][1] = bcd[2][1];
    conv->bcd[2][2] = bcd[2][2]; conv->bcd[2][3] = bcd[2][3];
    conv->bcd[3][0] = bcd[3][0]; conv->bcd[3][1] = bcd[3][1];
    conv->bcd[3][2] = bcd[3][2]; conv->bcd[3][3] = bcd[3][3];
    conv->bcd[4][0] = bcd[4][0]; conv->bcd[4][1] = bcd[4][1];
    conv->bcd[4][2] = bcd[4][2]; conv->bcd[4][3] = bcd[4][3];
    conv->bcd[5][0] = bcd[5][0]; conv->bcd[5][1] = bcd[5][1];
    conv->bcd[5][2] = bcd[5][2]; conv->bcd[5][3] = bcd[5][3];
    conv->bcd[6][0] = bcd[6][0]; conv->bcd[6][1] = bcd[6][1];
    conv->bcd[6][2] = bcd[6][2]; conv->bcd[6][3] = bcd[6][3];
    conv->bcd[7][0] = bcd[7][0]; conv->bcd[7][1] = bcd[7][1];
    conv->bcd[7][2] = bcd[7][2]; conv->bcd[7][3] = bcd[7][3];
    conv->bcd[8][0] = bcd[8][0]; conv->bcd[8][1] = bcd[8][1];
    conv->bcd[8][2] = bcd[8][2]; conv->bcd[8][3] = bcd[8][3];
    conv->bcd[9][0] = bcd[9][0]; conv->bcd[9][1] = bcd[9][1];
    conv->bcd[9][2] = bcd[9][2]; conv->bcd[9][3] = bcd[9][3];
}

void mul_by_10_32(bit input[32], bit output[32]) {
    bit shifted_3[32];
    bit shifted_1[32];
    bit carries[33];

    shifted_3[0] = 0; shifted_3[1] = 0; shifted_3[2] = 0;
    shifted_3[3] = input[0]; shifted_3[4] = input[1]; shifted_3[5] = input[2];
    shifted_3[6] = input[3]; shifted_3[7] = input[4]; shifted_3[8] = input[5];
    shifted_3[9] = input[6]; shifted_3[10] = input[7]; shifted_3[11] = input[8];
    shifted_3[12] = input[9]; shifted_3[13] = input[10]; shifted_3[14] = input[11];
    shifted_3[15] = input[12]; shifted_3[16] = input[13]; shifted_3[17] = input[14];
    shifted_3[18] = input[15]; shifted_3[19] = input[16]; shifted_3[20] = input[17];
    shifted_3[21] = input[18]; shifted_3[22] = input[19]; shifted_3[23] = input[20];
    shifted_3[24] = input[21]; shifted_3[25] = input[22]; shifted_3[26] = input[23];
    shifted_3[27] = input[24]; shifted_3[28] = input[25]; shifted_3[29] = input[26];
    shifted_3[30] = input[27]; shifted_3[31] = input[28];

    shifted_1[0] = 0;
    shifted_1[1] = input[0]; shifted_1[2] = input[1]; shifted_1[3] = input[2];
    shifted_1[4] = input[3]; shifted_1[5] = input[4]; shifted_1[6] = input[5];
    shifted_1[7] = input[6]; shifted_1[8] = input[7]; shifted_1[9] = input[8];
    shifted_1[10] = input[9]; shifted_1[11] = input[10]; shifted_1[12] = input[11];
    shifted_1[13] = input[12]; shifted_1[14] = input[13]; shifted_1[15] = input[14];
    shifted_1[16] = input[15]; shifted_1[17] = input[16]; shifted_1[18] = input[17];
    shifted_1[19] = input[18]; shifted_1[20] = input[19]; shifted_1[21] = input[20];
    shifted_1[22] = input[21]; shifted_1[23] = input[22]; shifted_1[24] = input[23];
    shifted_1[25] = input[24]; shifted_1[26] = input[25]; shifted_1[27] = input[26];
    shifted_1[28] = input[27]; shifted_1[29] = input[28]; shifted_1[30] = input[29];
    shifted_1[31] = input[30];

    adder_32(shifted_3, shifted_1, output, carries);
}

void add_digit_32(bit input[32], bit digit[4], bit output[32]) {
    bit digit_extended[32];
    bit carries[33];

    digit_extended[0] = digit[0]; digit_extended[1] = digit[1];
    digit_extended[2] = digit[2]; digit_extended[3] = digit[3];
    digit_extended[4] = 0; digit_extended[5] = 0; digit_extended[6] = 0; digit_extended[7] = 0;
    digit_extended[8] = 0; digit_extended[9] = 0; digit_extended[10] = 0; digit_extended[11] = 0;
    digit_extended[12] = 0; digit_extended[13] = 0; digit_extended[14] = 0; digit_extended[15] = 0;
    digit_extended[16] = 0; digit_extended[17] = 0; digit_extended[18] = 0; digit_extended[19] = 0;
    digit_extended[20] = 0; digit_extended[21] = 0; digit_extended[22] = 0; digit_extended[23] = 0;
    digit_extended[24] = 0; digit_extended[25] = 0; digit_extended[26] = 0; digit_extended[27] = 0;
    digit_extended[28] = 0; digit_extended[29] = 0; digit_extended[30] = 0; digit_extended[31] = 0;

    adder_32(input, digit_extended, output, carries);
}

static void process_digit(bit acc[32], bit digit[4], bit output[32]) {
    bit temp[32];
    mul_by_10_32(acc, temp);
    add_digit_32(temp, digit, output);
}

void bcd_to_bin_convert(bcd_to_bin_t *conv) {
    bit acc[32];
    bit t0[32], t1[32], t2[32], t3[32], t4[32];
    bit t5[32], t6[32], t7[32], t8[32];

    acc[0] = conv->bcd[9][0]; acc[1] = conv->bcd[9][1];
    acc[2] = conv->bcd[9][2]; acc[3] = conv->bcd[9][3];
    acc[4] = 0; acc[5] = 0; acc[6] = 0; acc[7] = 0;
    acc[8] = 0; acc[9] = 0; acc[10] = 0; acc[11] = 0;
    acc[12] = 0; acc[13] = 0; acc[14] = 0; acc[15] = 0;
    acc[16] = 0; acc[17] = 0; acc[18] = 0; acc[19] = 0;
    acc[20] = 0; acc[21] = 0; acc[22] = 0; acc[23] = 0;
    acc[24] = 0; acc[25] = 0; acc[26] = 0; acc[27] = 0;
    acc[28] = 0; acc[29] = 0; acc[30] = 0; acc[31] = 0;

    process_digit(acc, conv->bcd[8], t0);
    process_digit(t0, conv->bcd[7], t1);
    process_digit(t1, conv->bcd[6], t2);
    process_digit(t2, conv->bcd[5], t3);
    process_digit(t3, conv->bcd[4], t4);
    process_digit(t4, conv->bcd[3], t5);
    process_digit(t5, conv->bcd[2], t6);
    process_digit(t6, conv->bcd[1], t7);
    process_digit(t7, conv->bcd[0], t8);

    conv->result[0] = t8[0]; conv->result[1] = t8[1];
    conv->result[2] = t8[2]; conv->result[3] = t8[3];
    conv->result[4] = t8[4]; conv->result[5] = t8[5];
    conv->result[6] = t8[6]; conv->result[7] = t8[7];
    conv->result[8] = t8[8]; conv->result[9] = t8[9];
    conv->result[10] = t8[10]; conv->result[11] = t8[11];
    conv->result[12] = t8[12]; conv->result[13] = t8[13];
    conv->result[14] = t8[14]; conv->result[15] = t8[15];
    conv->result[16] = t8[16]; conv->result[17] = t8[17];
    conv->result[18] = t8[18]; conv->result[19] = t8[19];
    conv->result[20] = t8[20]; conv->result[21] = t8[21];
    conv->result[22] = t8[22]; conv->result[23] = t8[23];
    conv->result[24] = t8[24]; conv->result[25] = t8[25];
    conv->result[26] = t8[26]; conv->result[27] = t8[27];
    conv->result[28] = t8[28]; conv->result[29] = t8[29];
    conv->result[30] = t8[30]; conv->result[31] = t8[31];
}
