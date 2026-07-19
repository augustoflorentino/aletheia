#include "control_bus.h"
#include "../core/gates/and.h"
#include "../core/mux/mux2.h"

void ctrl_bus_init(control_bus_t *bus) {
    dff_reset(&bus->read); dff_reset(&bus->write);
    dff_reset(&bus->iorq); dff_reset(&bus->mreq);
    dff_reset(&bus->busreq); dff_reset(&bus->busack);
    dff_reset(&bus->wait_n); dff_reset(&bus->ready);

    bus->read_in = 0; bus->write_in = 0;
    bus->iorq_in = 0; bus->mreq_in = 0;
    bus->busreq_in = 0; bus->busack_in = 0;
    bus->wait_in = 0; bus->ready_in = 0;

    bus->read_out = 0; bus->write_out = 0;
    bus->iorq_out = 0; bus->mreq_out = 0;
    bus->busreq_out = 0; bus->busack_out = 0;
    bus->wait_out = 0; bus->ready_out = 0;

    bus->drive_enable = 0;
}

void ctrl_bus_reset(control_bus_t *bus) {
    ctrl_bus_init(bus);
}

void ctrl_bus_set_read(control_bus_t *bus, bit val) {
    bus->read_in = val;
}

void ctrl_bus_set_write(control_bus_t *bus, bit val) {
    bus->write_in = val;
}

void ctrl_bus_set_iorq(control_bus_t *bus, bit val) {
    bus->iorq_in = val;
}

void ctrl_bus_set_mreq(control_bus_t *bus, bit val) {
    bus->mreq_in = val;
}

void ctrl_bus_set_busreq(control_bus_t *bus, bit val) {
    bus->busreq_in = val;
}

void ctrl_bus_set_busack(control_bus_t *bus, bit val) {
    bus->busack_in = val;
}

void ctrl_bus_set_wait(control_bus_t *bus, bit val) {
    bus->wait_in = val;
}

void ctrl_bus_set_ready(control_bus_t *bus, bit val) {
    bus->ready_in = val;
}

void ctrl_bus_set_drive(control_bus_t *bus, bit enable) {
    bus->drive_enable = enable;
}

void ctrl_bus_clock_rise(control_bus_t *bus) {
    dff_clock_rise(&bus->read); dff_clock_rise(&bus->write);
    dff_clock_rise(&bus->iorq); dff_clock_rise(&bus->mreq);
    dff_clock_rise(&bus->busreq); dff_clock_rise(&bus->busack);
    dff_clock_rise(&bus->wait_n); dff_clock_rise(&bus->ready);
}

void ctrl_bus_clock_fall(control_bus_t *bus) {
    bit de = bus->drive_enable;

    bus->read.d = mux2(bus->read.q, bus->read_in, de);
    bus->write.d = mux2(bus->write.q, bus->write_in, de);
    bus->iorq.d = mux2(bus->iorq.q, bus->iorq_in, de);
    bus->mreq.d = mux2(bus->mreq.q, bus->mreq_in, de);
    bus->busreq.d = mux2(bus->busreq.q, bus->busreq_in, de);
    bus->busack.d = mux2(bus->busack.q, bus->busack_in, de);
    bus->wait_n.d = mux2(bus->wait_n.q, bus->wait_in, de);
    bus->ready.d = mux2(bus->ready.q, bus->ready_in, de);

    dff_clock_fall(&bus->read); dff_clock_fall(&bus->write);
    dff_clock_fall(&bus->iorq); dff_clock_fall(&bus->mreq);
    dff_clock_fall(&bus->busreq); dff_clock_fall(&bus->busack);
    dff_clock_fall(&bus->wait_n); dff_clock_fall(&bus->ready);
}

void ctrl_bus_propagate(control_bus_t *bus) {
    bus->read_out = bus->read.q;
    bus->write_out = bus->write.q;
    bus->iorq_out = bus->iorq.q;
    bus->mreq_out = bus->mreq.q;
    bus->busreq_out = bus->busreq.q;
    bus->busack_out = bus->busack.q;
    bus->wait_out = bus->wait_n.q;
    bus->ready_out = bus->ready.q;
}

bit ctrl_bus_get_read(control_bus_t *bus) {
    return bus->read_out;
}

bit ctrl_bus_get_write(control_bus_t *bus) {
    return bus->write_out;
}

bit ctrl_bus_get_iorq(control_bus_t *bus) {
    return bus->iorq_out;
}

bit ctrl_bus_get_mreq(control_bus_t *bus) {
    return bus->mreq_out;
}

bit ctrl_bus_get_busreq(control_bus_t *bus) {
    return bus->busreq_out;
}

bit ctrl_bus_get_busack(control_bus_t *bus) {
    return bus->busack_out;
}

bit ctrl_bus_get_wait(control_bus_t *bus) {
    return bus->wait_out;
}

bit ctrl_bus_get_ready(control_bus_t *bus) {
    return bus->ready_out;
}
