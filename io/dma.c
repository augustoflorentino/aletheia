#include "dma.h"
#include "../core/gates/and.h"
#include "../core/gates/or.h"
#include "../core/gates/not.h"
#include "../core/gates/xor.h"
#include "../core/mux/mux2.h"
#include "../cpu/alu/adder.h"

void dma_init(dma_controller_t *dma) {
    int i;
    i = 0; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);
    i = 1; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);
    i = 2; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);
    i = 3; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);
    i = 4; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);
    i = 5; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);
    i = 6; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);
    i = 7; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);
    i = 8; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);
    i = 9; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);
    i = 10; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);
    i = 11; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);
    i = 12; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);
    i = 13; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);
    i = 14; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);
    i = 15; dff_reset(&dma->src_addr[i]); dff_reset(&dma->dst_addr[i]); dff_reset(&dma->count[i]);

    dff_reset(&dma->control[0]); dff_reset(&dma->control[1]);
    dff_reset(&dma->control[2]); dff_reset(&dma->control[3]);
    dff_reset(&dma->control[4]); dff_reset(&dma->control[5]);
    dff_reset(&dma->control[6]); dff_reset(&dma->control[7]);

    dff_reset(&dma->status[0]); dff_reset(&dma->status[1]);
    dff_reset(&dma->status[2]); dff_reset(&dma->status[3]);
    dff_reset(&dma->status[4]); dff_reset(&dma->status[5]);
    dff_reset(&dma->status[6]); dff_reset(&dma->status[7]);

    i = 0; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;
    i = 1; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;
    i = 2; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;
    i = 3; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;
    i = 4; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;
    i = 5; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;
    i = 6; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;
    i = 7; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;
    i = 8; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;
    i = 9; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;
    i = 10; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;
    i = 11; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;
    i = 12; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;
    i = 13; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;
    i = 14; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;
    i = 15; dma->src_in[i] = 0; dma->dst_in[i] = 0; dma->count_in[i] = 0;

    dma->control_in[0] = 0; dma->control_in[1] = 0;
    dma->control_in[2] = 0; dma->control_in[3] = 0;
    dma->control_in[4] = 0; dma->control_in[5] = 0;
    dma->control_in[6] = 0; dma->control_in[7] = 0;

    i = 0; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;
    i = 1; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;
    i = 2; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;
    i = 3; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;
    i = 4; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;
    i = 5; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;
    i = 6; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;
    i = 7; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;
    i = 8; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;
    i = 9; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;
    i = 10; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;
    i = 11; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;
    i = 12; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;
    i = 13; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;
    i = 14; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;
    i = 15; dma->src_out[i] = 0; dma->dst_out[i] = 0; dma->count_out[i] = 0;

    dma->status_out[0] = 0; dma->status_out[1] = 0;
    dma->status_out[2] = 0; dma->status_out[3] = 0;
    dma->status_out[4] = 0; dma->status_out[5] = 0;
    dma->status_out[6] = 0; dma->status_out[7] = 0;

    i = 0; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 1; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 2; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 3; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 4; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 5; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 6; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 7; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 8; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 9; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 10; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 11; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 12; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 13; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 14; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 15; dma->data_in[i] = 0; dma->data_out[i] = 0; dma->addr_out[i] = 0;
    i = 16; dma->data_in[i] = 0; dma->data_out[i] = 0;
    i = 17; dma->data_in[i] = 0; dma->data_out[i] = 0;
    i = 18; dma->data_in[i] = 0; dma->data_out[i] = 0;
    i = 19; dma->data_in[i] = 0; dma->data_out[i] = 0;
    i = 20; dma->data_in[i] = 0; dma->data_out[i] = 0;
    i = 21; dma->data_in[i] = 0; dma->data_out[i] = 0;
    i = 22; dma->data_in[i] = 0; dma->data_out[i] = 0;
    i = 23; dma->data_in[i] = 0; dma->data_out[i] = 0;
    i = 24; dma->data_in[i] = 0; dma->data_out[i] = 0;
    i = 25; dma->data_in[i] = 0; dma->data_out[i] = 0;
    i = 26; dma->data_in[i] = 0; dma->data_out[i] = 0;
    i = 27; dma->data_in[i] = 0; dma->data_out[i] = 0;
    i = 28; dma->data_in[i] = 0; dma->data_out[i] = 0;
    i = 29; dma->data_in[i] = 0; dma->data_out[i] = 0;
    i = 30; dma->data_in[i] = 0; dma->data_out[i] = 0;
    i = 31; dma->data_in[i] = 0; dma->data_out[i] = 0;

    dma->chip_select = 0;
    dma->read = 0;
    dma->write = 0;
    dma->addr[0] = 0; dma->addr[1] = 0; dma->addr[2] = 0;

    dma->dreq = 0;
    dma->dack = 0;
    dma->tc = 0;
    dma->bus_request = 0;
    dma->bus_grant = 0;
    dma->mem_read = 0;
    dma->mem_write = 0;
}

