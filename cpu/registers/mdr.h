#ifndef ALETHEIA_MDR_H
#define ALETHEIA_MDR_H

#include "../../core/types.h"
#include "register.h"

typedef struct {
    register_32_t reg;
} memory_data_register_t;

void mdr_init(memory_data_register_t *mdr);
void mdr_reset(memory_data_register_t *mdr);
void mdr_set_input(memory_data_register_t *mdr, bit *data);
void mdr_set_load(memory_data_register_t *mdr, bit load);
void mdr_clock_rise(memory_data_register_t *mdr);
void mdr_clock_fall(memory_data_register_t *mdr);
void mdr_propagate(memory_data_register_t *mdr);

#endif
