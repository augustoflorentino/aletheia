#include "io_controller.h"
#include "../core/gates/and.h"
#include "../core/gates/or.h"
#include "../core/gates/not.h"
#include "../core/mux/mux2.h"

void io_ctrl_init(io_controller_t *ctrl) {
    dff_reset(&ctrl->port_select[0]);
    dff_reset(&ctrl->port_select[1]);
    dff_reset(&ctrl->port_select[2]);
    dff_reset(&ctrl->read_strobe);
    dff_reset(&ctrl->write_strobe);

    int i;
    i = 0; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 1; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 2; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 3; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 4; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 5; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 6; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 7; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 8; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 9; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 10; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 11; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 12; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 13; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 14; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 15; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 16; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 17; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 18; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 19; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 20; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 21; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 22; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 23; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 24; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 25; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 26; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 27; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 28; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 29; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 30; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;
    i = 31; dff_reset(&ctrl->data[i]); ctrl->data_in[i] = 0; ctrl->data_out[i] = 0;

    i = 0; ctrl->addr_in[i] = 0;
    i = 1; ctrl->addr_in[i] = 0;
    i = 2; ctrl->addr_in[i] = 0;
    i = 3; ctrl->addr_in[i] = 0;
    i = 4; ctrl->addr_in[i] = 0;
    i = 5; ctrl->addr_in[i] = 0;
    i = 6; ctrl->addr_in[i] = 0;
    i = 7; ctrl->addr_in[i] = 0;
    i = 8; ctrl->addr_in[i] = 0;
    i = 9; ctrl->addr_in[i] = 0;
    i = 10; ctrl->addr_in[i] = 0;
    i = 11; ctrl->addr_in[i] = 0;
    i = 12; ctrl->addr_in[i] = 0;
    i = 13; ctrl->addr_in[i] = 0;
    i = 14; ctrl->addr_in[i] = 0;
    i = 15; ctrl->addr_in[i] = 0;

    ctrl->read_in = 0;
    ctrl->write_in = 0;
    ctrl->chip_select = 0;

    ctrl->port_cs[0] = 0; ctrl->port_cs[1] = 0;
    ctrl->port_cs[2] = 0; ctrl->port_cs[3] = 0;
    ctrl->port_cs[4] = 0; ctrl->port_cs[5] = 0;
    ctrl->port_cs[6] = 0; ctrl->port_cs[7] = 0;

    ctrl->port_read[0] = 0; ctrl->port_read[1] = 0;
    ctrl->port_read[2] = 0; ctrl->port_read[3] = 0;
    ctrl->port_read[4] = 0; ctrl->port_read[5] = 0;
    ctrl->port_read[6] = 0; ctrl->port_read[7] = 0;

    ctrl->port_write[0] = 0; ctrl->port_write[1] = 0;
    ctrl->port_write[2] = 0; ctrl->port_write[3] = 0;
    ctrl->port_write[4] = 0; ctrl->port_write[5] = 0;
    ctrl->port_write[6] = 0; ctrl->port_write[7] = 0;

    ctrl->port_addr[0] = 0; ctrl->port_addr[1] = 0;
    ctrl->port_addr[2] = 0; ctrl->port_addr[3] = 0;

    ctrl->ready = 0;
}

void io_ctrl_reset(io_controller_t *ctrl) {
    io_ctrl_init(ctrl);
}

void io_ctrl_set_addr(io_controller_t *ctrl, bit *addr) {
    ctrl->addr_in[0] = addr[0]; ctrl->addr_in[1] = addr[1];
    ctrl->addr_in[2] = addr[2]; ctrl->addr_in[3] = addr[3];
    ctrl->addr_in[4] = addr[4]; ctrl->addr_in[5] = addr[5];
    ctrl->addr_in[6] = addr[6]; ctrl->addr_in[7] = addr[7];
    ctrl->addr_in[8] = addr[8]; ctrl->addr_in[9] = addr[9];
    ctrl->addr_in[10] = addr[10]; ctrl->addr_in[11] = addr[11];
    ctrl->addr_in[12] = addr[12]; ctrl->addr_in[13] = addr[13];
    ctrl->addr_in[14] = addr[14]; ctrl->addr_in[15] = addr[15];
}