void dma_reset(dma_controller_t *dma) {
    dma_init(dma);
}

void dma_set_cs(dma_controller_t *dma, bit cs) {
    dma->chip_select = cs;
}

void dma_set_read(dma_controller_t *dma, bit rd) {
    dma->read = rd;
}

void dma_set_write(dma_controller_t *dma, bit wr) {
    dma->write = wr;
}

void dma_set_addr(dma_controller_t *dma, bit *addr) {
    dma->addr[0] = addr[0];
    dma->addr[1] = addr[1];
    dma->addr[2] = addr[2];
}

void dma_set_cpu_data(dma_controller_t *dma, bit *data) {
    dma->src_in[0] = data[0]; dma->src_in[1] = data[1];
    dma->src_in[2] = data[2]; dma->src_in[3] = data[3];
    dma->src_in[4] = data[4]; dma->src_in[5] = data[5];
    dma->src_in[6] = data[6]; dma->src_in[7] = data[7];
    dma->src_in[8] = data[8]; dma->src_in[9] = data[9];
    dma->src_in[10] = data[10]; dma->src_in[11] = data[11];
    dma->src_in[12] = data[12]; dma->src_in[13] = data[13];
    dma->src_in[14] = data[14]; dma->src_in[15] = data[15];

    dma->dst_in[0] = data[0]; dma->dst_in[1] = data[1];
    dma->dst_in[2] = data[2]; dma->dst_in[3] = data[3];
    dma->dst_in[4] = data[4]; dma->dst_in[5] = data[5];
    dma->dst_in[6] = data[6]; dma->dst_in[7] = data[7];
    dma->dst_in[8] = data[8]; dma->dst_in[9] = data[9];
    dma->dst_in[10] = data[10]; dma->dst_in[11] = data[11];
    dma->dst_in[12] = data[12]; dma->dst_in[13] = data[13];
    dma->dst_in[14] = data[14]; dma->dst_in[15] = data[15];

    dma->count_in[0] = data[0]; dma->count_in[1] = data[1];
    dma->count_in[2] = data[2]; dma->count_in[3] = data[3];
    dma->count_in[4] = data[4]; dma->count_in[5] = data[5];
    dma->count_in[6] = data[6]; dma->count_in[7] = data[7];
    dma->count_in[8] = data[8]; dma->count_in[9] = data[9];
    dma->count_in[10] = data[10]; dma->count_in[11] = data[11];
    dma->count_in[12] = data[12]; dma->count_in[13] = data[13];
    dma->count_in[14] = data[14]; dma->count_in[15] = data[15];

    dma->control_in[0] = data[0]; dma->control_in[1] = data[1];
    dma->control_in[2] = data[2]; dma->control_in[3] = data[3];
    dma->control_in[4] = data[4]; dma->control_in[5] = data[5];
    dma->control_in[6] = data[6]; dma->control_in[7] = data[7];
}

