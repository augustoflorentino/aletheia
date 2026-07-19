#ifndef ALETHEIA_ADDRESS_BUS_H
#define ALETHEIA_ADDRESS_BUS_H

#include "../core/types.h"
#include "../core/sequential/d_flip_flop.h"

#define ADDR_BUS_WIDTH 32

typedef struct {
    d_flip_flop_t addr[ADDR_BUS_WIDTH];
    bit addr_in[ADDR_BUS_WIDTH];
    bit addr_out[ADDR_BUS_WIDTH];
    bit drive_enable;
    bit valid;
} address_bus_t;

void addr_bus_init(address_bus_t *bus);
void addr_bus_reset(address_bus_t *bus);
void addr_bus_set_addr(address_bus_t *bus, bit *addr);
void addr_bus_set_drive(address_bus_t *bus, bit enable);
void addr_bus_clock_rise(address_bus_t *bus);
void addr_bus_clock_fall(address_bus_t *bus);
void addr_bus_propagate(address_bus_t *bus);
void addr_bus_read(address_bus_t *bus, bit *out);
bit addr_bus_is_valid(address_bus_t *bus);

#endif