void io_ctrl_set_data(io_controller_t *ctrl, bit *data) {
    ctrl->data_in[0] = data[0]; ctrl->data_in[1] = data[1];
    ctrl->data_in[2] = data[2]; ctrl->data_in[3] = data[3];
    ctrl->data_in[4] = data[4]; ctrl->data_in[5] = data[5];
    ctrl->data_in[6] = data[6]; ctrl->data_in[7] = data[7];
    ctrl->data_in[8] = data[8]; ctrl->data_in[9] = data[9];
    ctrl->data_in[10] = data[10]; ctrl->data_in[11] = data[11];
    ctrl->data_in[12] = data[12]; ctrl->data_in[13] = data[13];
    ctrl->data_in[14] = data[14]; ctrl->data_in[15] = data[15];
    ctrl->data_in[16] = data[16]; ctrl->data_in[17] = data[17];
    ctrl->data_in[18] = data[18]; ctrl->data_in[19] = data[19];
    ctrl->data_in[20] = data[20]; ctrl->data_in[21] = data[21];
    ctrl->data_in[22] = data[22]; ctrl->data_in[23] = data[23];
    ctrl->data_in[24] = data[24]; ctrl->data_in[25] = data[25];
    ctrl->data_in[26] = data[26]; ctrl->data_in[27] = data[27];
    ctrl->data_in[28] = data[28]; ctrl->data_in[29] = data[29];
    ctrl->data_in[30] = data[30]; ctrl->data_in[31] = data[31];
}

void io_ctrl_set_read(io_controller_t *ctrl, bit rd) {
    ctrl->read_in = rd;
}

void io_ctrl_set_write(io_controller_t *ctrl, bit wr) {
    ctrl->write_in = wr;
}

void io_ctrl_set_cs(io_controller_t *ctrl, bit cs) {
    ctrl->chip_select = cs;
}

void io_ctrl_clock_rise(io_controller_t *ctrl) {
    dff_clock_rise(&ctrl->port_select[0]);
    dff_clock_rise(&ctrl->port_select[1]);
    dff_clock_rise(&ctrl->port_select[2]);
    dff_clock_rise(&ctrl->read_strobe);
    dff_clock_rise(&ctrl->write_strobe);

    int i;
    i = 0; dff_clock_rise(&ctrl->data[i]);
    i = 1; dff_clock_rise(&ctrl->data[i]);
    i = 2; dff_clock_rise(&ctrl->data[i]);
    i = 3; dff_clock_rise(&ctrl->data[i]);
    i = 4; dff_clock_rise(&ctrl->data[i]);
    i = 5; dff_clock_rise(&ctrl->data[i]);
    i = 6; dff_clock_rise(&ctrl->data[i]);
    i = 7; dff_clock_rise(&ctrl->data[i]);
    i = 8; dff_clock_rise(&ctrl->data[i]);
    i = 9; dff_clock_rise(&ctrl->data[i]);
    i = 10; dff_clock_rise(&ctrl->data[i]);
    i = 11; dff_clock_rise(&ctrl->data[i]);
    i = 12; dff_clock_rise(&ctrl->data[i]);
    i = 13; dff_clock_rise(&ctrl->data[i]);
    i = 14; dff_clock_rise(&ctrl->data[i]);
    i = 15; dff_clock_rise(&ctrl->data[i]);
    i = 16; dff_clock_rise(&ctrl->data[i]);
    i = 17; dff_clock_rise(&ctrl->data[i]);
    i = 18; dff_clock_rise(&ctrl->data[i]);
    i = 19; dff_clock_rise(&ctrl->data[i]);
    i = 20; dff_clock_rise(&ctrl->data[i]);
    i = 21; dff_clock_rise(&ctrl->data[i]);
    i = 22; dff_clock_rise(&ctrl->data[i]);
    i = 23; dff_clock_rise(&ctrl->data[i]);
    i = 24; dff_clock_rise(&ctrl->data[i]);
    i = 25; dff_clock_rise(&ctrl->data[i]);
    i = 26; dff_clock_rise(&ctrl->data[i]);
    i = 27; dff_clock_rise(&ctrl->data[i]);
    i = 28; dff_clock_rise(&ctrl->data[i]);
    i = 29; dff_clock_rise(&ctrl->data[i]);
    i = 30; dff_clock_rise(&ctrl->data[i]);
    i = 31; dff_clock_rise(&ctrl->data[i]);
}

