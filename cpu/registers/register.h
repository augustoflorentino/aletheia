#ifndef ALETHEIA_REGISTER_H
#define ALETHEIA_REGISTER_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"

typedef struct {
    d_flip_flop_t bits[8];
    bit load;
    bit data_in[8];
    bit data_out[8];
} register_8_t;

typedef struct {
    d_flip_flop_t bits[32];
    bit load;
    bit data_in[32];
    bit data_out[32];
} register_32_t;

void reg8_init(register_8_t *reg);
void reg8_reset(register_8_t *reg);
void reg8_set_input(register_8_t *reg, bit *data);
void reg8_set_load(register_8_t *reg, bit load);
void reg8_clock_rise(register_8_t *reg);
void reg8_clock_fall(register_8_t *reg);
void reg8_propagate(register_8_t *reg);

void reg32_init(register_32_t *reg);
void reg32_reset(register_32_t *reg);
void reg32_set_input(register_32_t *reg, bit *data);
void reg32_set_load(register_32_t *reg, bit load);
void reg32_clock_rise(register_32_t *reg);
void reg32_clock_fall(register_32_t *reg);
void reg32_propagate(register_32_t *reg);

#endif
