#include "number_formatter.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"
#include "../../cpu/alu/subtractor.h"
#include "../convert/bin_to_bcd.h"
#include "../convert/bcd_to_ascii.h"

static bit digit_is_zero(bit d[4]) {
    return AND(AND(NOT(d[0]), NOT(d[1])), AND(NOT(d[2]), NOT(d[3])));
}

void number_format(bit input[32], bit ascii[NF_DIGITS][8]) {
    bin_to_bcd_t conv;
    bin_to_bcd_init(&conv);
    bin_to_bcd_set_input(&conv, input);
    bin_to_bcd_convert(&conv);

    bit dascii[NF_DIGITS][8];
    bcd_digit_to_ascii(conv.bcd[0], dascii[0]);
    bcd_digit_to_ascii(conv.bcd[1], dascii[1]);
    bcd_digit_to_ascii(conv.bcd[2], dascii[2]);
    bcd_digit_to_ascii(conv.bcd[3], dascii[3]);
    bcd_digit_to_ascii(conv.bcd[4], dascii[4]);
    bcd_digit_to_ascii(conv.bcd[5], dascii[5]);
    bcd_digit_to_ascii(conv.bcd[6], dascii[6]);
    bcd_digit_to_ascii(conv.bcd[7], dascii[7]);
    bcd_digit_to_ascii(conv.bcd[8], dascii[8]);
    bcd_digit_to_ascii(conv.bcd[9], dascii[9]);

    bit z9 = digit_is_zero(conv.bcd[9]);
    bit z8 = digit_is_zero(conv.bcd[8]);
    bit z7 = digit_is_zero(conv.bcd[7]);
    bit z6 = digit_is_zero(conv.bcd[6]);
    bit z5 = digit_is_zero(conv.bcd[5]);
    bit z4 = digit_is_zero(conv.bcd[4]);
    bit z3 = digit_is_zero(conv.bcd[3]);
    bit z2 = digit_is_zero(conv.bcd[2]);
    bit z1 = digit_is_zero(conv.bcd[1]);

    bit lead9 = z9;
    bit lead8 = AND(lead9, z8);
    bit lead7 = AND(lead8, z7);
    bit lead6 = AND(lead7, z6);
    bit lead5 = AND(lead6, z5);
    bit lead4 = AND(lead5, z4);
    bit lead3 = AND(lead4, z3);
    bit lead2 = AND(lead3, z2);
    bit lead1 = AND(lead2, z1);

    bit space[8] = {0, 0, 0, 0, 0, 1, 0, 0};

    ascii[0][0] = dascii[0][0]; ascii[0][1] = dascii[0][1];
    ascii[0][2] = dascii[0][2]; ascii[0][3] = dascii[0][3];
    ascii[0][4] = dascii[0][4]; ascii[0][5] = dascii[0][5];
    ascii[0][6] = dascii[0][6]; ascii[0][7] = dascii[0][7];

    mux2_8(dascii[1], space, lead1, ascii[1]);
    mux2_8(dascii[2], space, lead2, ascii[2]);
    mux2_8(dascii[3], space, lead3, ascii[3]);
    mux2_8(dascii[4], space, lead4, ascii[4]);
    mux2_8(dascii[5], space, lead5, ascii[5]);
    mux2_8(dascii[6], space, lead6, ascii[6]);
    mux2_8(dascii[7], space, lead7, ascii[7]);
    mux2_8(dascii[8], space, lead8, ascii[8]);
    mux2_8(dascii[9], space, lead9, ascii[9]);
}