void io_ctrl_clock_fall(io_controller_t *ctrl) {
    bit active = ctrl->chip_select;

    ctrl->port_select[0].d = AND(ctrl->addr_in[4], active);
    ctrl->port_select[1].d = AND(ctrl->addr_in[5], active);
    ctrl->port_select[2].d = AND(ctrl->addr_in[6], active);
    ctrl->read_strobe.d = AND(ctrl->read_in, active);
    ctrl->write_strobe.d = AND(ctrl->write_in, active);

    bit wr = AND(ctrl->write_in, active);
    ctrl->data[0].d = mux2(ctrl->data[0].q, ctrl->data_in[0], wr);
    ctrl->data[1].d = mux2(ctrl->data[1].q, ctrl->data_in[1], wr);
    ctrl->data[2].d = mux2(ctrl->data[2].q, ctrl->data_in[2], wr);
    ctrl->data[3].d = mux2(ctrl->data[3].q, ctrl->data_in[3], wr);
    ctrl->data[4].d = mux2(ctrl->data[4].q, ctrl->data_in[4], wr);
    ctrl->data[5].d = mux2(ctrl->data[5].q, ctrl->data_in[5], wr);
    ctrl->data[6].d = mux2(ctrl->data[6].q, ctrl->data_in[6], wr);
    ctrl->data[7].d = mux2(ctrl->data[7].q, ctrl->data_in[7], wr);
    ctrl->data[8].d = mux2(ctrl->data[8].q, ctrl->data_in[8], wr);
    ctrl->data[9].d = mux2(ctrl->data[9].q, ctrl->data_in[9], wr);
    ctrl->data[10].d = mux2(ctrl->data[10].q, ctrl->data_in[10], wr);
    ctrl->data[11].d = mux2(ctrl->data[11].q, ctrl->data_in[11], wr);
    ctrl->data[12].d = mux2(ctrl->data[12].q, ctrl->data_in[12], wr);
    ctrl->data[13].d = mux2(ctrl->data[13].q, ctrl->data_in[13], wr);
    ctrl->data[14].d = mux2(ctrl->data[14].q, ctrl->data_in[14], wr);
    ctrl->data[15].d = mux2(ctrl->data[15].q, ctrl->data_in[15], wr);
    ctrl->data[16].d = mux2(ctrl->data[16].q, ctrl->data_in[16], wr);
    ctrl->data[17].d = mux2(ctrl->data[17].q, ctrl->data_in[17], wr);
    ctrl->data[18].d = mux2(ctrl->data[18].q, ctrl->data_in[18], wr);
    ctrl->data[19].d = mux2(ctrl->data[19].q, ctrl->data_in[19], wr);
    ctrl->data[20].d = mux2(ctrl->data[20].q, ctrl->data_in[20], wr);
    ctrl->data[21].d = mux2(ctrl->data[21].q, ctrl->data_in[21], wr);
    ctrl->data[22].d = mux2(ctrl->data[22].q, ctrl->data_in[22], wr);
    ctrl->data[23].d = mux2(ctrl->data[23].q, ctrl->data_in[23], wr);
    ctrl->data[24].d = mux2(ctrl->data[24].q, ctrl->data_in[24], wr);
    ctrl->data[25].d = mux2(ctrl->data[25].q, ctrl->data_in[25], wr);
    ctrl->data[26].d = mux2(ctrl->data[26].q, ctrl->data_in[26], wr);
    ctrl->data[27].d = mux2(ctrl->data[27].q, ctrl->data_in[27], wr);
    ctrl->data[28].d = mux2(ctrl->data[28].q, ctrl->data_in[28], wr);
    ctrl->data[29].d = mux2(ctrl->data[29].q, ctrl->data_in[29], wr);
    ctrl->data[30].d = mux2(ctrl->data[30].q, ctrl->data_in[30], wr);
    ctrl->data[31].d = mux2(ctrl->data[31].q, ctrl->data_in[31], wr);

    dff_clock_fall(&ctrl->port_select[0]);
    dff_clock_fall(&ctrl->port_select[1]);
    dff_clock_fall(&ctrl->port_select[2]);
    dff_clock_fall(&ctrl->read_strobe);
    dff_clock_fall(&ctrl->write_strobe);

    int i;
    i = 0; dff_clock_fall(&ctrl->data[i]);
    i = 1; dff_clock_fall(&ctrl->data[i]);
    i = 2; dff_clock_fall(&ctrl->data[i]);
    i = 3; dff_clock_fall(&ctrl->data[i]);
    i = 4; dff_clock_fall(&ctrl->data[i]);
    i = 5; dff_clock_fall(&ctrl->data[i]);
    i = 6; dff_clock_fall(&ctrl->data[i]);
    i = 7; dff_clock_fall(&ctrl->data[i]);
    i = 8; dff_clock_fall(&ctrl->data[i]);
    i = 9; dff_clock_fall(&ctrl->data[i]);
    i = 10; dff_clock_fall(&ctrl->data[i]);
    i = 11; dff_clock_fall(&ctrl->data[i]);
    i = 12; dff_clock_fall(&ctrl->data[i]);
    i = 13; dff_clock_fall(&ctrl->data[i]);
    i = 14; dff_clock_fall(&ctrl->data[i]);
    i = 15; dff_clock_fall(&ctrl->data[i]);
    i = 16; dff_clock_fall(&ctrl->data[i]);
    i = 17; dff_clock_fall(&ctrl->data[i]);
    i = 18; dff_clock_fall(&ctrl->data[i]);
    i = 19; dff_clock_fall(&ctrl->data[i]);
    i = 20; dff_clock_fall(&ctrl->data[i]);
    i = 21; dff_clock_fall(&ctrl->data[i]);
    i = 22; dff_clock_fall(&ctrl->data[i]);
    i = 23; dff_clock_fall(&ctrl->data[i]);
    i = 24; dff_clock_fall(&ctrl->data[i]);
    i = 25; dff_clock_fall(&ctrl->data[i]);
    i = 26; dff_clock_fall(&ctrl->data[i]);
    i = 27; dff_clock_fall(&ctrl->data[i]);
    i = 28; dff_clock_fall(&ctrl->data[i]);
    i = 29; dff_clock_fall(&ctrl->data[i]);
    i = 30; dff_clock_fall(&ctrl->data[i]);
    i = 31; dff_clock_fall(&ctrl->data[i]);
}

