#include "pc.h"
#include "../alu/adder.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

void pc_init(program_counter_t *pc) {
    reg32_init(&pc->reg);
    pc->increment = 0;
    pc->load = 0;
}

void pc_reset(program_counter_t *pc) {
    pc_init(pc);
}

void pc_set_increment(program_counter_t *pc, bit inc) {
    pc->increment = inc;
}

void pc_set_load(program_counter_t *pc, bit load) {
    pc->load = load;
}

void pc_set_load_data(program_counter_t *pc, bit *data) {
    pc->load_data[0] = data[0]; pc->load_data[1] = data[1];
    pc->load_data[2] = data[2]; pc->load_data[3] = data[3];
    pc->load_data[4] = data[4]; pc->load_data[5] = data[5];
    pc->load_data[6] = data[6]; pc->load_data[7] = data[7];
    pc->load_data[8] = data[8]; pc->load_data[9] = data[9];
    pc->load_data[10] = data[10]; pc->load_data[11] = data[11];
    pc->load_data[12] = data[12]; pc->load_data[13] = data[13];
    pc->load_data[14] = data[14]; pc->load_data[15] = data[15];
    pc->load_data[16] = data[16]; pc->load_data[17] = data[17];
    pc->load_data[18] = data[18]; pc->load_data[19] = data[19];
    pc->load_data[20] = data[20]; pc->load_data[21] = data[21];
    pc->load_data[22] = data[22]; pc->load_data[23] = data[23];
    pc->load_data[24] = data[24]; pc->load_data[25] = data[25];
    pc->load_data[26] = data[26]; pc->load_data[27] = data[27];
    pc->load_data[28] = data[28]; pc->load_data[29] = data[29];
    pc->load_data[30] = data[30]; pc->load_data[31] = data[31];
}

void pc_clock_rise(program_counter_t *pc) {
    reg32_clock_rise(&pc->reg);
}

void pc_clock_fall(program_counter_t *pc) {
    bit one[32] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    bit incremented[32];
    bit next_value[32];
    bit carries[33];

    adder_32(pc->reg.data_out, one, incremented, carries);

    mux2_32(pc->reg.data_out, incremented, pc->increment, next_value);
    mux2_32(next_value, pc->load_data, pc->load, next_value);

    reg32_set_input(&pc->reg, next_value);
    reg32_set_load(&pc->reg, OR(pc->increment, pc->load));
    reg32_clock_fall(&pc->reg);
}

void pc_propagate(program_counter_t *pc) {
    reg32_propagate(&pc->reg);
}
