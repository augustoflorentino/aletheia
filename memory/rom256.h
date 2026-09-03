#ifndef ALETHEIA_ROM256_H
#define ALETHEIA_ROM256_H

#include "../core/types.h"
#include "rom.h"

#define ROM256_SIZE 256
#define ROM256_CHIPS 16

typedef struct {
    rom_16x32_t chips[ROM256_CHIPS];
    bit addr[8];
    bit data_out[32];
    bit chip_select;
} rom_256x32_t;

void rom256_init(rom_256x32_t *rom);
void rom256_reset(rom_256x32_t *rom);
void rom256_program(rom_256x32_t *rom, bit *addr, bit en, bit *data);
void rom256_set_addr(rom_256x32_t *rom, bit *addr);
void rom256_set_cs(rom_256x32_t *rom, bit cs);
void rom256_propagate(rom_256x32_t *rom);
void rom256_read(rom_256x32_t *rom, bit *out);

#endif
