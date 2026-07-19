#ifndef ALETHEIA_IO_CONTROLLER_H
#define ALETHEIA_IO_CONTROLLER_H

#include "../core/types.h"
#include "../core/sequential/d_flip_flop.h"

#define NUM_IO_PORTS 8

typedef struct {
    d_flip_flop_t port_select[3];
    d_flip_flop_t data[32];
    d_flip_flop_t read_strobe;
    d_flip_flop_t write_strobe;

    bit addr_in[16];
    bit data_in[32];
    bit data_out[32];
    bit read_in;
    bit write_in;
    bit chip_select;

    bit port_cs[NUM_IO_PORTS];
    bit port_read[NUM_IO_PORTS];
    bit port_write[NUM_IO_PORTS];
    bit port_addr[4];
    bit ready;
} io_controller_t;

void io_ctrl_init(io_controller_t *ctrl);
void io_ctrl_reset(io_controller_t *ctrl);
void io_ctrl_set_addr(io_controller_t *ctrl, bit *addr);
void io_ctrl_set_data(io_controller_t *ctrl, bit *data);
void io_ctrl_set_read(io_controller_t *ctrl, bit rd);
void io_ctrl_set_write(io_controller_t *ctrl, bit wr);
void io_ctrl_set_cs(io_controller_t *ctrl, bit cs);
void io_ctrl_clock_rise(io_controller_t *ctrl);
void io_ctrl_clock_fall(io_controller_t *ctrl);
void io_ctrl_propagate(io_controller_t *ctrl);
void io_ctrl_read_data(io_controller_t *ctrl, bit *out);
bit io_ctrl_get_port_cs(io_controller_t *ctrl, bit s0, bit s1, bit s2);
bit io_ctrl_get_port_read(io_controller_t *ctrl, bit s0, bit s1, bit s2);
bit io_ctrl_get_port_write(io_controller_t *ctrl, bit s0, bit s1, bit s2);

#endif
