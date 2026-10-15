#ifndef ALETHEIA_RAM_H
#define ALETHEIA_RAM_H

#if defined(ALETHEIA_MEM_4K)
#include "../memory/sram4k.h"
#define RAM_WORDS 4096
typedef sram_4kx32_t ram_t;
#define RAM_PREFIX(fn) sram4k_##fn
#define RAM_CELL(r, a) (&(r)->banks[(a) >> 10].banks[((a) >> 8) & 3].banks[((a) >> 4) & 15].cells[(a) & 15])
#elif defined(ALETHEIA_MEM_1K)
#include "../memory/sram1k.h"
#define RAM_WORDS 1024
typedef sram_1kx32_t ram_t;
#define RAM_PREFIX(fn) sram1k_##fn
#define RAM_CELL(r, a) (&(r)->banks[(a) >> 8].banks[((a) >> 4) & 15].cells[(a) & 15])
#else
#include "../memory/sram256.h"
#define RAM_WORDS 256
typedef sram_256x32_t ram_t;
#define RAM_PREFIX(fn) sram256_##fn
#define RAM_CELL(r, a) (&(r)->banks[(a) >> 4].cells[(a) & 15])
#endif

#define ram_init RAM_PREFIX(init)
#define ram_reset RAM_PREFIX(reset)
#define ram_set_addr RAM_PREFIX(set_addr)
#define ram_set_data RAM_PREFIX(set_data)
#define ram_set_write RAM_PREFIX(set_write)
#define ram_set_cs RAM_PREFIX(set_cs)
#define ram_clock_fall RAM_PREFIX(clock_fall)
#define ram_clock_rise RAM_PREFIX(clock_rise)
#define ram_propagate RAM_PREFIX(propagate)
#define ram_read RAM_PREFIX(read)

#endif