void dma_set_mem_data(dma_controller_t *dma, bit *data) {
    dma->data_in[0] = data[0]; dma->data_in[1] = data[1];
    dma->data_in[2] = data[2]; dma->data_in[3] = data[3];
    dma->data_in[4] = data[4]; dma->data_in[5] = data[5];
    dma->data_in[6] = data[6]; dma->data_in[7] = data[7];
    dma->data_in[8] = data[8]; dma->data_in[9] = data[9];
    dma->data_in[10] = data[10]; dma->data_in[11] = data[11];
    dma->data_in[12] = data[12]; dma->data_in[13] = data[13];
    dma->data_in[14] = data[14]; dma->data_in[15] = data[15];
    dma->data_in[16] = data[16]; dma->data_in[17] = data[17];
    dma->data_in[18] = data[18]; dma->data_in[19] = data[19];
    dma->data_in[20] = data[20]; dma->data_in[21] = data[21];
    dma->data_in[22] = data[22]; dma->data_in[23] = data[23];
    dma->data_in[24] = data[24]; dma->data_in[25] = data[25];
    dma->data_in[26] = data[26]; dma->data_in[27] = data[27];
    dma->data_in[28] = data[28]; dma->data_in[29] = data[29];
    dma->data_in[30] = data[30]; dma->data_in[31] = data[31];
}

void dma_set_dreq(dma_controller_t *dma, bit dreq) {
    dma->dreq = dreq;
}

void dma_set_bus_grant(dma_controller_t *dma, bit grant) {
    dma->bus_grant = grant;
}

void dma_clock_rise(dma_controller_t *dma) {
    int i;
    i = 0; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);
    i = 1; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);
    i = 2; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);
    i = 3; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);
    i = 4; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);
    i = 5; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);
    i = 6; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);
    i = 7; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);
    i = 8; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);
    i = 9; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);
    i = 10; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);
    i = 11; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);
    i = 12; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);
    i = 13; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);
    i = 14; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);
    i = 15; dff_clock_rise(&dma->src_addr[i]); dff_clock_rise(&dma->dst_addr[i]); dff_clock_rise(&dma->count[i]);

    dff_clock_rise(&dma->control[0]); dff_clock_rise(&dma->control[1]);
    dff_clock_rise(&dma->control[2]); dff_clock_rise(&dma->control[3]);
    dff_clock_rise(&dma->control[4]); dff_clock_rise(&dma->control[5]);
    dff_clock_rise(&dma->control[6]); dff_clock_rise(&dma->control[7]);

    dff_clock_rise(&dma->status[0]); dff_clock_rise(&dma->status[1]);
    dff_clock_rise(&dma->status[2]); dff_clock_rise(&dma->status[3]);
    dff_clock_rise(&dma->status[4]); dff_clock_rise(&dma->status[5]);
    dff_clock_rise(&dma->status[6]); dff_clock_rise(&dma->status[7]);
}

