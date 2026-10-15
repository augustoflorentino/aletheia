#ifndef ALETHEIA_SRAM1K_H
#define ALETHEIA_SRAM1K_H

#include "../core/types.h"
#include "sram256.h"

typedef struct {
    sram_256x32_t banks[4];
    bit addr[10];
    bit data_in[32];
    bit data_out[32];
    bit write_enable;
    bit chip_select;
} sram_1kx32_t;

void sram1k_init(sram_1kx32_t *ram);
void sram1k_reset(sram_1kx32_t *ram);
void sram1k_set_addr(sram_1kx32_t *ram, bit *addr);
void sram1k_set_data(sram_1kx32_t *ram, bit *data);
void sram1k_set_write(sram_1kx32_t *ram, bit we);
void sram1k_set_cs(sram_1kx32_t *ram, bit cs);
void sram1k_clock_rise(sram_1kx32_t *ram);
void sram1k_clock_fall(sram_1kx32_t *ram);
void sram1k_propagate(sram_1kx32_t *ram);
void sram1k_read(sram_1kx32_t *ram, bit *out);

#endif
