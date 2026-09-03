#ifndef ALETHEIA_ROM1K_H
#define ALETHEIA_ROM1K_H

#include "../core/types.h"
#include "rom256.h"

#define ROM_WORDS 1024
#define ROM_BANKS 4

typedef struct {
    rom_256x32_t banks[ROM_BANKS];
    bit data_out[32];
} rom_1kx32_t;

void rom1k_init(rom_1kx32_t *rom);
void rom1k_program(rom_1kx32_t *rom, bit *addr, bit *data);
void rom1k_fetch(rom_1kx32_t *rom, bit *addr, bit *out);

#endif
