#include "address_bus.h"
#include "../core/gates/and.h"
#include "../core/mux/mux2.h"

void addr_bus_init(address_bus_t *bus) {
    int i;
    i = 0; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 1; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 2; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 3; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 4; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 5; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 6; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 7; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 8; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 9; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 10; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 11; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 12; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 13; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 14; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 15; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 16; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 17; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 18; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 19; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 20; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 21; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 22; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 23; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 24; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 25; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 26; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 27; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 28; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 29; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 30; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;
    i = 31; dff_reset(&bus->addr[i]); bus->addr_in[i] = 0; bus->addr_out[i] = 0;

    bus->drive_enable = 0;
    bus->valid = 0;
}

void addr_bus_reset(address_bus_t *bus) {
    addr_bus_init(bus);
}

void addr_bus_set_addr(address_bus_t *bus, bit *addr) {
    bus->addr_in[0] = addr[0]; bus->addr_in[1] = addr[1];
    bus->addr_in[2] = addr[2]; bus->addr_in[3] = addr[3];
    bus->addr_in[4] = addr[4]; bus->addr_in[5] = addr[5];
    bus->addr_in[6] = addr[6]; bus->addr_in[7] = addr[7];
    bus->addr_in[8] = addr[8]; bus->addr_in[9] = addr[9];
    bus->addr_in[10] = addr[10]; bus->addr_in[11] = addr[11];
    bus->addr_in[12] = addr[12]; bus->addr_in[13] = addr[13];
    bus->addr_in[14] = addr[14]; bus->addr_in[15] = addr[15];
    bus->addr_in[16] = addr[16]; bus->addr_in[17] = addr[17];
    bus->addr_in[18] = addr[18]; bus->addr_in[19] = addr[19];
    bus->addr_in[20] = addr[20]; bus->addr_in[21] = addr[21];
    bus->addr_in[22] = addr[22]; bus->addr_in[23] = addr[23];
    bus->addr_in[24] = addr[24]; bus->addr_in[25] = addr[25];
    bus->addr_in[26] = addr[26]; bus->addr_in[27] = addr[27];
    bus->addr_in[28] = addr[28]; bus->addr_in[29] = addr[29];
    bus->addr_in[30] = addr[30]; bus->addr_in[31] = addr[31];
}

void addr_bus_set_drive(address_bus_t *bus, bit enable) {
    bus->drive_enable = enable;
}

void addr_bus_clock_rise(address_bus_t *bus) {
    int i;
    i = 0; dff_clock_rise(&bus->addr[i]);
    i = 1; dff_clock_rise(&bus->addr[i]);
    i = 2; dff_clock_rise(&bus->addr[i]);
    i = 3; dff_clock_rise(&bus->addr[i]);
    i = 4; dff_clock_rise(&bus->addr[i]);
    i = 5; dff_clock_rise(&bus->addr[i]);
    i = 6; dff_clock_rise(&bus->addr[i]);
    i = 7; dff_clock_rise(&bus->addr[i]);
    i = 8; dff_clock_rise(&bus->addr[i]);
    i = 9; dff_clock_rise(&bus->addr[i]);
    i = 10; dff_clock_rise(&bus->addr[i]);
    i = 11; dff_clock_rise(&bus->addr[i]);
    i = 12; dff_clock_rise(&bus->addr[i]);
    i = 13; dff_clock_rise(&bus->addr[i]);
    i = 14; dff_clock_rise(&bus->addr[i]);
    i = 15; dff_clock_rise(&bus->addr[i]);
    i = 16; dff_clock_rise(&bus->addr[i]);
    i = 17; dff_clock_rise(&bus->addr[i]);
    i = 18; dff_clock_rise(&bus->addr[i]);
    i = 19; dff_clock_rise(&bus->addr[i]);
    i = 20; dff_clock_rise(&bus->addr[i]);
    i = 21; dff_clock_rise(&bus->addr[i]);
    i = 22; dff_clock_rise(&bus->addr[i]);
    i = 23; dff_clock_rise(&bus->addr[i]);
    i = 24; dff_clock_rise(&bus->addr[i]);
    i = 25; dff_clock_rise(&bus->addr[i]);
    i = 26; dff_clock_rise(&bus->addr[i]);
    i = 27; dff_clock_rise(&bus->addr[i]);
    i = 28; dff_clock_rise(&bus->addr[i]);
    i = 29; dff_clock_rise(&bus->addr[i]);
    i = 30; dff_clock_rise(&bus->addr[i]);
    i = 31; dff_clock_rise(&bus->addr[i]);
}

