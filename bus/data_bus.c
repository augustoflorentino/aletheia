#include "data_bus.h"
#include "../core/gates/and.h"
#include "../core/gates/not.h"
#include "../core/mux/mux2.h"

void data_bus_init(data_bus_t *bus) {
    int i;
    i = 0; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 1; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 2; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 3; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 4; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 5; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 6; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 7; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 8; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 9; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 10; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 11; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 12; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 13; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 14; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 15; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 16; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 17; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 18; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 19; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 20; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 21; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 22; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 23; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 24; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 25; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 26; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 27; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 28; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 29; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 30; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;
    i = 31; dff_reset(&bus->data[i]); bus->data_in[i] = 0; bus->data_out[i] = 0;

    bus->drive_enable = 0;
    bus->valid = 0;
    bus->read_write = 0;
}

void data_bus_reset(data_bus_t *bus) {
    data_bus_init(bus);
}

void data_bus_set_data(data_bus_t *bus, bit *data) {
    bus->data_in[0] = data[0]; bus->data_in[1] = data[1];
    bus->data_in[2] = data[2]; bus->data_in[3] = data[3];
    bus->data_in[4] = data[4]; bus->data_in[5] = data[5];
    bus->data_in[6] = data[6]; bus->data_in[7] = data[7];
    bus->data_in[8] = data[8]; bus->data_in[9] = data[9];
    bus->data_in[10] = data[10]; bus->data_in[11] = data[11];
    bus->data_in[12] = data[12]; bus->data_in[13] = data[13];
    bus->data_in[14] = data[14]; bus->data_in[15] = data[15];
    bus->data_in[16] = data[16]; bus->data_in[17] = data[17];
    bus->data_in[18] = data[18]; bus->data_in[19] = data[19];
    bus->data_in[20] = data[20]; bus->data_in[21] = data[21];
    bus->data_in[22] = data[22]; bus->data_in[23] = data[23];
    bus->data_in[24] = data[24]; bus->data_in[25] = data[25];
    bus->data_in[26] = data[26]; bus->data_in[27] = data[27];
    bus->data_in[28] = data[28]; bus->data_in[29] = data[29];
    bus->data_in[30] = data[30]; bus->data_in[31] = data[31];
}

void data_bus_set_drive(data_bus_t *bus, bit enable) {
    bus->drive_enable = enable;
}

void data_bus_set_rw(data_bus_t *bus, bit rw) {
    bus->read_write = rw;
}

void data_bus_clock_rise(data_bus_t *bus) {
    int i;
    i = 0; dff_clock_rise(&bus->data[i]);
    i = 1; dff_clock_rise(&bus->data[i]);
    i = 2; dff_clock_rise(&bus->data[i]);
    i = 3; dff_clock_rise(&bus->data[i]);
    i = 4; dff_clock_rise(&bus->data[i]);
    i = 5; dff_clock_rise(&bus->data[i]);
    i = 6; dff_clock_rise(&bus->data[i]);
    i = 7; dff_clock_rise(&bus->data[i]);
    i = 8; dff_clock_rise(&bus->data[i]);
    i = 9; dff_clock_rise(&bus->data[i]);
    i = 10; dff_clock_rise(&bus->data[i]);
    i = 11; dff_clock_rise(&bus->data[i]);
    i = 12; dff_clock_rise(&bus->data[i]);
    i = 13; dff_clock_rise(&bus->data[i]);
    i = 14; dff_clock_rise(&bus->data[i]);
    i = 15; dff_clock_rise(&bus->data[i]);
    i = 16; dff_clock_rise(&bus->data[i]);
    i = 17; dff_clock_rise(&bus->data[i]);
    i = 18; dff_clock_rise(&bus->data[i]);
    i = 19; dff_clock_rise(&bus->data[i]);
    i = 20; dff_clock_rise(&bus->data[i]);
    i = 21; dff_clock_rise(&bus->data[i]);
    i = 22; dff_clock_rise(&bus->data[i]);
    i = 23; dff_clock_rise(&bus->data[i]);
    i = 24; dff_clock_rise(&bus->data[i]);
    i = 25; dff_clock_rise(&bus->data[i]);
    i = 26; dff_clock_rise(&bus->data[i]);
    i = 27; dff_clock_rise(&bus->data[i]);
    i = 28; dff_clock_rise(&bus->data[i]);
    i = 29; dff_clock_rise(&bus->data[i]);
    i = 30; dff_clock_rise(&bus->data[i]);
    i = 31; dff_clock_rise(&bus->data[i]);
}

