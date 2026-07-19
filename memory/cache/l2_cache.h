#ifndef ALETHEIA_L2_CACHE_H
#define ALETHEIA_L2_CACHE_H

#include "../../core/types.h"
#include "cache_line.h"

#define L2_LINES 8

typedef struct {
    cache_line_t lines[L2_LINES];

    bit addr[32];
    bit data_in[32];
    bit write;

    bit hit;
    bit data_out[32];
} l2_cache_t;

void l2_init(l2_cache_t *c);
void l2_reset(l2_cache_t *c);
void l2_set_addr(l2_cache_t *c, bit *addr);
void l2_set_data(l2_cache_t *c, bit *data);
void l2_set_write(l2_cache_t *c, bit write);
void l2_clock_fall(l2_cache_t *c);
void l2_clock_rise(l2_cache_t *c);
void l2_propagate(l2_cache_t *c);

#endif