void io_ctrl_propagate(io_controller_t *ctrl) {
    bit s0 = ctrl->port_select[0].q;
    bit s1 = ctrl->port_select[1].q;
    bit s2 = ctrl->port_select[2].q;
    bit ns0 = NOT(s0);
    bit ns1 = NOT(s1);
    bit ns2 = NOT(s2);

    ctrl->port_cs[0] = AND(AND(ns2, ns1), ns0);
    ctrl->port_cs[1] = AND(AND(ns2, ns1), s0);
    ctrl->port_cs[2] = AND(AND(ns2, s1), ns0);
    ctrl->port_cs[3] = AND(AND(ns2, s1), s0);
    ctrl->port_cs[4] = AND(AND(s2, ns1), ns0);
    ctrl->port_cs[5] = AND(AND(s2, ns1), s0);
    ctrl->port_cs[6] = AND(AND(s2, s1), ns0);
    ctrl->port_cs[7] = AND(AND(s2, s1), s0);

    bit rd = ctrl->read_strobe.q;
    bit wr = ctrl->write_strobe.q;

    ctrl->port_read[0] = AND(ctrl->port_cs[0], rd);
    ctrl->port_read[1] = AND(ctrl->port_cs[1], rd);
    ctrl->port_read[2] = AND(ctrl->port_cs[2], rd);
    ctrl->port_read[3] = AND(ctrl->port_cs[3], rd);
    ctrl->port_read[4] = AND(ctrl->port_cs[4], rd);
    ctrl->port_read[5] = AND(ctrl->port_cs[5], rd);
    ctrl->port_read[6] = AND(ctrl->port_cs[6], rd);
    ctrl->port_read[7] = AND(ctrl->port_cs[7], rd);

    ctrl->port_write[0] = AND(ctrl->port_cs[0], wr);
    ctrl->port_write[1] = AND(ctrl->port_cs[1], wr);
    ctrl->port_write[2] = AND(ctrl->port_cs[2], wr);
    ctrl->port_write[3] = AND(ctrl->port_cs[3], wr);
    ctrl->port_write[4] = AND(ctrl->port_cs[4], wr);
    ctrl->port_write[5] = AND(ctrl->port_cs[5], wr);
    ctrl->port_write[6] = AND(ctrl->port_cs[6], wr);
    ctrl->port_write[7] = AND(ctrl->port_cs[7], wr);

    ctrl->port_addr[0] = ctrl->addr_in[0];
    ctrl->port_addr[1] = ctrl->addr_in[1];
    ctrl->port_addr[2] = ctrl->addr_in[2];
    ctrl->port_addr[3] = ctrl->addr_in[3];

    ctrl->data_out[0] = ctrl->data[0].q; ctrl->data_out[1] = ctrl->data[1].q;
    ctrl->data_out[2] = ctrl->data[2].q; ctrl->data_out[3] = ctrl->data[3].q;
    ctrl->data_out[4] = ctrl->data[4].q; ctrl->data_out[5] = ctrl->data[5].q;
    ctrl->data_out[6] = ctrl->data[6].q; ctrl->data_out[7] = ctrl->data[7].q;
    ctrl->data_out[8] = ctrl->data[8].q; ctrl->data_out[9] = ctrl->data[9].q;
    ctrl->data_out[10] = ctrl->data[10].q; ctrl->data_out[11] = ctrl->data[11].q;
    ctrl->data_out[12] = ctrl->data[12].q; ctrl->data_out[13] = ctrl->data[13].q;
    ctrl->data_out[14] = ctrl->data[14].q; ctrl->data_out[15] = ctrl->data[15].q;
    ctrl->data_out[16] = ctrl->data[16].q; ctrl->data_out[17] = ctrl->data[17].q;
    ctrl->data_out[18] = ctrl->data[18].q; ctrl->data_out[19] = ctrl->data[19].q;
    ctrl->data_out[20] = ctrl->data[20].q; ctrl->data_out[21] = ctrl->data[21].q;
    ctrl->data_out[22] = ctrl->data[22].q; ctrl->data_out[23] = ctrl->data[23].q;
    ctrl->data_out[24] = ctrl->data[24].q; ctrl->data_out[25] = ctrl->data[25].q;
    ctrl->data_out[26] = ctrl->data[26].q; ctrl->data_out[27] = ctrl->data[27].q;
    ctrl->data_out[28] = ctrl->data[28].q; ctrl->data_out[29] = ctrl->data[29].q;
    ctrl->data_out[30] = ctrl->data[30].q; ctrl->data_out[31] = ctrl->data[31].q;

    ctrl->ready = OR(rd, wr);
}

