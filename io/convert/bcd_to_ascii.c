#include "bcd_to_ascii.h"
#include "../../core/gates/gates.h"

void bcd_digit_to_ascii(bit bcd[4], bit ascii[8]) {
    ascii[0] = bcd[0];
    ascii[1] = bcd[1];
    ascii[2] = bcd[2];
    ascii[3] = bcd[3];
    ascii[4] = 1;
    ascii[5] = 1;
    ascii[6] = 0;
    ascii[7] = 0;
}

void bcd_to_ascii_string(bit bcd[10][4], bit ascii[10][8]) {
    bcd_digit_to_ascii(bcd[0], ascii[0]);
    bcd_digit_to_ascii(bcd[1], ascii[1]);
    bcd_digit_to_ascii(bcd[2], ascii[2]);
    bcd_digit_to_ascii(bcd[3], ascii[3]);
    bcd_digit_to_ascii(bcd[4], ascii[4]);
    bcd_digit_to_ascii(bcd[5], ascii[5]);
    bcd_digit_to_ascii(bcd[6], ascii[6]);
    bcd_digit_to_ascii(bcd[7], ascii[7]);
    bcd_digit_to_ascii(bcd[8], ascii[8]);
    bcd_digit_to_ascii(bcd[9], ascii[9]);
}
