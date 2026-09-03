#ifndef ALETHEIA_CONFIG_H
#define ALETHEIA_CONFIG_H

#include "../core/types.h"
#include "../core/sequential/d_flip_flop.h"

#define STRAP_L1 0
#define STRAP_L2 1
#define STRAP_MMU 2
#define STRAP_TDIV0 3
#define STRAP_TDIV1 4
#define STRAP_COUNT 5

typedef struct {
    d_flip_flop_t strap[STRAP_COUNT];
    bit pin[STRAP_COUNT];
} system_config_t;

void config_init(system_config_t *cfg);
void config_reset(system_config_t *cfg);
bit config_l1(system_config_t *cfg);
bit config_l2(system_config_t *cfg);
bit config_mmu(system_config_t *cfg);
void config_timer_div(system_config_t *cfg, bit *out);

#endif
