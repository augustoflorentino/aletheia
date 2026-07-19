#ifndef ALETHEIA_SRAM_H
#define ALETHEIA_SRAM_H

#include "../core/types.h"
#include "cell.h"

#define SRAM_SIZE 16

typedef struct {
    memory_cell_32_t cells[SRAM_SIZE];
    bit addr[4];
    bit data_in[32];
    bit data_out[32];
    bit write_enable;
    bit chip_select;
} sram_16x32_t;

void sram_init(sram_16x32_t *ram);
void sram_reset(sram_16x32_t *ram);
void sram_set_addr(sram_16x32_t *ram, bit *addr);
void sram_set_data(sram_16x32_t *ram, bit *data);
void sram_set_write(sram_16x32_t *ram, bit we);
void sram_set_cs(sram_16x32_t *ram, bit cs);
void sram_clock_rise(sram_16x32_t *ram);
void sram_clock_fall(sram_16x32_t *ram);
void sram_propagate(sram_16x32_t *ram);
void sram_read(sram_16x32_t *ram, bit *out);

#endif
