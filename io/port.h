#ifndef ALETHEIA_PORT_H
#define ALETHEIA_PORT_H

#include "../core/types.h"
#include "../core/sequential/d_flip_flop.h"

typedef struct {
    d_flip_flop_t data[8];
    d_flip_flop_t direction[8];
    d_flip_flop_t irq_enable;
    d_flip_flop_t irq_pending;

    bit data_in[8];
    bit data_out[8];
    bit direction_in[8];
    bit external_in[8];
    bit external_out[8];

    bit chip_select;
    bit read;
    bit write;
    bit addr[2];

    bit irq;
} io_port_t;

void port_init(io_port_t *port);
void port_reset(io_port_t *port);
void port_set_cs(io_port_t *port, bit cs);
void port_set_read(io_port_t *port, bit rd);
void port_set_write(io_port_t *port, bit wr);
void port_set_addr(io_port_t *port, bit a0, bit a1);
void port_set_data(io_port_t *port, bit *data);
void port_set_external(io_port_t *port, bit *ext);
void port_clock_rise(io_port_t *port);
void port_clock_fall(io_port_t *port);
void port_propagate(io_port_t *port);
void port_read_data(io_port_t *port, bit *out);
void port_read_external(io_port_t *port, bit *out);
bit port_get_irq(io_port_t *port);

#endif