void dma_clock_fall(dma_controller_t *dma) {
    bit active = dma->chip_select;
    bit wr = AND(dma->write, active);

    bit na0 = NOT(dma->addr[0]);
    bit na1 = NOT(dma->addr[1]);
    bit na2 = NOT(dma->addr[2]);

    bit sel_src = AND(AND(na2, na1), na0);
    bit sel_dst = AND(AND(na2, na1), dma->addr[0]);
    bit sel_cnt = AND(AND(na2, dma->addr[1]), na0);
    bit sel_ctrl = AND(AND(na2, dma->addr[1]), dma->addr[0]);

    bit wr_src = AND(wr, sel_src);
    bit wr_dst = AND(wr, sel_dst);
    bit wr_cnt = AND(wr, sel_cnt);
    bit wr_ctrl = AND(wr, sel_ctrl);

    bit x_enabled = dma->control[0].q;
    bit x_cnt_nz = OR(OR(OR(OR(dma->count[0].q, dma->count[1].q), OR(dma->count[2].q, dma->count[3].q)), OR(OR(dma->count[4].q, dma->count[5].q), OR(dma->count[6].q, dma->count[7].q))), OR(OR(OR(dma->count[8].q, dma->count[9].q), OR(dma->count[10].q, dma->count[11].q)), OR(OR(dma->count[12].q, dma->count[13].q), OR(dma->count[14].q, dma->count[15].q))));
    bit xfer = AND(dma->bus_grant, AND(x_enabled, x_cnt_nz));
    bit src_plus1[16]; bit srcc[17]; srcc[0] = 1;
    src_plus1[0] = XOR(dma->src_addr[0].q, srcc[0]); srcc[1] = AND(dma->src_addr[0].q, srcc[0]);
    src_plus1[1] = XOR(dma->src_addr[1].q, srcc[1]); srcc[2] = AND(dma->src_addr[1].q, srcc[1]);
    src_plus1[2] = XOR(dma->src_addr[2].q, srcc[2]); srcc[3] = AND(dma->src_addr[2].q, srcc[2]);
    src_plus1[3] = XOR(dma->src_addr[3].q, srcc[3]); srcc[4] = AND(dma->src_addr[3].q, srcc[3]);
    src_plus1[4] = XOR(dma->src_addr[4].q, srcc[4]); srcc[5] = AND(dma->src_addr[4].q, srcc[4]);
    src_plus1[5] = XOR(dma->src_addr[5].q, srcc[5]); srcc[6] = AND(dma->src_addr[5].q, srcc[5]);
    src_plus1[6] = XOR(dma->src_addr[6].q, srcc[6]); srcc[7] = AND(dma->src_addr[6].q, srcc[6]);
    src_plus1[7] = XOR(dma->src_addr[7].q, srcc[7]); srcc[8] = AND(dma->src_addr[7].q, srcc[7]);
    src_plus1[8] = XOR(dma->src_addr[8].q, srcc[8]); srcc[9] = AND(dma->src_addr[8].q, srcc[8]);
    src_plus1[9] = XOR(dma->src_addr[9].q, srcc[9]); srcc[10] = AND(dma->src_addr[9].q, srcc[9]);
    src_plus1[10] = XOR(dma->src_addr[10].q, srcc[10]); srcc[11] = AND(dma->src_addr[10].q, srcc[10]);
    src_plus1[11] = XOR(dma->src_addr[11].q, srcc[11]); srcc[12] = AND(dma->src_addr[11].q, srcc[11]);
    src_plus1[12] = XOR(dma->src_addr[12].q, srcc[12]); srcc[13] = AND(dma->src_addr[12].q, srcc[12]);
    src_plus1[13] = XOR(dma->src_addr[13].q, srcc[13]); srcc[14] = AND(dma->src_addr[13].q, srcc[13]);
    src_plus1[14] = XOR(dma->src_addr[14].q, srcc[14]); srcc[15] = AND(dma->src_addr[14].q, srcc[14]);
    src_plus1[15] = XOR(dma->src_addr[15].q, srcc[15]); srcc[16] = AND(dma->src_addr[15].q, srcc[15]);
    bit dst_plus1[16]; bit dstc[17]; dstc[0] = 1;
    dst_plus1[0] = XOR(dma->dst_addr[0].q, dstc[0]); dstc[1] = AND(dma->dst_addr[0].q, dstc[0]);
    dst_plus1[1] = XOR(dma->dst_addr[1].q, dstc[1]); dstc[2] = AND(dma->dst_addr[1].q, dstc[1]);
    dst_plus1[2] = XOR(dma->dst_addr[2].q, dstc[2]); dstc[3] = AND(dma->dst_addr[2].q, dstc[2]);
    dst_plus1[3] = XOR(dma->dst_addr[3].q, dstc[3]); dstc[4] = AND(dma->dst_addr[3].q, dstc[3]);
    dst_plus1[4] = XOR(dma->dst_addr[4].q, dstc[4]); dstc[5] = AND(dma->dst_addr[4].q, dstc[4]);
    dst_plus1[5] = XOR(dma->dst_addr[5].q, dstc[5]); dstc[6] = AND(dma->dst_addr[5].q, dstc[5]);
    dst_plus1[6] = XOR(dma->dst_addr[6].q, dstc[6]); dstc[7] = AND(dma->dst_addr[6].q, dstc[6]);
    dst_plus1[7] = XOR(dma->dst_addr[7].q, dstc[7]); dstc[8] = AND(dma->dst_addr[7].q, dstc[7]);
    dst_plus1[8] = XOR(dma->dst_addr[8].q, dstc[8]); dstc[9] = AND(dma->dst_addr[8].q, dstc[8]);
    dst_plus1[9] = XOR(dma->dst_addr[9].q, dstc[9]); dstc[10] = AND(dma->dst_addr[9].q, dstc[9]);
    dst_plus1[10] = XOR(dma->dst_addr[10].q, dstc[10]); dstc[11] = AND(dma->dst_addr[10].q, dstc[10]);
    dst_plus1[11] = XOR(dma->dst_addr[11].q, dstc[11]); dstc[12] = AND(dma->dst_addr[11].q, dstc[11]);
    dst_plus1[12] = XOR(dma->dst_addr[12].q, dstc[12]); dstc[13] = AND(dma->dst_addr[12].q, dstc[12]);
    dst_plus1[13] = XOR(dma->dst_addr[13].q, dstc[13]); dstc[14] = AND(dma->dst_addr[13].q, dstc[13]);
    dst_plus1[14] = XOR(dma->dst_addr[14].q, dstc[14]); dstc[15] = AND(dma->dst_addr[14].q, dstc[14]);
    dst_plus1[15] = XOR(dma->dst_addr[15].q, dstc[15]); dstc[16] = AND(dma->dst_addr[15].q, dstc[15]);
    bit count_minus1[16]; bit cmb[17]; cmb[0] = 1;
    count_minus1[0] = XOR(dma->count[0].q, cmb[0]); cmb[1] = AND(NOT(dma->count[0].q), cmb[0]);
    count_minus1[1] = XOR(dma->count[1].q, cmb[1]); cmb[2] = AND(NOT(dma->count[1].q), cmb[1]);
    count_minus1[2] = XOR(dma->count[2].q, cmb[2]); cmb[3] = AND(NOT(dma->count[2].q), cmb[2]);
    count_minus1[3] = XOR(dma->count[3].q, cmb[3]); cmb[4] = AND(NOT(dma->count[3].q), cmb[3]);
    count_minus1[4] = XOR(dma->count[4].q, cmb[4]); cmb[5] = AND(NOT(dma->count[4].q), cmb[4]);
    count_minus1[5] = XOR(dma->count[5].q, cmb[5]); cmb[6] = AND(NOT(dma->count[5].q), cmb[5]);
    count_minus1[6] = XOR(dma->count[6].q, cmb[6]); cmb[7] = AND(NOT(dma->count[6].q), cmb[6]);
    count_minus1[7] = XOR(dma->count[7].q, cmb[7]); cmb[8] = AND(NOT(dma->count[7].q), cmb[7]);
    count_minus1[8] = XOR(dma->count[8].q, cmb[8]); cmb[9] = AND(NOT(dma->count[8].q), cmb[8]);
    count_minus1[9] = XOR(dma->count[9].q, cmb[9]); cmb[10] = AND(NOT(dma->count[9].q), cmb[9]);
    count_minus1[10] = XOR(dma->count[10].q, cmb[10]); cmb[11] = AND(NOT(dma->count[10].q), cmb[10]);
    count_minus1[11] = XOR(dma->count[11].q, cmb[11]); cmb[12] = AND(NOT(dma->count[11].q), cmb[11]);
    count_minus1[12] = XOR(dma->count[12].q, cmb[12]); cmb[13] = AND(NOT(dma->count[12].q), cmb[12]);
    count_minus1[13] = XOR(dma->count[13].q, cmb[13]); cmb[14] = AND(NOT(dma->count[13].q), cmb[13]);
    count_minus1[14] = XOR(dma->count[14].q, cmb[14]); cmb[15] = AND(NOT(dma->count[14].q), cmb[14]);
    count_minus1[15] = XOR(dma->count[15].q, cmb[15]); cmb[16] = AND(NOT(dma->count[15].q), cmb[15]);
    (void)srcc[16]; (void)dstc[16]; (void)cmb[16];

    int i;
    i = 0; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);
    i = 1; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);
    i = 2; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);
    i = 3; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);
    i = 4; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);
    i = 5; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);
    i = 6; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);
    i = 7; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);
    i = 8; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);
    i = 9; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);
    i = 10; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);
    i = 11; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);
    i = 12; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);
    i = 13; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);
    i = 14; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);
    i = 15; dma->src_addr[i].d = mux2(mux2(dma->src_addr[i].q, src_plus1[i], xfer), dma->src_in[i], wr_src);

    i = 0; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);
    i = 1; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);
    i = 2; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);
    i = 3; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);
    i = 4; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);
    i = 5; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);
    i = 6; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);
    i = 7; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);
    i = 8; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);
    i = 9; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);
    i = 10; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);
    i = 11; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);
    i = 12; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);
    i = 13; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);
    i = 14; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);
    i = 15; dma->dst_addr[i].d = mux2(mux2(dma->dst_addr[i].q, dst_plus1[i], xfer), dma->dst_in[i], wr_dst);

    i = 0; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);
    i = 1; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);
    i = 2; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);
    i = 3; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);
    i = 4; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);
    i = 5; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);
    i = 6; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);
    i = 7; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);
    i = 8; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);
    i = 9; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);
    i = 10; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);
    i = 11; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);
    i = 12; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);
    i = 13; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);
    i = 14; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);
    i = 15; dma->count[i].d = mux2(mux2(dma->count[i].q, count_minus1[i], xfer), dma->count_in[i], wr_cnt);

    dma->control[0].d = mux2(dma->control[0].q, dma->control_in[0], wr_ctrl);
    dma->control[1].d = mux2(dma->control[1].q, dma->control_in[1], wr_ctrl);
    dma->control[2].d = mux2(dma->control[2].q, dma->control_in[2], wr_ctrl);
    dma->control[3].d = mux2(dma->control[3].q, dma->control_in[3], wr_ctrl);
    dma->control[4].d = mux2(dma->control[4].q, dma->control_in[4], wr_ctrl);
    dma->control[5].d = mux2(dma->control[5].q, dma->control_in[5], wr_ctrl);
    dma->control[6].d = mux2(dma->control[6].q, dma->control_in[6], wr_ctrl);
    dma->control[7].d = mux2(dma->control[7].q, dma->control_in[7], wr_ctrl);

    bit enabled = dma->control[0].q;
    bit cnt_zero = AND(AND(AND(AND(NOT(dma->count[0].q), NOT(dma->count[1].q)),
                               AND(NOT(dma->count[2].q), NOT(dma->count[3].q))),
                           AND(AND(NOT(dma->count[4].q), NOT(dma->count[5].q)),
                               AND(NOT(dma->count[6].q), NOT(dma->count[7].q)))),
                       AND(AND(AND(NOT(dma->count[8].q), NOT(dma->count[9].q)),
                               AND(NOT(dma->count[10].q), NOT(dma->count[11].q))),
                           AND(AND(NOT(dma->count[12].q), NOT(dma->count[13].q)),
                               AND(NOT(dma->count[14].q), NOT(dma->count[15].q)))));

    dma->status[0].d = enabled;
    dma->status[1].d = dma->bus_grant;
    dma->status[2].d = cnt_zero;
    dma->status[3].d = 0;
    dma->status[4].d = 0;
    dma->status[5].d = 0;
    dma->status[6].d = 0;
    dma->status[7].d = 0;

    i = 0; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);
    i = 1; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);
    i = 2; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);
    i = 3; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);
    i = 4; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);
    i = 5; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);
    i = 6; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);
    i = 7; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);
    i = 8; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);
    i = 9; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);
    i = 10; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);
    i = 11; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);
    i = 12; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);
    i = 13; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);
    i = 14; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);
    i = 15; dff_clock_fall(&dma->src_addr[i]); dff_clock_fall(&dma->dst_addr[i]); dff_clock_fall(&dma->count[i]);

    dff_clock_fall(&dma->control[0]); dff_clock_fall(&dma->control[1]);
    dff_clock_fall(&dma->control[2]); dff_clock_fall(&dma->control[3]);
    dff_clock_fall(&dma->control[4]); dff_clock_fall(&dma->control[5]);
    dff_clock_fall(&dma->control[6]); dff_clock_fall(&dma->control[7]);

    dff_clock_fall(&dma->status[0]); dff_clock_fall(&dma->status[1]);
    dff_clock_fall(&dma->status[2]); dff_clock_fall(&dma->status[3]);
    dff_clock_fall(&dma->status[4]); dff_clock_fall(&dma->status[5]);
    dff_clock_fall(&dma->status[6]); dff_clock_fall(&dma->status[7]);
}

