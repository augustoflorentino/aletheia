#include "mdr.h"

void mdr_init(memory_data_register_t *mdr) {
    reg32_init(&mdr->reg);
}

void mdr_reset(memory_data_register_t *mdr) {
    mdr_init(mdr);
}

void mdr_set_input(memory_data_register_t *mdr, bit *data) {
    reg32_set_input(&mdr->reg, data);
}

void mdr_set_load(memory_data_register_t *mdr, bit load) {
    reg32_set_load(&mdr->reg, load);
}

void mdr_clock_rise(memory_data_register_t *mdr) {
    reg32_clock_rise(&mdr->reg);
}

void mdr_clock_fall(memory_data_register_t *mdr) {
    reg32_clock_fall(&mdr->reg);
}

void mdr_propagate(memory_data_register_t *mdr) {
    reg32_propagate(&mdr->reg);
}
