#ifndef ALETHEIA_IR_H
#define ALETHEIA_IR_H

#include "../../core/types.h"
#include "register.h"

typedef struct {
    register_32_t reg;
} instruction_register_t;

void ir_init(instruction_register_t *ir);
void ir_reset(instruction_register_t *ir);
void ir_set_input(instruction_register_t *ir, bit *data);
void ir_set_load(instruction_register_t *ir, bit load);
void ir_clock_rise(instruction_register_t *ir);
void ir_clock_fall(instruction_register_t *ir);
void ir_propagate(instruction_register_t *ir);

#endif
