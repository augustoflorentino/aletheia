#ifndef ALETHEIA_SRAM4K_H
#define ALETHEIA_SRAM4K_H

#include "../core/types.h"
#include "sram1k.h"

typedef struct {
    sram_1kx32_t banks[4];
    bit addr[12];
    bit data_in[32];
    bit data_out[32];
    bit write_enable;
    bit chip_select;
} sram_4kx32_t;

void sram4k_init(sram_4kx32_t *ram);
void sram4k_reset(sram_4kx32_t *ram);
void sram4k_set_addr(sram_4kx32_t *ram, bit *addr);
void sram4k_set_data(sram_4kx32_t *ram, bit *data);
void sram4k_set_write(sram_4kx32_t *ram, bit we);
void sram4k_set_cs(sram_4kx32_t *ram, bit cs);
void sram4k_clock_rise(sram_4kx32_t *ram);
void sram4k_clock_fall(sram_4kx32_t *ram);
void sram4k_propagate(sram_4kx32_t *ram);
void sram4k_read(sram_4kx32_t *ram, bit *out);

#endif
