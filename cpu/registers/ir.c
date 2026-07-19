#include "ir.h"

void ir_init(instruction_register_t *ir) {
    reg32_init(&ir->reg);
}

void ir_reset(instruction_register_t *ir) {
    ir_init(ir);
}

void ir_set_input(instruction_register_t *ir, bit *data) {
    reg32_set_input(&ir->reg, data);
}

void ir_set_load(instruction_register_t *ir, bit load) {
    reg32_set_load(&ir->reg, load);
}

void ir_clock_rise(instruction_register_t *ir) {
    reg32_clock_rise(&ir->reg);
}

void ir_clock_fall(instruction_register_t *ir) {
    reg32_clock_fall(&ir->reg);
}

void ir_propagate(instruction_register_t *ir) {
    reg32_propagate(&ir->reg);
}
