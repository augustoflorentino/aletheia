#include "mar.h"

void mar_init(memory_address_register_t *mar) {
    reg32_init(&mar->reg);
}

void mar_reset(memory_address_register_t *mar) {
    mar_init(mar);
}

void mar_set_input(memory_address_register_t *mar, bit *data) {
    reg32_set_input(&mar->reg, data);
}

void mar_set_load(memory_address_register_t *mar, bit load) {
    reg32_set_load(&mar->reg, load);
}

void mar_clock_rise(memory_address_register_t *mar) {
    reg32_clock_rise(&mar->reg);
}

void mar_clock_fall(memory_address_register_t *mar) {
    reg32_clock_fall(&mar->reg);
}

void mar_propagate(memory_address_register_t *mar) {
    reg32_propagate(&mar->reg);
}