void addr_bus_clock_fall(address_bus_t *bus) {
    bit de = bus->drive_enable;
    bus->addr[0].d = mux2(bus->addr[0].q, bus->addr_in[0], de);
    bus->addr[1].d = mux2(bus->addr[1].q, bus->addr_in[1], de);
    bus->addr[2].d = mux2(bus->addr[2].q, bus->addr_in[2], de);
    bus->addr[3].d = mux2(bus->addr[3].q, bus->addr_in[3], de);
    bus->addr[4].d = mux2(bus->addr[4].q, bus->addr_in[4], de);
    bus->addr[5].d = mux2(bus->addr[5].q, bus->addr_in[5], de);
    bus->addr[6].d = mux2(bus->addr[6].q, bus->addr_in[6], de);
    bus->addr[7].d = mux2(bus->addr[7].q, bus->addr_in[7], de);
    bus->addr[8].d = mux2(bus->addr[8].q, bus->addr_in[8], de);
    bus->addr[9].d = mux2(bus->addr[9].q, bus->addr_in[9], de);
    bus->addr[10].d = mux2(bus->addr[10].q, bus->addr_in[10], de);
    bus->addr[11].d = mux2(bus->addr[11].q, bus->addr_in[11], de);
    bus->addr[12].d = mux2(bus->addr[12].q, bus->addr_in[12], de);
    bus->addr[13].d = mux2(bus->addr[13].q, bus->addr_in[13], de);
    bus->addr[14].d = mux2(bus->addr[14].q, bus->addr_in[14], de);
    bus->addr[15].d = mux2(bus->addr[15].q, bus->addr_in[15], de);
    bus->addr[16].d = mux2(bus->addr[16].q, bus->addr_in[16], de);
    bus->addr[17].d = mux2(bus->addr[17].q, bus->addr_in[17], de);
    bus->addr[18].d = mux2(bus->addr[18].q, bus->addr_in[18], de);
    bus->addr[19].d = mux2(bus->addr[19].q, bus->addr_in[19], de);
    bus->addr[20].d = mux2(bus->addr[20].q, bus->addr_in[20], de);
    bus->addr[21].d = mux2(bus->addr[21].q, bus->addr_in[21], de);
    bus->addr[22].d = mux2(bus->addr[22].q, bus->addr_in[22], de);
    bus->addr[23].d = mux2(bus->addr[23].q, bus->addr_in[23], de);
    bus->addr[24].d = mux2(bus->addr[24].q, bus->addr_in[24], de);
    bus->addr[25].d = mux2(bus->addr[25].q, bus->addr_in[25], de);
    bus->addr[26].d = mux2(bus->addr[26].q, bus->addr_in[26], de);
    bus->addr[27].d = mux2(bus->addr[27].q, bus->addr_in[27], de);
    bus->addr[28].d = mux2(bus->addr[28].q, bus->addr_in[28], de);
    bus->addr[29].d = mux2(bus->addr[29].q, bus->addr_in[29], de);
    bus->addr[30].d = mux2(bus->addr[30].q, bus->addr_in[30], de);
    bus->addr[31].d = mux2(bus->addr[31].q, bus->addr_in[31], de);

    int i;
    i = 0; dff_clock_fall(&bus->addr[i]);
    i = 1; dff_clock_fall(&bus->addr[i]);
    i = 2; dff_clock_fall(&bus->addr[i]);
    i = 3; dff_clock_fall(&bus->addr[i]);
    i = 4; dff_clock_fall(&bus->addr[i]);
    i = 5; dff_clock_fall(&bus->addr[i]);
    i = 6; dff_clock_fall(&bus->addr[i]);
    i = 7; dff_clock_fall(&bus->addr[i]);
    i = 8; dff_clock_fall(&bus->addr[i]);
    i = 9; dff_clock_fall(&bus->addr[i]);
    i = 10; dff_clock_fall(&bus->addr[i]);
    i = 11; dff_clock_fall(&bus->addr[i]);
    i = 12; dff_clock_fall(&bus->addr[i]);
    i = 13; dff_clock_fall(&bus->addr[i]);
    i = 14; dff_clock_fall(&bus->addr[i]);
    i = 15; dff_clock_fall(&bus->addr[i]);
    i = 16; dff_clock_fall(&bus->addr[i]);
    i = 17; dff_clock_fall(&bus->addr[i]);
    i = 18; dff_clock_fall(&bus->addr[i]);
    i = 19; dff_clock_fall(&bus->addr[i]);
    i = 20; dff_clock_fall(&bus->addr[i]);
    i = 21; dff_clock_fall(&bus->addr[i]);
    i = 22; dff_clock_fall(&bus->addr[i]);
    i = 23; dff_clock_fall(&bus->addr[i]);
    i = 24; dff_clock_fall(&bus->addr[i]);
    i = 25; dff_clock_fall(&bus->addr[i]);
    i = 26; dff_clock_fall(&bus->addr[i]);
    i = 27; dff_clock_fall(&bus->addr[i]);
    i = 28; dff_clock_fall(&bus->addr[i]);
    i = 29; dff_clock_fall(&bus->addr[i]);
    i = 30; dff_clock_fall(&bus->addr[i]);
    i = 31; dff_clock_fall(&bus->addr[i]);
}