void data_bus_clock_fall(data_bus_t *bus) {
    bit de = bus->drive_enable;
    bus->data[0].d = mux2(bus->data[0].q, bus->data_in[0], de);
    bus->data[1].d = mux2(bus->data[1].q, bus->data_in[1], de);
    bus->data[2].d = mux2(bus->data[2].q, bus->data_in[2], de);
    bus->data[3].d = mux2(bus->data[3].q, bus->data_in[3], de);
    bus->data[4].d = mux2(bus->data[4].q, bus->data_in[4], de);
    bus->data[5].d = mux2(bus->data[5].q, bus->data_in[5], de);
    bus->data[6].d = mux2(bus->data[6].q, bus->data_in[6], de);
    bus->data[7].d = mux2(bus->data[7].q, bus->data_in[7], de);
    bus->data[8].d = mux2(bus->data[8].q, bus->data_in[8], de);
    bus->data[9].d = mux2(bus->data[9].q, bus->data_in[9], de);
    bus->data[10].d = mux2(bus->data[10].q, bus->data_in[10], de);
    bus->data[11].d = mux2(bus->data[11].q, bus->data_in[11], de);
    bus->data[12].d = mux2(bus->data[12].q, bus->data_in[12], de);
    bus->data[13].d = mux2(bus->data[13].q, bus->data_in[13], de);
    bus->data[14].d = mux2(bus->data[14].q, bus->data_in[14], de);
    bus->data[15].d = mux2(bus->data[15].q, bus->data_in[15], de);
    bus->data[16].d = mux2(bus->data[16].q, bus->data_in[16], de);
    bus->data[17].d = mux2(bus->data[17].q, bus->data_in[17], de);
    bus->data[18].d = mux2(bus->data[18].q, bus->data_in[18], de);
    bus->data[19].d = mux2(bus->data[19].q, bus->data_in[19], de);
    bus->data[20].d = mux2(bus->data[20].q, bus->data_in[20], de);
    bus->data[21].d = mux2(bus->data[21].q, bus->data_in[21], de);
    bus->data[22].d = mux2(bus->data[22].q, bus->data_in[22], de);
    bus->data[23].d = mux2(bus->data[23].q, bus->data_in[23], de);
    bus->data[24].d = mux2(bus->data[24].q, bus->data_in[24], de);
    bus->data[25].d = mux2(bus->data[25].q, bus->data_in[25], de);
    bus->data[26].d = mux2(bus->data[26].q, bus->data_in[26], de);
    bus->data[27].d = mux2(bus->data[27].q, bus->data_in[27], de);
    bus->data[28].d = mux2(bus->data[28].q, bus->data_in[28], de);
    bus->data[29].d = mux2(bus->data[29].q, bus->data_in[29], de);
    bus->data[30].d = mux2(bus->data[30].q, bus->data_in[30], de);
    bus->data[31].d = mux2(bus->data[31].q, bus->data_in[31], de);

    int i;
    i = 0; dff_clock_fall(&bus->data[i]);
    i = 1; dff_clock_fall(&bus->data[i]);
    i = 2; dff_clock_fall(&bus->data[i]);
    i = 3; dff_clock_fall(&bus->data[i]);
    i = 4; dff_clock_fall(&bus->data[i]);
    i = 5; dff_clock_fall(&bus->data[i]);
    i = 6; dff_clock_fall(&bus->data[i]);
    i = 7; dff_clock_fall(&bus->data[i]);
    i = 8; dff_clock_fall(&bus->data[i]);
    i = 9; dff_clock_fall(&bus->data[i]);
    i = 10; dff_clock_fall(&bus->data[i]);
    i = 11; dff_clock_fall(&bus->data[i]);
    i = 12; dff_clock_fall(&bus->data[i]);
    i = 13; dff_clock_fall(&bus->data[i]);
    i = 14; dff_clock_fall(&bus->data[i]);
    i = 15; dff_clock_fall(&bus->data[i]);
    i = 16; dff_clock_fall(&bus->data[i]);
    i = 17; dff_clock_fall(&bus->data[i]);
    i = 18; dff_clock_fall(&bus->data[i]);
    i = 19; dff_clock_fall(&bus->data[i]);
    i = 20; dff_clock_fall(&bus->data[i]);
    i = 21; dff_clock_fall(&bus->data[i]);
    i = 22; dff_clock_fall(&bus->data[i]);
    i = 23; dff_clock_fall(&bus->data[i]);
    i = 24; dff_clock_fall(&bus->data[i]);
    i = 25; dff_clock_fall(&bus->data[i]);
    i = 26; dff_clock_fall(&bus->data[i]);
    i = 27; dff_clock_fall(&bus->data[i]);
    i = 28; dff_clock_fall(&bus->data[i]);
    i = 29; dff_clock_fall(&bus->data[i]);
    i = 30; dff_clock_fall(&bus->data[i]);
    i = 31; dff_clock_fall(&bus->data[i]);
}

