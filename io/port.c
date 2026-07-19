#include "port.h"
#include "../core/gates/and.h"
#include "../core/gates/or.h"
#include "../core/gates/not.h"
#include "../core/gates/xor.h"
#include "../core/mux/mux2.h"

void port_init(io_port_t *port) {
    dff_reset(&port->data[0]); dff_reset(&port->data[1]);
    dff_reset(&port->data[2]); dff_reset(&port->data[3]);
    dff_reset(&port->data[4]); dff_reset(&port->data[5]);
    dff_reset(&port->data[6]); dff_reset(&port->data[7]);

    dff_reset(&port->direction[0]); dff_reset(&port->direction[1]);
    dff_reset(&port->direction[2]); dff_reset(&port->direction[3]);
    dff_reset(&port->direction[4]); dff_reset(&port->direction[5]);
    dff_reset(&port->direction[6]); dff_reset(&port->direction[7]);

    dff_reset(&port->irq_enable);
    dff_reset(&port->irq_pending);

    port->data_in[0] = 0; port->data_in[1] = 0;
    port->data_in[2] = 0; port->data_in[3] = 0;
    port->data_in[4] = 0; port->data_in[5] = 0;
    port->data_in[6] = 0; port->data_in[7] = 0;

    port->data_out[0] = 0; port->data_out[1] = 0;
    port->data_out[2] = 0; port->data_out[3] = 0;
    port->data_out[4] = 0; port->data_out[5] = 0;
    port->data_out[6] = 0; port->data_out[7] = 0;

    port->direction_in[0] = 0; port->direction_in[1] = 0;
    port->direction_in[2] = 0; port->direction_in[3] = 0;
    port->direction_in[4] = 0; port->direction_in[5] = 0;
    port->direction_in[6] = 0; port->direction_in[7] = 0;

    port->external_in[0] = 0; port->external_in[1] = 0;
    port->external_in[2] = 0; port->external_in[3] = 0;
    port->external_in[4] = 0; port->external_in[5] = 0;
    port->external_in[6] = 0; port->external_in[7] = 0;

    port->external_out[0] = 0; port->external_out[1] = 0;
    port->external_out[2] = 0; port->external_out[3] = 0;
    port->external_out[4] = 0; port->external_out[5] = 0;
    port->external_out[6] = 0; port->external_out[7] = 0;

    port->chip_select = 0;
    port->read = 0;
    port->write = 0;
    port->addr[0] = 0;
    port->addr[1] = 0;
    port->irq = 0;
}

void port_reset(io_port_t *port) {
    port_init(port);
}

void port_set_cs(io_port_t *port, bit cs) {
    port->chip_select = cs;
}

void port_set_read(io_port_t *port, bit rd) {
    port->read = rd;
}

void port_set_write(io_port_t *port, bit wr) {
    port->write = wr;
}

void port_set_addr(io_port_t *port, bit a0, bit a1) {
    port->addr[0] = a0;
    port->addr[1] = a1;
}

void port_set_data(io_port_t *port, bit *data) {
    port->data_in[0] = data[0]; port->data_in[1] = data[1];
    port->data_in[2] = data[2]; port->data_in[3] = data[3];
    port->data_in[4] = data[4]; port->data_in[5] = data[5];
    port->data_in[6] = data[6]; port->data_in[7] = data[7];
}

void port_set_external(io_port_t *port, bit *ext) {
    port->external_in[0] = ext[0]; port->external_in[1] = ext[1];
    port->external_in[2] = ext[2]; port->external_in[3] = ext[3];
    port->external_in[4] = ext[4]; port->external_in[5] = ext[5];
    port->external_in[6] = ext[6]; port->external_in[7] = ext[7];
}

void port_clock_rise(io_port_t *port) {
    dff_clock_rise(&port->data[0]); dff_clock_rise(&port->data[1]);
    dff_clock_rise(&port->data[2]); dff_clock_rise(&port->data[3]);
    dff_clock_rise(&port->data[4]); dff_clock_rise(&port->data[5]);
    dff_clock_rise(&port->data[6]); dff_clock_rise(&port->data[7]);

    dff_clock_rise(&port->direction[0]); dff_clock_rise(&port->direction[1]);
    dff_clock_rise(&port->direction[2]); dff_clock_rise(&port->direction[3]);
    dff_clock_rise(&port->direction[4]); dff_clock_rise(&port->direction[5]);
    dff_clock_rise(&port->direction[6]); dff_clock_rise(&port->direction[7]);

    dff_clock_rise(&port->irq_enable);
    dff_clock_rise(&port->irq_pending);
}