void io_ctrl_read_data(io_controller_t *ctrl, bit *out) {
    out[0] = ctrl->data_out[0]; out[1] = ctrl->data_out[1];
    out[2] = ctrl->data_out[2]; out[3] = ctrl->data_out[3];
    out[4] = ctrl->data_out[4]; out[5] = ctrl->data_out[5];
    out[6] = ctrl->data_out[6]; out[7] = ctrl->data_out[7];
    out[8] = ctrl->data_out[8]; out[9] = ctrl->data_out[9];
    out[10] = ctrl->data_out[10]; out[11] = ctrl->data_out[11];
    out[12] = ctrl->data_out[12]; out[13] = ctrl->data_out[13];
    out[14] = ctrl->data_out[14]; out[15] = ctrl->data_out[15];
    out[16] = ctrl->data_out[16]; out[17] = ctrl->data_out[17];
    out[18] = ctrl->data_out[18]; out[19] = ctrl->data_out[19];
    out[20] = ctrl->data_out[20]; out[21] = ctrl->data_out[21];
    out[22] = ctrl->data_out[22]; out[23] = ctrl->data_out[23];
    out[24] = ctrl->data_out[24]; out[25] = ctrl->data_out[25];
    out[26] = ctrl->data_out[26]; out[27] = ctrl->data_out[27];
    out[28] = ctrl->data_out[28]; out[29] = ctrl->data_out[29];
    out[30] = ctrl->data_out[30]; out[31] = ctrl->data_out[31];
}

