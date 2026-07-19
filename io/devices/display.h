#ifndef ALETHEIA_DISPLAY_H
#define ALETHEIA_DISPLAY_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"

#define DISPLAY_CHARS 8

typedef struct {
    d_flip_flop_t chars[DISPLAY_CHARS][8];

    bit addr[3];
    bit data_in[8];
    bit write;

    bit data_out[8];
} display_t;

void display_init(display_t *d);
void display_reset(display_t *d);
void display_set_addr(display_t *d, bit *addr);
void display_set_data(display_t *d, bit *data);
void display_set_write(display_t *d, bit write);
void display_clock_fall(display_t *d);
void display_clock_rise(display_t *d);
void display_propagate(display_t *d);

#endif
