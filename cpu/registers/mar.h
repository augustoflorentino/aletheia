#ifndef ALETHEIA_MAR_H
#define ALETHEIA_MAR_H

#include "../../core/types.h"
#include "register.h"

typedef struct {
    register_32_t reg;
} memory_address_register_t;

void mar_init(memory_address_register_t *mar);
void mar_reset(memory_address_register_t *mar);
void mar_set_input(memory_address_register_t *mar, bit *data);
void mar_set_load(memory_address_register_t *mar, bit load);
void mar_clock_rise(memory_address_register_t *mar);
void mar_clock_fall(memory_address_register_t *mar);
void mar_propagate(memory_address_register_t *mar);

#endif
