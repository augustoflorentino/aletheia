#ifndef ALETHEIA_CONFIG_H
#define ALETHEIA_CONFIG_H

#include "../core/types.h"
#include "../core/sequential/d_flip_flop.h"

typedef struct {
    d_flip_flop_t boot_mode[2];
    d_flip_flop_t clock_select[2];
    d_flip_flop_t wait_states[2];
    d_flip_flop_t bus_width[2];

    bit boot_mode_in[2];
    bit clock_select_in[2];
    bit wait_states_in[2];
    bit bus_width_in[2];

    bit boot_mode_out[2];
    bit clock_select_out[2];
    bit wait_states_out[2];
    bit bus_width_out[2];

    bit chip_select;
    bit write;
    bit addr[2];
} system_config_t;

void config_init(system_config_t *cfg);
void config_reset(system_config_t *cfg);
void config_set_cs(system_config_t *cfg, bit cs);
void config_set_write(system_config_t *cfg, bit wr);
void config_set_addr(system_config_t *cfg, bit a0, bit a1);
void config_set_data(system_config_t *cfg, bit *data);
void config_clock_rise(system_config_t *cfg);
void config_clock_fall(system_config_t *cfg);
void config_propagate(system_config_t *cfg);
void config_get_boot_mode(system_config_t *cfg, bit *out);
void config_get_clock_select(system_config_t *cfg, bit *out);
void config_get_wait_states(system_config_t *cfg, bit *out);
void config_get_bus_width(system_config_t *cfg, bit *out);

#endif
