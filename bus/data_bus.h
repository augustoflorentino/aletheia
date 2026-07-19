#ifndef ALETHEIA_DATA_BUS_H
#define ALETHEIA_DATA_BUS_H

#include "../core/types.h"
#include "../core/sequential/d_flip_flop.h"

#define DATA_BUS_WIDTH 32

typedef struct {
    d_flip_flop_t data[DATA_BUS_WIDTH];
    bit data_in[DATA_BUS_WIDTH];
    bit data_out[DATA_BUS_WIDTH];
    bit drive_enable;
    bit valid;
    bit read_write;
} data_bus_t;

void data_bus_init(data_bus_t *bus);
void data_bus_reset(data_bus_t *bus);
void data_bus_set_data(data_bus_t *bus, bit *data);
void data_bus_set_drive(data_bus_t *bus, bit enable);
void data_bus_set_rw(data_bus_t *bus, bit rw);
void data_bus_clock_rise(data_bus_t *bus);
void data_bus_clock_fall(data_bus_t *bus);
void data_bus_propagate(data_bus_t *bus);
void data_bus_read(data_bus_t *bus, bit *out);
bit data_bus_is_valid(data_bus_t *bus);
bit data_bus_is_read(data_bus_t *bus);

#endif