void addr_bus_propagate(address_bus_t *bus) {
    bus->addr_out[0] = bus->addr[0].q; bus->addr_out[1] = bus->addr[1].q;
    bus->addr_out[2] = bus->addr[2].q; bus->addr_out[3] = bus->addr[3].q;
    bus->addr_out[4] = bus->addr[4].q; bus->addr_out[5] = bus->addr[5].q;
    bus->addr_out[6] = bus->addr[6].q; bus->addr_out[7] = bus->addr[7].q;
    bus->addr_out[8] = bus->addr[8].q; bus->addr_out[9] = bus->addr[9].q;
    bus->addr_out[10] = bus->addr[10].q; bus->addr_out[11] = bus->addr[11].q;
    bus->addr_out[12] = bus->addr[12].q; bus->addr_out[13] = bus->addr[13].q;
    bus->addr_out[14] = bus->addr[14].q; bus->addr_out[15] = bus->addr[15].q;
    bus->addr_out[16] = bus->addr[16].q; bus->addr_out[17] = bus->addr[17].q;
    bus->addr_out[18] = bus->addr[18].q; bus->addr_out[19] = bus->addr[19].q;
    bus->addr_out[20] = bus->addr[20].q; bus->addr_out[21] = bus->addr[21].q;
    bus->addr_out[22] = bus->addr[22].q; bus->addr_out[23] = bus->addr[23].q;
    bus->addr_out[24] = bus->addr[24].q; bus->addr_out[25] = bus->addr[25].q;
    bus->addr_out[26] = bus->addr[26].q; bus->addr_out[27] = bus->addr[27].q;
    bus->addr_out[28] = bus->addr[28].q; bus->addr_out[29] = bus->addr[29].q;
    bus->addr_out[30] = bus->addr[30].q; bus->addr_out[31] = bus->addr[31].q;

    bus->valid = bus->drive_enable;
}

void addr_bus_read(address_bus_t *bus, bit *out) {
    out[0] = bus->addr_out[0]; out[1] = bus->addr_out[1];
    out[2] = bus->addr_out[2]; out[3] = bus->addr_out[3];
    out[4] = bus->addr_out[4]; out[5] = bus->addr_out[5];
    out[6] = bus->addr_out[6]; out[7] = bus->addr_out[7];
    out[8] = bus->addr_out[8]; out[9] = bus->addr_out[9];
    out[10] = bus->addr_out[10]; out[11] = bus->addr_out[11];
    out[12] = bus->addr_out[12]; out[13] = bus->addr_out[13];
    out[14] = bus->addr_out[14]; out[15] = bus->addr_out[15];
    out[16] = bus->addr_out[16]; out[17] = bus->addr_out[17];
    out[18] = bus->addr_out[18]; out[19] = bus->addr_out[19];
    out[20] = bus->addr_out[20]; out[21] = bus->addr_out[21];
    out[22] = bus->addr_out[22]; out[23] = bus->addr_out[23];
    out[24] = bus->addr_out[24]; out[25] = bus->addr_out[25];
    out[26] = bus->addr_out[26]; out[27] = bus->addr_out[27];
    out[28] = bus->addr_out[28]; out[29] = bus->addr_out[29];
    out[30] = bus->addr_out[30]; out[31] = bus->addr_out[31];
}

bit addr_bus_is_valid(address_bus_t *bus) {
    return bus->valid;
}
