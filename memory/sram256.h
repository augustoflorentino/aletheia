#ifndef ALETHEIA_SRAM256_H
#define ALETHEIA_SRAM256_H

#include "../core/types.h"
#include "sram.h"

#define SRAM256_SIZE 256
#define SRAM256_BANKS 16

typedef struct {
    sram_16x32_t banks[SRAM256_BANKS];
    bit addr[8];
    bit data_in[32];
    bit data_out[32];
    bit write_enable;
    bit chip_select;
} sram_256x32_t;

void sram256_init(sram_256x32_t *ram);
void sram256_reset(sram_256x32_t *ram);
void sram256_set_addr(sram_256x32_t *ram, bit *addr);
void sram256_set_data(sram_256x32_t *ram, bit *data);
void sram256_set_write(sram_256x32_t *ram, bit we);
void sram256_set_cs(sram_256x32_t *ram, bit cs);
void sram256_clock_rise(sram_256x32_t *ram);
void sram256_clock_fall(sram_256x32_t *ram);
void sram256_propagate(sram_256x32_t *ram);
void sram256_read(sram_256x32_t *ram, bit *out);

#endif