bit io_ctrl_get_port_cs(io_controller_t *ctrl, bit s0, bit s1, bit s2) {
    bit ns0 = NOT(s0); bit ns1 = NOT(s1); bit ns2 = NOT(s2);
    bit sel0 = AND(AND(ns2, ns1), ns0);
    bit sel1 = AND(AND(ns2, ns1), s0);
    bit sel2 = AND(AND(ns2, s1), ns0);
    bit sel3 = AND(AND(ns2, s1), s0);
    bit sel4 = AND(AND(s2, ns1), ns0);
    bit sel5 = AND(AND(s2, ns1), s0);
    bit sel6 = AND(AND(s2, s1), ns0);
    bit sel7 = AND(AND(s2, s1), s0);

    bit t0 = OR(AND(ctrl->port_cs[0], sel0), AND(ctrl->port_cs[1], sel1));
    bit t1 = OR(AND(ctrl->port_cs[2], sel2), AND(ctrl->port_cs[3], sel3));
    bit t2 = OR(AND(ctrl->port_cs[4], sel4), AND(ctrl->port_cs[5], sel5));
    bit t3 = OR(AND(ctrl->port_cs[6], sel6), AND(ctrl->port_cs[7], sel7));
    return OR(OR(t0, t1), OR(t2, t3));
}

bit io_ctrl_get_port_read(io_controller_t *ctrl, bit s0, bit s1, bit s2) {
    bit ns0 = NOT(s0); bit ns1 = NOT(s1); bit ns2 = NOT(s2);
    bit sel0 = AND(AND(ns2, ns1), ns0);
    bit sel1 = AND(AND(ns2, ns1), s0);
    bit sel2 = AND(AND(ns2, s1), ns0);
    bit sel3 = AND(AND(ns2, s1), s0);
    bit sel4 = AND(AND(s2, ns1), ns0);
    bit sel5 = AND(AND(s2, ns1), s0);
    bit sel6 = AND(AND(s2, s1), ns0);
    bit sel7 = AND(AND(s2, s1), s0);

    bit t0 = OR(AND(ctrl->port_read[0], sel0), AND(ctrl->port_read[1], sel1));
    bit t1 = OR(AND(ctrl->port_read[2], sel2), AND(ctrl->port_read[3], sel3));
    bit t2 = OR(AND(ctrl->port_read[4], sel4), AND(ctrl->port_read[5], sel5));
    bit t3 = OR(AND(ctrl->port_read[6], sel6), AND(ctrl->port_read[7], sel7));
    return OR(OR(t0, t1), OR(t2, t3));
}

bit io_ctrl_get_port_write(io_controller_t *ctrl, bit s0, bit s1, bit s2) {
    bit ns0 = NOT(s0); bit ns1 = NOT(s1); bit ns2 = NOT(s2);
    bit sel0 = AND(AND(ns2, ns1), ns0);
    bit sel1 = AND(AND(ns2, ns1), s0);
    bit sel2 = AND(AND(ns2, s1), ns0);
    bit sel3 = AND(AND(ns2, s1), s0);
    bit sel4 = AND(AND(s2, ns1), ns0);
    bit sel5 = AND(AND(s2, ns1), s0);
    bit sel6 = AND(AND(s2, s1), ns0);
    bit sel7 = AND(AND(s2, s1), s0);

    bit t0 = OR(AND(ctrl->port_write[0], sel0), AND(ctrl->port_write[1], sel1));
    bit t1 = OR(AND(ctrl->port_write[2], sel2), AND(ctrl->port_write[3], sel3));
    bit t2 = OR(AND(ctrl->port_write[4], sel4), AND(ctrl->port_write[5], sel5));
    bit t3 = OR(AND(ctrl->port_write[6], sel6), AND(ctrl->port_write[7], sel7));
    return OR(OR(t0, t1), OR(t2, t3));
}