void data_bus_propagate(data_bus_t *bus) {
    bus->data_out[0] = bus->data[0].q; bus->data_out[1] = bus->data[1].q;
    bus->data_out[2] = bus->data[2].q; bus->data_out[3] = bus->data[3].q;
    bus->data_out[4] = bus->data[4].q; bus->data_out[5] = bus->data[5].q;
    bus->data_out[6] = bus->data[6].q; bus->data_out[7] = bus->data[7].q;
    bus->data_out[8] = bus->data[8].q; bus->data_out[9] = bus->data[9].q;
    bus->data_out[10] = bus->data[10].q; bus->data_out[11] = bus->data[11].q;
    bus->data_out[12] = bus->data[12].q; bus->data_out[13] = bus->data[13].q;
    bus->data_out[14] = bus->data[14].q; bus->data_out[15] = bus->data[15].q;
    bus->data_out[16] = bus->data[16].q; bus->data_out[17] = bus->data[17].q;
    bus->data_out[18] = bus->data[18].q; bus->data_out[19] = bus->data[19].q;
    bus->data_out[20] = bus->data[20].q; bus->data_out[21] = bus->data[21].q;
    bus->data_out[22] = bus->data[22].q; bus->data_out[23] = bus->data[23].q;
    bus->data_out[24] = bus->data[24].q; bus->data_out[25] = bus->data[25].q;
    bus->data_out[26] = bus->data[26].q; bus->data_out[27] = bus->data[27].q;
    bus->data_out[28] = bus->data[28].q; bus->data_out[29] = bus->data[29].q;
    bus->data_out[30] = bus->data[30].q; bus->data_out[31] = bus->data[31].q;

    bus->valid = bus->drive_enable;
}

void data_bus_read(data_bus_t *bus, bit *out) {
    out[0] = bus->data_out[0]; out[1] = bus->data_out[1];
    out[2] = bus->data_out[2]; out[3] = bus->data_out[3];
    out[4] = bus->data_out[4]; out[5] = bus->data_out[5];
    out[6] = bus->data_out[6]; out[7] = bus->data_out[7];
    out[8] = bus->data_out[8]; out[9] = bus->data_out[9];
    out[10] = bus->data_out[10]; out[11] = bus->data_out[11];
    out[12] = bus->data_out[12]; out[13] = bus->data_out[13];
    out[14] = bus->data_out[14]; out[15] = bus->data_out[15];
    out[16] = bus->data_out[16]; out[17] = bus->data_out[17];
    out[18] = bus->data_out[18]; out[19] = bus->data_out[19];
    out[20] = bus->data_out[20]; out[21] = bus->data_out[21];
    out[22] = bus->data_out[22]; out[23] = bus->data_out[23];
    out[24] = bus->data_out[24]; out[25] = bus->data_out[25];
    out[26] = bus->data_out[26]; out[27] = bus->data_out[27];
    out[28] = bus->data_out[28]; out[29] = bus->data_out[29];
    out[30] = bus->data_out[30]; out[31] = bus->data_out[31];
}

bit data_bus_is_valid(data_bus_t *bus) {
    return bus->valid;
}

bit data_bus_is_read(data_bus_t *bus) {
    return bus->read_write;
}
