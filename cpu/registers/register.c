#include "register.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

void reg8_init(register_8_t *reg) {
    dff_reset(&reg->bits[0]);
    dff_reset(&reg->bits[1]);
    dff_reset(&reg->bits[2]);
    dff_reset(&reg->bits[3]);
    dff_reset(&reg->bits[4]);
    dff_reset(&reg->bits[5]);
    dff_reset(&reg->bits[6]);
    dff_reset(&reg->bits[7]);
    reg->load = 0;
}

void reg8_reset(register_8_t *reg) {
    reg8_init(reg);
}

void reg8_set_input(register_8_t *reg, bit *data) {
    reg->data_in[0] = data[0];
    reg->data_in[1] = data[1];
    reg->data_in[2] = data[2];
    reg->data_in[3] = data[3];
    reg->data_in[4] = data[4];
    reg->data_in[5] = data[5];
    reg->data_in[6] = data[6];
    reg->data_in[7] = data[7];
}

void reg8_set_load(register_8_t *reg, bit load) {
    reg->load = load;
}

void reg8_clock_rise(register_8_t *reg) {
    dff_clock_rise(&reg->bits[0]);
    dff_clock_rise(&reg->bits[1]);
    dff_clock_rise(&reg->bits[2]);
    dff_clock_rise(&reg->bits[3]);
    dff_clock_rise(&reg->bits[4]);
    dff_clock_rise(&reg->bits[5]);
    dff_clock_rise(&reg->bits[6]);
    dff_clock_rise(&reg->bits[7]);
}

void reg8_clock_fall(register_8_t *reg) {
    reg->bits[0].d = mux2(reg->bits[0].q, reg->data_in[0], reg->load);
    reg->bits[1].d = mux2(reg->bits[1].q, reg->data_in[1], reg->load);
    reg->bits[2].d = mux2(reg->bits[2].q, reg->data_in[2], reg->load);
    reg->bits[3].d = mux2(reg->bits[3].q, reg->data_in[3], reg->load);
    reg->bits[4].d = mux2(reg->bits[4].q, reg->data_in[4], reg->load);
    reg->bits[5].d = mux2(reg->bits[5].q, reg->data_in[5], reg->load);
    reg->bits[6].d = mux2(reg->bits[6].q, reg->data_in[6], reg->load);
    reg->bits[7].d = mux2(reg->bits[7].q, reg->data_in[7], reg->load);

    dff_clock_fall(&reg->bits[0]);
    dff_clock_fall(&reg->bits[1]);
    dff_clock_fall(&reg->bits[2]);
    dff_clock_fall(&reg->bits[3]);
    dff_clock_fall(&reg->bits[4]);
    dff_clock_fall(&reg->bits[5]);
    dff_clock_fall(&reg->bits[6]);
    dff_clock_fall(&reg->bits[7]);
}

void reg8_propagate(register_8_t *reg) {
    reg->data_out[0] = reg->bits[0].q;
    reg->data_out[1] = reg->bits[1].q;
    reg->data_out[2] = reg->bits[2].q;
    reg->data_out[3] = reg->bits[3].q;
    reg->data_out[4] = reg->bits[4].q;
    reg->data_out[5] = reg->bits[5].q;
    reg->data_out[6] = reg->bits[6].q;
    reg->data_out[7] = reg->bits[7].q;
}

void reg32_init(register_32_t *reg) {
    dff_reset(&reg->bits[0]); dff_reset(&reg->bits[1]);
    dff_reset(&reg->bits[2]); dff_reset(&reg->bits[3]);
    dff_reset(&reg->bits[4]); dff_reset(&reg->bits[5]);
    dff_reset(&reg->bits[6]); dff_reset(&reg->bits[7]);
    dff_reset(&reg->bits[8]); dff_reset(&reg->bits[9]);
    dff_reset(&reg->bits[10]); dff_reset(&reg->bits[11]);
    dff_reset(&reg->bits[12]); dff_reset(&reg->bits[13]);
    dff_reset(&reg->bits[14]); dff_reset(&reg->bits[15]);
    dff_reset(&reg->bits[16]); dff_reset(&reg->bits[17]);
    dff_reset(&reg->bits[18]); dff_reset(&reg->bits[19]);
    dff_reset(&reg->bits[20]); dff_reset(&reg->bits[21]);
    dff_reset(&reg->bits[22]); dff_reset(&reg->bits[23]);
    dff_reset(&reg->bits[24]); dff_reset(&reg->bits[25]);
    dff_reset(&reg->bits[26]); dff_reset(&reg->bits[27]);
    dff_reset(&reg->bits[28]); dff_reset(&reg->bits[29]);
    dff_reset(&reg->bits[30]); dff_reset(&reg->bits[31]);
    reg->load = 0;
}

void reg32_reset(register_32_t *reg) {
    reg32_init(reg);
}