void port_clock_fall(io_port_t *port) {
    bit active = port->chip_select;
    bit wr = AND(port->write, active);
    bit na0 = NOT(port->addr[0]);
    bit na1 = NOT(port->addr[1]);

    bit sel_data = AND(na1, na0);
    bit sel_dir = AND(na1, port->addr[0]);
    bit sel_irq = AND(port->addr[1], na0);

    bit wr_data = AND(wr, sel_data);
    bit wr_dir = AND(wr, sel_dir);
    bit wr_irq = AND(wr, sel_irq);

    port->data[0].d = mux2(port->data[0].q, port->data_in[0], wr_data);
    port->data[1].d = mux2(port->data[1].q, port->data_in[1], wr_data);
    port->data[2].d = mux2(port->data[2].q, port->data_in[2], wr_data);
    port->data[3].d = mux2(port->data[3].q, port->data_in[3], wr_data);
    port->data[4].d = mux2(port->data[4].q, port->data_in[4], wr_data);
    port->data[5].d = mux2(port->data[5].q, port->data_in[5], wr_data);
    port->data[6].d = mux2(port->data[6].q, port->data_in[6], wr_data);
    port->data[7].d = mux2(port->data[7].q, port->data_in[7], wr_data);

    port->direction[0].d = mux2(port->direction[0].q, port->data_in[0], wr_dir);
    port->direction[1].d = mux2(port->direction[1].q, port->data_in[1], wr_dir);
    port->direction[2].d = mux2(port->direction[2].q, port->data_in[2], wr_dir);
    port->direction[3].d = mux2(port->direction[3].q, port->data_in[3], wr_dir);
    port->direction[4].d = mux2(port->direction[4].q, port->data_in[4], wr_dir);
    port->direction[5].d = mux2(port->direction[5].q, port->data_in[5], wr_dir);
    port->direction[6].d = mux2(port->direction[6].q, port->data_in[6], wr_dir);
    port->direction[7].d = mux2(port->direction[7].q, port->data_in[7], wr_dir);

    port->irq_enable.d = mux2(port->irq_enable.q, port->data_in[0], wr_irq);

    bit change0 = XOR(port->external_in[0], port->data[0].q);
    bit change1 = XOR(port->external_in[1], port->data[1].q);
    bit change2 = XOR(port->external_in[2], port->data[2].q);
    bit change3 = XOR(port->external_in[3], port->data[3].q);
    bit change4 = XOR(port->external_in[4], port->data[4].q);
    bit change5 = XOR(port->external_in[5], port->data[5].q);
    bit change6 = XOR(port->external_in[6], port->data[6].q);
    bit change7 = XOR(port->external_in[7], port->data[7].q);

    bit in0 = NOT(port->direction[0].q);
    bit in1 = NOT(port->direction[1].q);
    bit in2 = NOT(port->direction[2].q);
    bit in3 = NOT(port->direction[3].q);
    bit in4 = NOT(port->direction[4].q);
    bit in5 = NOT(port->direction[5].q);
    bit in6 = NOT(port->direction[6].q);
    bit in7 = NOT(port->direction[7].q);

    bit any_change = OR(OR(OR(AND(change0, in0), AND(change1, in1)),
                           OR(AND(change2, in2), AND(change3, in3))),
                        OR(OR(AND(change4, in4), AND(change5, in5)),
                           OR(AND(change6, in6), AND(change7, in7))));

    bit rd_irq = AND(AND(port->read, active), sel_irq);
    bit pend = mux2(port->irq_pending.q, 0, rd_irq);
    port->irq_pending.d = OR(pend, any_change);

    dff_clock_fall(&port->data[0]); dff_clock_fall(&port->data[1]);
    dff_clock_fall(&port->data[2]); dff_clock_fall(&port->data[3]);
    dff_clock_fall(&port->data[4]); dff_clock_fall(&port->data[5]);
    dff_clock_fall(&port->data[6]); dff_clock_fall(&port->data[7]);

    dff_clock_fall(&port->direction[0]); dff_clock_fall(&port->direction[1]);
    dff_clock_fall(&port->direction[2]); dff_clock_fall(&port->direction[3]);
    dff_clock_fall(&port->direction[4]); dff_clock_fall(&port->direction[5]);
    dff_clock_fall(&port->direction[6]); dff_clock_fall(&port->direction[7]);

    dff_clock_fall(&port->irq_enable);
    dff_clock_fall(&port->irq_pending);
}

void port_propagate(io_port_t *port) {
    bit d0 = port->direction[0].q;
    bit d1 = port->direction[1].q;
    bit d2 = port->direction[2].q;
    bit d3 = port->direction[3].q;
    bit d4 = port->direction[4].q;
    bit d5 = port->direction[5].q;
    bit d6 = port->direction[6].q;
    bit d7 = port->direction[7].q;

    port->external_out[0] = AND(port->data[0].q, d0);
    port->external_out[1] = AND(port->data[1].q, d1);
    port->external_out[2] = AND(port->data[2].q, d2);
    port->external_out[3] = AND(port->data[3].q, d3);
    port->external_out[4] = AND(port->data[4].q, d4);
    port->external_out[5] = AND(port->data[5].q, d5);
    port->external_out[6] = AND(port->data[6].q, d6);
    port->external_out[7] = AND(port->data[7].q, d7);

    port->data_out[0] = mux2(port->external_in[0], port->data[0].q, d0);
    port->data_out[1] = mux2(port->external_in[1], port->data[1].q, d1);
    port->data_out[2] = mux2(port->external_in[2], port->data[2].q, d2);
    port->data_out[3] = mux2(port->external_in[3], port->data[3].q, d3);
    port->data_out[4] = mux2(port->external_in[4], port->data[4].q, d4);
    port->data_out[5] = mux2(port->external_in[5], port->data[5].q, d5);
    port->data_out[6] = mux2(port->external_in[6], port->data[6].q, d6);
    port->data_out[7] = mux2(port->external_in[7], port->data[7].q, d7);

    port->irq = AND(port->irq_pending.q, port->irq_enable.q);
}

void port_read_data(io_port_t *port, bit *out) {
    out[0] = port->data_out[0]; out[1] = port->data_out[1];
    out[2] = port->data_out[2]; out[3] = port->data_out[3];
    out[4] = port->data_out[4]; out[5] = port->data_out[5];
    out[6] = port->data_out[6]; out[7] = port->data_out[7];
}

void port_read_external(io_port_t *port, bit *out) {
    out[0] = port->external_out[0]; out[1] = port->external_out[1];
    out[2] = port->external_out[2]; out[3] = port->external_out[3];
    out[4] = port->external_out[4]; out[5] = port->external_out[5];
    out[6] = port->external_out[6]; out[7] = port->external_out[7];
}

bit port_get_irq(io_port_t *port) {
    return port->irq;
}
