#ifndef ALETHEIA_L1_CACHE_H
#define ALETHEIA_L1_CACHE_H

#include "../../core/types.h"
#include "cache_line.h"

#define L1_LINES 4

typedef struct {
    cache_line_t lines[L1_LINES];

    bit addr[32];
    bit data_in[32];
    bit write;

    bit hit;
    bit data_out[32];
} l1_cache_t;

void l1_init(l1_cache_t *c);
void l1_reset(l1_cache_t *c);
void l1_set_addr(l1_cache_t *c, bit *addr);
void l1_set_data(l1_cache_t *c, bit *data);
void l1_set_write(l1_cache_t *c, bit write);
void l1_clock_fall(l1_cache_t *c);
void l1_clock_rise(l1_cache_t *c);
void l1_propagate(l1_cache_t *c);

#endif
