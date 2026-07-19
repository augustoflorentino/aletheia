#ifndef ALETHEIA_CACHE_CONTROLLER_H
#define ALETHEIA_CACHE_CONTROLLER_H

#include "../../core/types.h"
#include "l1_cache.h"
#include "../sram.h"

typedef struct {
    l1_cache_t l1;
    sram_16x32_t mem;

    bit addr[32];
    bit data_out[32];
    bit hit_out;
} cache_controller_t;

void cc_init(cache_controller_t *cc);
void cc_reset(cache_controller_t *cc);
void cc_mem_write(cache_controller_t *cc, bit *addr, bit *data);
void cc_access(cache_controller_t *cc, bit *addr);

#endif