void reg32_set_input(register_32_t *reg, bit *data) {
    reg->data_in[0] = data[0]; reg->data_in[1] = data[1];
    reg->data_in[2] = data[2]; reg->data_in[3] = data[3];
    reg->data_in[4] = data[4]; reg->data_in[5] = data[5];
    reg->data_in[6] = data[6]; reg->data_in[7] = data[7];
    reg->data_in[8] = data[8]; reg->data_in[9] = data[9];
    reg->data_in[10] = data[10]; reg->data_in[11] = data[11];
    reg->data_in[12] = data[12]; reg->data_in[13] = data[13];
    reg->data_in[14] = data[14]; reg->data_in[15] = data[15];
    reg->data_in[16] = data[16]; reg->data_in[17] = data[17];
    reg->data_in[18] = data[18]; reg->data_in[19] = data[19];
    reg->data_in[20] = data[20]; reg->data_in[21] = data[21];
    reg->data_in[22] = data[22]; reg->data_in[23] = data[23];
    reg->data_in[24] = data[24]; reg->data_in[25] = data[25];
    reg->data_in[26] = data[26]; reg->data_in[27] = data[27];
    reg->data_in[28] = data[28]; reg->data_in[29] = data[29];
    reg->data_in[30] = data[30]; reg->data_in[31] = data[31];
}

void reg32_set_load(register_32_t *reg, bit load) {
    reg->load = load;
}

void reg32_clock_rise(register_32_t *reg) {
    dff_clock_rise(&reg->bits[0]); dff_clock_rise(&reg->bits[1]);
    dff_clock_rise(&reg->bits[2]); dff_clock_rise(&reg->bits[3]);
    dff_clock_rise(&reg->bits[4]); dff_clock_rise(&reg->bits[5]);
    dff_clock_rise(&reg->bits[6]); dff_clock_rise(&reg->bits[7]);
    dff_clock_rise(&reg->bits[8]); dff_clock_rise(&reg->bits[9]);
    dff_clock_rise(&reg->bits[10]); dff_clock_rise(&reg->bits[11]);
    dff_clock_rise(&reg->bits[12]); dff_clock_rise(&reg->bits[13]);
    dff_clock_rise(&reg->bits[14]); dff_clock_rise(&reg->bits[15]);
    dff_clock_rise(&reg->bits[16]); dff_clock_rise(&reg->bits[17]);
    dff_clock_rise(&reg->bits[18]); dff_clock_rise(&reg->bits[19]);
    dff_clock_rise(&reg->bits[20]); dff_clock_rise(&reg->bits[21]);
    dff_clock_rise(&reg->bits[22]); dff_clock_rise(&reg->bits[23]);
    dff_clock_rise(&reg->bits[24]); dff_clock_rise(&reg->bits[25]);
    dff_clock_rise(&reg->bits[26]); dff_clock_rise(&reg->bits[27]);
    dff_clock_rise(&reg->bits[28]); dff_clock_rise(&reg->bits[29]);
    dff_clock_rise(&reg->bits[30]); dff_clock_rise(&reg->bits[31]);
}

