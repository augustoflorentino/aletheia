#ifndef ALETHEIA_ROM_H
#define ALETHEIA_ROM_H

#include "../core/types.h"

#define ROM_SIZE 16

typedef struct {
    bit data[ROM_SIZE][32];
    bit addr[4];
    bit data_out[32];
    bit chip_select;
} rom_16x32_t;

void rom_init(rom_16x32_t *rom);
void rom_reset(rom_16x32_t *rom);
void rom_load_word(rom_16x32_t *rom, int index, bit *data);
void rom_set_addr(rom_16x32_t *rom, bit *addr);
void rom_set_cs(rom_16x32_t *rom, bit cs);
void rom_propagate(rom_16x32_t *rom);
void rom_read(rom_16x32_t *rom, bit *out);

#endif