void dma_propagate(dma_controller_t *dma) {
    dma->src_out[0] = dma->src_addr[0].q; dma->src_out[1] = dma->src_addr[1].q;
    dma->src_out[2] = dma->src_addr[2].q; dma->src_out[3] = dma->src_addr[3].q;
    dma->src_out[4] = dma->src_addr[4].q; dma->src_out[5] = dma->src_addr[5].q;
    dma->src_out[6] = dma->src_addr[6].q; dma->src_out[7] = dma->src_addr[7].q;
    dma->src_out[8] = dma->src_addr[8].q; dma->src_out[9] = dma->src_addr[9].q;
    dma->src_out[10] = dma->src_addr[10].q; dma->src_out[11] = dma->src_addr[11].q;
    dma->src_out[12] = dma->src_addr[12].q; dma->src_out[13] = dma->src_addr[13].q;
    dma->src_out[14] = dma->src_addr[14].q; dma->src_out[15] = dma->src_addr[15].q;

    dma->dst_out[0] = dma->dst_addr[0].q; dma->dst_out[1] = dma->dst_addr[1].q;
    dma->dst_out[2] = dma->dst_addr[2].q; dma->dst_out[3] = dma->dst_addr[3].q;
    dma->dst_out[4] = dma->dst_addr[4].q; dma->dst_out[5] = dma->dst_addr[5].q;
    dma->dst_out[6] = dma->dst_addr[6].q; dma->dst_out[7] = dma->dst_addr[7].q;
    dma->dst_out[8] = dma->dst_addr[8].q; dma->dst_out[9] = dma->dst_addr[9].q;
    dma->dst_out[10] = dma->dst_addr[10].q; dma->dst_out[11] = dma->dst_addr[11].q;
    dma->dst_out[12] = dma->dst_addr[12].q; dma->dst_out[13] = dma->dst_addr[13].q;
    dma->dst_out[14] = dma->dst_addr[14].q; dma->dst_out[15] = dma->dst_addr[15].q;

    dma->count_out[0] = dma->count[0].q; dma->count_out[1] = dma->count[1].q;
    dma->count_out[2] = dma->count[2].q; dma->count_out[3] = dma->count[3].q;
    dma->count_out[4] = dma->count[4].q; dma->count_out[5] = dma->count[5].q;
    dma->count_out[6] = dma->count[6].q; dma->count_out[7] = dma->count[7].q;
    dma->count_out[8] = dma->count[8].q; dma->count_out[9] = dma->count[9].q;
    dma->count_out[10] = dma->count[10].q; dma->count_out[11] = dma->count[11].q;
    dma->count_out[12] = dma->count[12].q; dma->count_out[13] = dma->count[13].q;
    dma->count_out[14] = dma->count[14].q; dma->count_out[15] = dma->count[15].q;

    dma->status_out[0] = dma->status[0].q; dma->status_out[1] = dma->status[1].q;
    dma->status_out[2] = dma->status[2].q; dma->status_out[3] = dma->status[3].q;
    dma->status_out[4] = dma->status[4].q; dma->status_out[5] = dma->status[5].q;
    dma->status_out[6] = dma->status[6].q; dma->status_out[7] = dma->status[7].q;

    bit enabled = dma->control[0].q;
    dma->bus_request = AND(enabled, dma->dreq);
    dma->dack = AND(dma->bus_grant, enabled);
    dma->tc = dma->status[2].q;
    dma->mem_read = AND(dma->dack, dma->control[1].q);
    dma->mem_write = AND(dma->dack, NOT(dma->control[1].q));

    dma->addr_out[0] = dma->src_out[0]; dma->addr_out[1] = dma->src_out[1];
    dma->addr_out[2] = dma->src_out[2]; dma->addr_out[3] = dma->src_out[3];
    dma->addr_out[4] = dma->src_out[4]; dma->addr_out[5] = dma->src_out[5];
    dma->addr_out[6] = dma->src_out[6]; dma->addr_out[7] = dma->src_out[7];
    dma->addr_out[8] = dma->src_out[8]; dma->addr_out[9] = dma->src_out[9];
    dma->addr_out[10] = dma->src_out[10]; dma->addr_out[11] = dma->src_out[11];
    dma->addr_out[12] = dma->src_out[12]; dma->addr_out[13] = dma->src_out[13];
    dma->addr_out[14] = dma->src_out[14]; dma->addr_out[15] = dma->src_out[15];

    dma->data_out[0] = dma->data_in[0]; dma->data_out[1] = dma->data_in[1];
    dma->data_out[2] = dma->data_in[2]; dma->data_out[3] = dma->data_in[3];
    dma->data_out[4] = dma->data_in[4]; dma->data_out[5] = dma->data_in[5];
    dma->data_out[6] = dma->data_in[6]; dma->data_out[7] = dma->data_in[7];
    dma->data_out[8] = dma->data_in[8]; dma->data_out[9] = dma->data_in[9];
    dma->data_out[10] = dma->data_in[10]; dma->data_out[11] = dma->data_in[11];
    dma->data_out[12] = dma->data_in[12]; dma->data_out[13] = dma->data_in[13];
    dma->data_out[14] = dma->data_in[14]; dma->data_out[15] = dma->data_in[15];
    dma->data_out[16] = dma->data_in[16]; dma->data_out[17] = dma->data_in[17];
    dma->data_out[18] = dma->data_in[18]; dma->data_out[19] = dma->data_in[19];
    dma->data_out[20] = dma->data_in[20]; dma->data_out[21] = dma->data_in[21];
    dma->data_out[22] = dma->data_in[22]; dma->data_out[23] = dma->data_in[23];
    dma->data_out[24] = dma->data_in[24]; dma->data_out[25] = dma->data_in[25];
    dma->data_out[26] = dma->data_in[26]; dma->data_out[27] = dma->data_in[27];
    dma->data_out[28] = dma->data_in[28]; dma->data_out[29] = dma->data_in[29];
    dma->data_out[30] = dma->data_in[30]; dma->data_out[31] = dma->data_in[31];
}