void reg32_clock_fall(register_32_t *reg) {
    reg->bits[0].d = mux2(reg->bits[0].q, reg->data_in[0], reg->load);
    reg->bits[1].d = mux2(reg->bits[1].q, reg->data_in[1], reg->load);
    reg->bits[2].d = mux2(reg->bits[2].q, reg->data_in[2], reg->load);
    reg->bits[3].d = mux2(reg->bits[3].q, reg->data_in[3], reg->load);
    reg->bits[4].d = mux2(reg->bits[4].q, reg->data_in[4], reg->load);
    reg->bits[5].d = mux2(reg->bits[5].q, reg->data_in[5], reg->load);
    reg->bits[6].d = mux2(reg->bits[6].q, reg->data_in[6], reg->load);
    reg->bits[7].d = mux2(reg->bits[7].q, reg->data_in[7], reg->load);
    reg->bits[8].d = mux2(reg->bits[8].q, reg->data_in[8], reg->load);
    reg->bits[9].d = mux2(reg->bits[9].q, reg->data_in[9], reg->load);
    reg->bits[10].d = mux2(reg->bits[10].q, reg->data_in[10], reg->load);
    reg->bits[11].d = mux2(reg->bits[11].q, reg->data_in[11], reg->load);
    reg->bits[12].d = mux2(reg->bits[12].q, reg->data_in[12], reg->load);
    reg->bits[13].d = mux2(reg->bits[13].q, reg->data_in[13], reg->load);
    reg->bits[14].d = mux2(reg->bits[14].q, reg->data_in[14], reg->load);
    reg->bits[15].d = mux2(reg->bits[15].q, reg->data_in[15], reg->load);
    reg->bits[16].d = mux2(reg->bits[16].q, reg->data_in[16], reg->load);
    reg->bits[17].d = mux2(reg->bits[17].q, reg->data_in[17], reg->load);
    reg->bits[18].d = mux2(reg->bits[18].q, reg->data_in[18], reg->load);
    reg->bits[19].d = mux2(reg->bits[19].q, reg->data_in[19], reg->load);
    reg->bits[20].d = mux2(reg->bits[20].q, reg->data_in[20], reg->load);
    reg->bits[21].d = mux2(reg->bits[21].q, reg->data_in[21], reg->load);
    reg->bits[22].d = mux2(reg->bits[22].q, reg->data_in[22], reg->load);
    reg->bits[23].d = mux2(reg->bits[23].q, reg->data_in[23], reg->load);
    reg->bits[24].d = mux2(reg->bits[24].q, reg->data_in[24], reg->load);
    reg->bits[25].d = mux2(reg->bits[25].q, reg->data_in[25], reg->load);
    reg->bits[26].d = mux2(reg->bits[26].q, reg->data_in[26], reg->load);
    reg->bits[27].d = mux2(reg->bits[27].q, reg->data_in[27], reg->load);
    reg->bits[28].d = mux2(reg->bits[28].q, reg->data_in[28], reg->load);
    reg->bits[29].d = mux2(reg->bits[29].q, reg->data_in[29], reg->load);
    reg->bits[30].d = mux2(reg->bits[30].q, reg->data_in[30], reg->load);
    reg->bits[31].d = mux2(reg->bits[31].q, reg->data_in[31], reg->load);

    dff_clock_fall(&reg->bits[0]); dff_clock_fall(&reg->bits[1]);
    dff_clock_fall(&reg->bits[2]); dff_clock_fall(&reg->bits[3]);
    dff_clock_fall(&reg->bits[4]); dff_clock_fall(&reg->bits[5]);
    dff_clock_fall(&reg->bits[6]); dff_clock_fall(&reg->bits[7]);
    dff_clock_fall(&reg->bits[8]); dff_clock_fall(&reg->bits[9]);
    dff_clock_fall(&reg->bits[10]); dff_clock_fall(&reg->bits[11]);
    dff_clock_fall(&reg->bits[12]); dff_clock_fall(&reg->bits[13]);
    dff_clock_fall(&reg->bits[14]); dff_clock_fall(&reg->bits[15]);
    dff_clock_fall(&reg->bits[16]); dff_clock_fall(&reg->bits[17]);
    dff_clock_fall(&reg->bits[18]); dff_clock_fall(&reg->bits[19]);
    dff_clock_fall(&reg->bits[20]); dff_clock_fall(&reg->bits[21]);
    dff_clock_fall(&reg->bits[22]); dff_clock_fall(&reg->bits[23]);
    dff_clock_fall(&reg->bits[24]); dff_clock_fall(&reg->bits[25]);
    dff_clock_fall(&reg->bits[26]); dff_clock_fall(&reg->bits[27]);
    dff_clock_fall(&reg->bits[28]); dff_clock_fall(&reg->bits[29]);
    dff_clock_fall(&reg->bits[30]); dff_clock_fall(&reg->bits[31]);
}

void reg32_propagate(register_32_t *reg) {
    reg->data_out[0] = reg->bits[0].q; reg->data_out[1] = reg->bits[1].q;
    reg->data_out[2] = reg->bits[2].q; reg->data_out[3] = reg->bits[3].q;
    reg->data_out[4] = reg->bits[4].q; reg->data_out[5] = reg->bits[5].q;
    reg->data_out[6] = reg->bits[6].q; reg->data_out[7] = reg->bits[7].q;
    reg->data_out[8] = reg->bits[8].q; reg->data_out[9] = reg->bits[9].q;
    reg->data_out[10] = reg->bits[10].q; reg->data_out[11] = reg->bits[11].q;
    reg->data_out[12] = reg->bits[12].q; reg->data_out[13] = reg->bits[13].q;
    reg->data_out[14] = reg->bits[14].q; reg->data_out[15] = reg->bits[15].q;
    reg->data_out[16] = reg->bits[16].q; reg->data_out[17] = reg->bits[17].q;
    reg->data_out[18] = reg->bits[18].q; reg->data_out[19] = reg->bits[19].q;
    reg->data_out[20] = reg->bits[20].q; reg->data_out[21] = reg->bits[21].q;
    reg->data_out[22] = reg->bits[22].q; reg->data_out[23] = reg->bits[23].q;
    reg->data_out[24] = reg->bits[24].q; reg->data_out[25] = reg->bits[25].q;
    reg->data_out[26] = reg->bits[26].q; reg->data_out[27] = reg->bits[27].q;
    reg->data_out[28] = reg->bits[28].q; reg->data_out[29] = reg->bits[29].q;
    reg->data_out[30] = reg->bits[30].q; reg->data_out[31] = reg->bits[31].q;
}
