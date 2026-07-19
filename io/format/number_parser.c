#include "number_parser.h"
#include "../../core/gates/gates.h"
#include "../convert/ascii_to_bcd.h"
#include "../convert/bcd_to_bin.h"

void number_parser_init(number_parser_t *p) {
    reg32_init(&p->acc);
    reg32_reset(&p->acc);
    dff_reset(&p->done);
    p->byte_in[0] = 0; p->byte_in[1] = 0; p->byte_in[2] = 0; p->byte_in[3] = 0;
    p->byte_in[4] = 0; p->byte_in[5] = 0; p->byte_in[6] = 0; p->byte_in[7] = 0;
    p->feed = 0;
    p->done_out = 0;
}

void number_parser_set_byte(number_parser_t *p, bit *byte) {
    p->byte_in[0] = byte[0]; p->byte_in[1] = byte[1];
    p->byte_in[2] = byte[2]; p->byte_in[3] = byte[3];
    p->byte_in[4] = byte[4]; p->byte_in[5] = byte[5];
    p->byte_in[6] = byte[6]; p->byte_in[7] = byte[7];
}

void number_parser_set_feed(number_parser_t *p, bit feed) {
    p->feed = feed;
}

void number_parser_clock_fall(number_parser_t *p) {
    reg32_propagate(&p->acc);

    bit is_digit = ascii_is_digit(p->byte_in);
    bit digit[4];
    ascii_digit_to_bcd(p->byte_in, digit);

    bit acc10[32];
    mul_by_10_32(p->acc.data_out, acc10);
    bit acc_next[32];
    add_digit_32(acc10, digit, acc_next);

    reg32_set_input(&p->acc, acc_next);
    reg32_set_load(&p->acc, AND(p->feed, is_digit));

    p->done.d = OR(p->done.q, AND(p->feed, NOT(is_digit)));

    reg32_clock_fall(&p->acc);
    dff_clock_fall(&p->done);
}

void number_parser_clock_rise(number_parser_t *p) {
    reg32_clock_rise(&p->acc);
    dff_clock_rise(&p->done);
}

void number_parser_propagate(number_parser_t *p) {
    reg32_propagate(&p->acc);
    p->value[0] = p->acc.data_out[0]; p->value[1] = p->acc.data_out[1];
    p->value[2] = p->acc.data_out[2]; p->value[3] = p->acc.data_out[3];
    p->value[4] = p->acc.data_out[4]; p->value[5] = p->acc.data_out[5];
    p->value[6] = p->acc.data_out[6]; p->value[7] = p->acc.data_out[7];
    p->value[8] = p->acc.data_out[8]; p->value[9] = p->acc.data_out[9];
    p->value[10] = p->acc.data_out[10]; p->value[11] = p->acc.data_out[11];
    p->value[12] = p->acc.data_out[12]; p->value[13] = p->acc.data_out[13];
    p->value[14] = p->acc.data_out[14]; p->value[15] = p->acc.data_out[15];
    p->value[16] = p->acc.data_out[16]; p->value[17] = p->acc.data_out[17];
    p->value[18] = p->acc.data_out[18]; p->value[19] = p->acc.data_out[19];
    p->value[20] = p->acc.data_out[20]; p->value[21] = p->acc.data_out[21];
    p->value[22] = p->acc.data_out[22]; p->value[23] = p->acc.data_out[23];
    p->value[24] = p->acc.data_out[24]; p->value[25] = p->acc.data_out[25];
    p->value[26] = p->acc.data_out[26]; p->value[27] = p->acc.data_out[27];
    p->value[28] = p->acc.data_out[28]; p->value[29] = p->acc.data_out[29];
    p->value[30] = p->acc.data_out[30]; p->value[31] = p->acc.data_out[31];
    p->done_out = p->done.q;
}