void dma_read_data(dma_controller_t *dma, bit *out) {
    out[0] = dma->data_out[0]; out[1] = dma->data_out[1];
    out[2] = dma->data_out[2]; out[3] = dma->data_out[3];
    out[4] = dma->data_out[4]; out[5] = dma->data_out[5];
    out[6] = dma->data_out[6]; out[7] = dma->data_out[7];
    out[8] = dma->data_out[8]; out[9] = dma->data_out[9];
    out[10] = dma->data_out[10]; out[11] = dma->data_out[11];
    out[12] = dma->data_out[12]; out[13] = dma->data_out[13];
    out[14] = dma->data_out[14]; out[15] = dma->data_out[15];
    out[16] = dma->data_out[16]; out[17] = dma->data_out[17];
    out[18] = dma->data_out[18]; out[19] = dma->data_out[19];
    out[20] = dma->data_out[20]; out[21] = dma->data_out[21];
    out[22] = dma->data_out[22]; out[23] = dma->data_out[23];
    out[24] = dma->data_out[24]; out[25] = dma->data_out[25];
    out[26] = dma->data_out[26]; out[27] = dma->data_out[27];
    out[28] = dma->data_out[28]; out[29] = dma->data_out[29];
    out[30] = dma->data_out[30]; out[31] = dma->data_out[31];
}

void dma_read_addr(dma_controller_t *dma, bit *out) {
    out[0] = dma->addr_out[0]; out[1] = dma->addr_out[1];
    out[2] = dma->addr_out[2]; out[3] = dma->addr_out[3];
    out[4] = dma->addr_out[4]; out[5] = dma->addr_out[5];
    out[6] = dma->addr_out[6]; out[7] = dma->addr_out[7];
    out[8] = dma->addr_out[8]; out[9] = dma->addr_out[9];
    out[10] = dma->addr_out[10]; out[11] = dma->addr_out[11];
    out[12] = dma->addr_out[12]; out[13] = dma->addr_out[13];
    out[14] = dma->addr_out[14]; out[15] = dma->addr_out[15];
}

bit dma_get_bus_request(dma_controller_t *dma) {
    return dma->bus_request;
}

bit dma_get_dack(dma_controller_t *dma) {
    return dma->dack;
}

bit dma_get_tc(dma_controller_t *dma) {
    return dma->tc;
}

bit dma_get_mem_read(dma_controller_t *dma) {
    return dma->mem_read;
}

bit dma_get_mem_write(dma_controller_t *dma) {
    return dma->mem_write;
}
