#include "ascii_to_bcd.h"
#include "../../core/gates/gates.h"

void ascii_digit_to_bcd(bit ascii[8], bit bcd[4]) {
    bcd[0] = ascii[0];
    bcd[1] = ascii[1];
    bcd[2] = ascii[2];
    bcd[3] = ascii[3];
}

void ascii_to_bcd_string(bit ascii[10][8], bit bcd[10][4]) {
    ascii_digit_to_bcd(ascii[0], bcd[0]);
    ascii_digit_to_bcd(ascii[1], bcd[1]);
    ascii_digit_to_bcd(ascii[2], bcd[2]);
    ascii_digit_to_bcd(ascii[3], bcd[3]);
    ascii_digit_to_bcd(ascii[4], bcd[4]);
    ascii_digit_to_bcd(ascii[5], bcd[5]);
    ascii_digit_to_bcd(ascii[6], bcd[6]);
    ascii_digit_to_bcd(ascii[7], bcd[7]);
    ascii_digit_to_bcd(ascii[8], bcd[8]);
    ascii_digit_to_bcd(ascii[9], bcd[9]);
}

bit ascii_is_digit(bit ascii[8]) {
    bit high_nibble_30 = AND(AND(NOT(ascii[7]), NOT(ascii[6])), AND(ascii[5], ascii[4]));

    bit valid_low = NOT(AND(ascii[3], OR(ascii[2], ascii[1])));

    return AND(high_nibble_30, valid_low);
}
