#ifndef ALETHEIA_DRAM_H
#define ALETHEIA_DRAM_H

#include "../core/types.h"
#include "cell.h"
#include "../core/sequential/d_flip_flop.h"

#define DRAM_SIZE 16

typedef struct {
    memory_cell_32_t cells[DRAM_SIZE];
    d_flip_flop_t fresh[DRAM_SIZE];
    d_flip_flop_t refreshed[DRAM_SIZE];

    bit addr[4];
    bit data_in[32];
    bit write;
    bit refresh;
    bit leak;

    bit data_out[32];
} dram_16x32_t;

void dram_init(dram_16x32_t *d);
void dram_reset(dram_16x32_t *d);
void dram_set_addr(dram_16x32_t *d, bit *addr);
void dram_set_data(dram_16x32_t *d, bit *data);
void dram_set_write(dram_16x32_t *d, bit we);
void dram_set_refresh(dram_16x32_t *d, bit r);
void dram_set_leak(dram_16x32_t *d, bit l);
void dram_clock_fall(dram_16x32_t *d);
void dram_clock_rise(dram_16x32_t *d);
void dram_propagate(dram_16x32_t *d);

#endif
