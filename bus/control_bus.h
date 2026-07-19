#ifndef ALETHEIA_CONTROL_BUS_H
#define ALETHEIA_CONTROL_BUS_H

#include "../core/types.h"
#include "../core/sequential/d_flip_flop.h"

typedef struct {
    d_flip_flop_t read;
    d_flip_flop_t write;
    d_flip_flop_t iorq;
    d_flip_flop_t mreq;
    d_flip_flop_t busreq;
    d_flip_flop_t busack;
    d_flip_flop_t wait_n;
    d_flip_flop_t ready;

    bit read_in;
    bit write_in;
    bit iorq_in;
    bit mreq_in;
    bit busreq_in;
    bit busack_in;
    bit wait_in;
    bit ready_in;

    bit read_out;
    bit write_out;
    bit iorq_out;
    bit mreq_out;
    bit busreq_out;
    bit busack_out;
    bit wait_out;
    bit ready_out;

    bit drive_enable;
} control_bus_t;

void ctrl_bus_init(control_bus_t *bus);
void ctrl_bus_reset(control_bus_t *bus);
void ctrl_bus_set_read(control_bus_t *bus, bit val);
void ctrl_bus_set_write(control_bus_t *bus, bit val);
void ctrl_bus_set_iorq(control_bus_t *bus, bit val);
void ctrl_bus_set_mreq(control_bus_t *bus, bit val);
void ctrl_bus_set_busreq(control_bus_t *bus, bit val);
void ctrl_bus_set_busack(control_bus_t *bus, bit val);
void ctrl_bus_set_wait(control_bus_t *bus, bit val);
void ctrl_bus_set_ready(control_bus_t *bus, bit val);
void ctrl_bus_set_drive(control_bus_t *bus, bit enable);
void ctrl_bus_clock_rise(control_bus_t *bus);
void ctrl_bus_clock_fall(control_bus_t *bus);
void ctrl_bus_propagate(control_bus_t *bus);

bit ctrl_bus_get_read(control_bus_t *bus);
bit ctrl_bus_get_write(control_bus_t *bus);
bit ctrl_bus_get_iorq(control_bus_t *bus);
bit ctrl_bus_get_mreq(control_bus_t *bus);
bit ctrl_bus_get_busreq(control_bus_t *bus);
bit ctrl_bus_get_busack(control_bus_t *bus);
bit ctrl_bus_get_wait(control_bus_t *bus);
bit ctrl_bus_get_ready(control_bus_t *bus);

#endif
