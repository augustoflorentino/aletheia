#ifndef ALETHEIA_NUMBER_PARSER_H
#define ALETHEIA_NUMBER_PARSER_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"
#include "../../cpu/registers/register.h"

typedef struct {
    register_32_t acc;
    d_flip_flop_t done;

    bit byte_in[8];
    bit feed;

    bit value[32];
    bit done_out;
} number_parser_t;

void number_parser_init(number_parser_t *p);
void number_parser_set_byte(number_parser_t *p, bit *byte);
void number_parser_set_feed(number_parser_t *p, bit feed);
void number_parser_clock_fall(number_parser_t *p);
void number_parser_clock_rise(number_parser_t *p);
void number_parser_propagate(number_parser_t *p);

#endif
