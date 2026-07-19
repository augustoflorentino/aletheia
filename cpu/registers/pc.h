#ifndef ALETHEIA_PC_H
#define ALETHEIA_PC_H

#include "../../core/types.h"
#include "register.h"

typedef struct {
    register_32_t reg;
    bit increment;
    bit load;
    bit load_data[32];
} program_counter_t;

void pc_init(program_counter_t *pc);
void pc_reset(program_counter_t *pc);
void pc_set_increment(program_counter_t *pc, bit inc);
void pc_set_load(program_counter_t *pc, bit load);
void pc_set_load_data(program_counter_t *pc, bit *data);
void pc_clock_rise(program_counter_t *pc);
void pc_clock_fall(program_counter_t *pc);
void pc_propagate(program_counter_t *pc);

#endif
