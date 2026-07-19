#ifndef ALETHEIA_POWER_H
#define ALETHEIA_POWER_H

#include "../core/types.h"
#include "../core/sequential/d_flip_flop.h"

#define RESET_CYCLES 4

typedef struct {
    d_flip_flop_t reset_counter[3];
    d_flip_flop_t power_good;
    d_flip_flop_t reset_out;

    bit power_in;
    bit external_reset;

    bit reset;
    bit ready;
    bit power_ok;
} power_controller_t;

void power_init(power_controller_t *pwr);
void power_reset(power_controller_t *pwr);
void power_set_power(power_controller_t *pwr, bit pwr_in);
void power_set_external_reset(power_controller_t *pwr, bit rst);
void power_clock_rise(power_controller_t *pwr);
void power_clock_fall(power_controller_t *pwr);
void power_propagate(power_controller_t *pwr);
bit power_get_reset(power_controller_t *pwr);
bit power_is_ready(power_controller_t *pwr);

#endif