void number_format_signed(bit input[32], bit ascii[NF_DIGITS][8]) {
    bit sign = input[31];
    bit neg[32];
    negate_32(input, neg);
    bit mag[32];
    mux2_32(input, neg, sign, mag);

    bin_to_bcd_t conv;
    bin_to_bcd_init(&conv);
    bin_to_bcd_set_input(&conv, mag);
    bin_to_bcd_convert(&conv);

    bit dascii[NF_DIGITS][8];
    bcd_digit_to_ascii(conv.bcd[0], dascii[0]);
    bcd_digit_to_ascii(conv.bcd[1], dascii[1]);
    bcd_digit_to_ascii(conv.bcd[2], dascii[2]);
    bcd_digit_to_ascii(conv.bcd[3], dascii[3]);
    bcd_digit_to_ascii(conv.bcd[4], dascii[4]);
    bcd_digit_to_ascii(conv.bcd[5], dascii[5]);
    bcd_digit_to_ascii(conv.bcd[6], dascii[6]);
    bcd_digit_to_ascii(conv.bcd[7], dascii[7]);
    bcd_digit_to_ascii(conv.bcd[8], dascii[8]);
    bcd_digit_to_ascii(conv.bcd[9], dascii[9]);

    bit z9 = digit_is_zero(conv.bcd[9]);
    bit z8 = digit_is_zero(conv.bcd[8]);
    bit z7 = digit_is_zero(conv.bcd[7]);
    bit z6 = digit_is_zero(conv.bcd[6]);
    bit z5 = digit_is_zero(conv.bcd[5]);
    bit z4 = digit_is_zero(conv.bcd[4]);
    bit z3 = digit_is_zero(conv.bcd[3]);
    bit z2 = digit_is_zero(conv.bcd[2]);
    bit z1 = digit_is_zero(conv.bcd[1]);

    bit lead0 = 0;
    bit lead9 = z9;
    bit lead8 = AND(lead9, z8);
    bit lead7 = AND(lead8, z7);
    bit lead6 = AND(lead7, z6);
    bit lead5 = AND(lead6, z5);
    bit lead4 = AND(lead5, z4);
    bit lead3 = AND(lead4, z3);
    bit lead2 = AND(lead3, z2);
    bit lead1 = AND(lead2, z1);

    bit slot1 = AND(lead1, NOT(lead0));
    bit slot2 = AND(lead2, NOT(lead1));
    bit slot3 = AND(lead3, NOT(lead2));
    bit slot4 = AND(lead4, NOT(lead3));
    bit slot5 = AND(lead5, NOT(lead4));
    bit slot6 = AND(lead6, NOT(lead5));
    bit slot7 = AND(lead7, NOT(lead6));
    bit slot8 = AND(lead8, NOT(lead7));
    bit slot9 = AND(lead9, NOT(lead8));

    bit space[8] = {0, 0, 0, 0, 0, 1, 0, 0};
    bit minus[8] = {1, 0, 1, 1, 0, 1, 0, 0};

    ascii[0][0] = dascii[0][0]; ascii[0][1] = dascii[0][1];
    ascii[0][2] = dascii[0][2]; ascii[0][3] = dascii[0][3];
    ascii[0][4] = dascii[0][4]; ascii[0][5] = dascii[0][5];
    ascii[0][6] = dascii[0][6]; ascii[0][7] = dascii[0][7];

    bit base1[8]; mux2_8(dascii[1], space, lead1, base1);
    mux2_8(base1, minus, AND(sign, slot1), ascii[1]);
    bit base2[8]; mux2_8(dascii[2], space, lead2, base2);
    mux2_8(base2, minus, AND(sign, slot2), ascii[2]);
    bit base3[8]; mux2_8(dascii[3], space, lead3, base3);
    mux2_8(base3, minus, AND(sign, slot3), ascii[3]);
    bit base4[8]; mux2_8(dascii[4], space, lead4, base4);
    mux2_8(base4, minus, AND(sign, slot4), ascii[4]);
    bit base5[8]; mux2_8(dascii[5], space, lead5, base5);
    mux2_8(base5, minus, AND(sign, slot5), ascii[5]);
    bit base6[8]; mux2_8(dascii[6], space, lead6, base6);
    mux2_8(base6, minus, AND(sign, slot6), ascii[6]);
    bit base7[8]; mux2_8(dascii[7], space, lead7, base7);
    mux2_8(base7, minus, AND(sign, slot7), ascii[7]);
    bit base8[8]; mux2_8(dascii[8], space, lead8, base8);
    mux2_8(base8, minus, AND(sign, slot8), ascii[8]);
    bit base9[8]; mux2_8(dascii[9], space, lead9, base9);
    mux2_8(base9, minus, AND(sign, slot9), ascii[9]);
}
