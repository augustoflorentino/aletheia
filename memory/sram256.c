#include "sram256.h"
#include "../core/gates/gates.h"
#include "../core/mux/mux.h"

static void bank_decode(bit *addr, bit *sel) {
    bit a4 = addr[4], a5 = addr[5], a6 = addr[6], a7 = addr[7];
    bit n4 = NOT(a4), n5 = NOT(a5), n6 = NOT(a6), n7 = NOT(a7);

    sel[0] = AND(AND(n7, n6), AND(n5, n4));
    sel[1] = AND(AND(n7, n6), AND(n5, a4));
    sel[2] = AND(AND(n7, n6), AND(a5, n4));
    sel[3] = AND(AND(n7, n6), AND(a5, a4));
    sel[4] = AND(AND(n7, a6), AND(n5, n4));
    sel[5] = AND(AND(n7, a6), AND(n5, a4));
    sel[6] = AND(AND(n7, a6), AND(a5, n4));
    sel[7] = AND(AND(n7, a6), AND(a5, a4));
    sel[8] = AND(AND(a7, n6), AND(n5, n4));
    sel[9] = AND(AND(a7, n6), AND(n5, a4));
    sel[10] = AND(AND(a7, n6), AND(a5, n4));
    sel[11] = AND(AND(a7, n6), AND(a5, a4));
    sel[12] = AND(AND(a7, a6), AND(n5, n4));
    sel[13] = AND(AND(a7, a6), AND(n5, a4));
    sel[14] = AND(AND(a7, a6), AND(a5, n4));
    sel[15] = AND(AND(a7, a6), AND(a5, a4));
}

void sram256_init(sram_256x32_t *ram) {
    sram_init(&ram->banks[0]); sram_init(&ram->banks[1]);
    sram_init(&ram->banks[2]); sram_init(&ram->banks[3]);
    sram_init(&ram->banks[4]); sram_init(&ram->banks[5]);
    sram_init(&ram->banks[6]); sram_init(&ram->banks[7]);
    sram_init(&ram->banks[8]); sram_init(&ram->banks[9]);
    sram_init(&ram->banks[10]); sram_init(&ram->banks[11]);
    sram_init(&ram->banks[12]); sram_init(&ram->banks[13]);
    sram_init(&ram->banks[14]); sram_init(&ram->banks[15]);

    ram->addr[0] = 0; ram->addr[1] = 0; ram->addr[2] = 0; ram->addr[3] = 0;
    ram->addr[4] = 0; ram->addr[5] = 0; ram->addr[6] = 0; ram->addr[7] = 0;
    ram->write_enable = 0;
    ram->chip_select = 0;

    int i;
    i = 0; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 1; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 2; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 3; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 4; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 5; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 6; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 7; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 8; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 9; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 10; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 11; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 12; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 13; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 14; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 15; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 16; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 17; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 18; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 19; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 20; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 21; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 22; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 23; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 24; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 25; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 26; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 27; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 28; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 29; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 30; ram->data_in[i] = 0; ram->data_out[i] = 0;
    i = 31; ram->data_in[i] = 0; ram->data_out[i] = 0;
}

void sram256_reset(sram_256x32_t *ram) {
    sram256_init(ram);
}

void sram256_set_addr(sram_256x32_t *ram, bit *addr) {
    ram->addr[0] = addr[0]; ram->addr[1] = addr[1];
    ram->addr[2] = addr[2]; ram->addr[3] = addr[3];
    ram->addr[4] = addr[4]; ram->addr[5] = addr[5];
    ram->addr[6] = addr[6]; ram->addr[7] = addr[7];
}

void sram256_set_data(sram_256x32_t *ram, bit *data) {
    ram->data_in[0] = data[0]; ram->data_in[1] = data[1];
    ram->data_in[2] = data[2]; ram->data_in[3] = data[3];
    ram->data_in[4] = data[4]; ram->data_in[5] = data[5];
    ram->data_in[6] = data[6]; ram->data_in[7] = data[7];
    ram->data_in[8] = data[8]; ram->data_in[9] = data[9];
    ram->data_in[10] = data[10]; ram->data_in[11] = data[11];
    ram->data_in[12] = data[12]; ram->data_in[13] = data[13];
    ram->data_in[14] = data[14]; ram->data_in[15] = data[15];
    ram->data_in[16] = data[16]; ram->data_in[17] = data[17];
    ram->data_in[18] = data[18]; ram->data_in[19] = data[19];
    ram->data_in[20] = data[20]; ram->data_in[21] = data[21];
    ram->data_in[22] = data[22]; ram->data_in[23] = data[23];
    ram->data_in[24] = data[24]; ram->data_in[25] = data[25];
    ram->data_in[26] = data[26]; ram->data_in[27] = data[27];
    ram->data_in[28] = data[28]; ram->data_in[29] = data[29];
    ram->data_in[30] = data[30]; ram->data_in[31] = data[31];
}

void sram256_set_write(sram_256x32_t *ram, bit we) {
    ram->write_enable = we;
}

void sram256_set_cs(sram_256x32_t *ram, bit cs) {
    ram->chip_select = cs;
}

void sram256_clock_rise(sram_256x32_t *ram) {
    sram_clock_rise(&ram->banks[0]); sram_clock_rise(&ram->banks[1]);
    sram_clock_rise(&ram->banks[2]); sram_clock_rise(&ram->banks[3]);
    sram_clock_rise(&ram->banks[4]); sram_clock_rise(&ram->banks[5]);
    sram_clock_rise(&ram->banks[6]); sram_clock_rise(&ram->banks[7]);
    sram_clock_rise(&ram->banks[8]); sram_clock_rise(&ram->banks[9]);
    sram_clock_rise(&ram->banks[10]); sram_clock_rise(&ram->banks[11]);
    sram_clock_rise(&ram->banks[12]); sram_clock_rise(&ram->banks[13]);
    sram_clock_rise(&ram->banks[14]); sram_clock_rise(&ram->banks[15]);
}

void sram256_clock_fall(sram_256x32_t *ram) {
    bit sel[16];
    bank_decode(ram->addr, sel);

    sram_set_addr(&ram->banks[0], ram->addr);
    sram_set_addr(&ram->banks[1], ram->addr);
    sram_set_addr(&ram->banks[2], ram->addr);
    sram_set_addr(&ram->banks[3], ram->addr);
    sram_set_addr(&ram->banks[4], ram->addr);
    sram_set_addr(&ram->banks[5], ram->addr);
    sram_set_addr(&ram->banks[6], ram->addr);
    sram_set_addr(&ram->banks[7], ram->addr);
    sram_set_addr(&ram->banks[8], ram->addr);
    sram_set_addr(&ram->banks[9], ram->addr);
    sram_set_addr(&ram->banks[10], ram->addr);
    sram_set_addr(&ram->banks[11], ram->addr);
    sram_set_addr(&ram->banks[12], ram->addr);
    sram_set_addr(&ram->banks[13], ram->addr);
    sram_set_addr(&ram->banks[14], ram->addr);
    sram_set_addr(&ram->banks[15], ram->addr);

    sram_set_data(&ram->banks[0], ram->data_in);
    sram_set_data(&ram->banks[1], ram->data_in);
    sram_set_data(&ram->banks[2], ram->data_in);
    sram_set_data(&ram->banks[3], ram->data_in);
    sram_set_data(&ram->banks[4], ram->data_in);
    sram_set_data(&ram->banks[5], ram->data_in);
    sram_set_data(&ram->banks[6], ram->data_in);
    sram_set_data(&ram->banks[7], ram->data_in);
    sram_set_data(&ram->banks[8], ram->data_in);
    sram_set_data(&ram->banks[9], ram->data_in);
    sram_set_data(&ram->banks[10], ram->data_in);
    sram_set_data(&ram->banks[11], ram->data_in);
    sram_set_data(&ram->banks[12], ram->data_in);
    sram_set_data(&ram->banks[13], ram->data_in);
    sram_set_data(&ram->banks[14], ram->data_in);
    sram_set_data(&ram->banks[15], ram->data_in);

    sram_set_write(&ram->banks[0], AND(ram->write_enable, sel[0]));
    sram_set_write(&ram->banks[1], AND(ram->write_enable, sel[1]));
    sram_set_write(&ram->banks[2], AND(ram->write_enable, sel[2]));
    sram_set_write(&ram->banks[3], AND(ram->write_enable, sel[3]));
    sram_set_write(&ram->banks[4], AND(ram->write_enable, sel[4]));
    sram_set_write(&ram->banks[5], AND(ram->write_enable, sel[5]));
    sram_set_write(&ram->banks[6], AND(ram->write_enable, sel[6]));
    sram_set_write(&ram->banks[7], AND(ram->write_enable, sel[7]));
    sram_set_write(&ram->banks[8], AND(ram->write_enable, sel[8]));
    sram_set_write(&ram->banks[9], AND(ram->write_enable, sel[9]));
    sram_set_write(&ram->banks[10], AND(ram->write_enable, sel[10]));
    sram_set_write(&ram->banks[11], AND(ram->write_enable, sel[11]));
    sram_set_write(&ram->banks[12], AND(ram->write_enable, sel[12]));
    sram_set_write(&ram->banks[13], AND(ram->write_enable, sel[13]));
    sram_set_write(&ram->banks[14], AND(ram->write_enable, sel[14]));
    sram_set_write(&ram->banks[15], AND(ram->write_enable, sel[15]));

    sram_set_cs(&ram->banks[0], ram->chip_select);
    sram_set_cs(&ram->banks[1], ram->chip_select);
    sram_set_cs(&ram->banks[2], ram->chip_select);
    sram_set_cs(&ram->banks[3], ram->chip_select);
    sram_set_cs(&ram->banks[4], ram->chip_select);
    sram_set_cs(&ram->banks[5], ram->chip_select);
    sram_set_cs(&ram->banks[6], ram->chip_select);
    sram_set_cs(&ram->banks[7], ram->chip_select);
    sram_set_cs(&ram->banks[8], ram->chip_select);
    sram_set_cs(&ram->banks[9], ram->chip_select);
    sram_set_cs(&ram->banks[10], ram->chip_select);
    sram_set_cs(&ram->banks[11], ram->chip_select);
    sram_set_cs(&ram->banks[12], ram->chip_select);
    sram_set_cs(&ram->banks[13], ram->chip_select);
    sram_set_cs(&ram->banks[14], ram->chip_select);
    sram_set_cs(&ram->banks[15], ram->chip_select);

    sram_clock_fall(&ram->banks[0]); sram_clock_fall(&ram->banks[1]);
    sram_clock_fall(&ram->banks[2]); sram_clock_fall(&ram->banks[3]);
    sram_clock_fall(&ram->banks[4]); sram_clock_fall(&ram->banks[5]);
    sram_clock_fall(&ram->banks[6]); sram_clock_fall(&ram->banks[7]);
    sram_clock_fall(&ram->banks[8]); sram_clock_fall(&ram->banks[9]);
    sram_clock_fall(&ram->banks[10]); sram_clock_fall(&ram->banks[11]);
    sram_clock_fall(&ram->banks[12]); sram_clock_fall(&ram->banks[13]);
    sram_clock_fall(&ram->banks[14]); sram_clock_fall(&ram->banks[15]);
}

void sram256_propagate(sram_256x32_t *ram) {
    bit d0[32], d1[32], d2[32], d3[32], d4[32], d5[32], d6[32], d7[32];
    bit d8[32], d9[32], d10[32], d11[32], d12[32], d13[32], d14[32], d15[32];

    sram_set_addr(&ram->banks[0], ram->addr);
    sram_set_addr(&ram->banks[1], ram->addr);
    sram_set_addr(&ram->banks[2], ram->addr);
    sram_set_addr(&ram->banks[3], ram->addr);
    sram_set_addr(&ram->banks[4], ram->addr);
    sram_set_addr(&ram->banks[5], ram->addr);
    sram_set_addr(&ram->banks[6], ram->addr);
    sram_set_addr(&ram->banks[7], ram->addr);
    sram_set_addr(&ram->banks[8], ram->addr);
    sram_set_addr(&ram->banks[9], ram->addr);
    sram_set_addr(&ram->banks[10], ram->addr);
    sram_set_addr(&ram->banks[11], ram->addr);
    sram_set_addr(&ram->banks[12], ram->addr);
    sram_set_addr(&ram->banks[13], ram->addr);
    sram_set_addr(&ram->banks[14], ram->addr);
    sram_set_addr(&ram->banks[15], ram->addr);

    sram_propagate(&ram->banks[0]); sram_propagate(&ram->banks[1]);
    sram_propagate(&ram->banks[2]); sram_propagate(&ram->banks[3]);
    sram_propagate(&ram->banks[4]); sram_propagate(&ram->banks[5]);
    sram_propagate(&ram->banks[6]); sram_propagate(&ram->banks[7]);
    sram_propagate(&ram->banks[8]); sram_propagate(&ram->banks[9]);
    sram_propagate(&ram->banks[10]); sram_propagate(&ram->banks[11]);
    sram_propagate(&ram->banks[12]); sram_propagate(&ram->banks[13]);
    sram_propagate(&ram->banks[14]); sram_propagate(&ram->banks[15]);

    sram_read(&ram->banks[0], d0); sram_read(&ram->banks[1], d1);
    sram_read(&ram->banks[2], d2); sram_read(&ram->banks[3], d3);
    sram_read(&ram->banks[4], d4); sram_read(&ram->banks[5], d5);
    sram_read(&ram->banks[6], d6); sram_read(&ram->banks[7], d7);
    sram_read(&ram->banks[8], d8); sram_read(&ram->banks[9], d9);
    sram_read(&ram->banks[10], d10); sram_read(&ram->banks[11], d11);
    sram_read(&ram->banks[12], d12); sram_read(&ram->banks[13], d13);
    sram_read(&ram->banks[14], d14); sram_read(&ram->banks[15], d15);

    bit t01[32], t23[32], t45[32], t67[32], t89[32], tab[32], tcd[32], tef[32];
    bit t0123[32], t4567[32], t89ab[32], tcdef[32];
    bit t_lo[32], t_hi[32];

    mux2_32(d0, d1, ram->addr[4], t01);
    mux2_32(d2, d3, ram->addr[4], t23);
    mux2_32(d4, d5, ram->addr[4], t45);
    mux2_32(d6, d7, ram->addr[4], t67);
    mux2_32(d8, d9, ram->addr[4], t89);
    mux2_32(d10, d11, ram->addr[4], tab);
    mux2_32(d12, d13, ram->addr[4], tcd);
    mux2_32(d14, d15, ram->addr[4], tef);

    mux2_32(t01, t23, ram->addr[5], t0123);
    mux2_32(t45, t67, ram->addr[5], t4567);
    mux2_32(t89, tab, ram->addr[5], t89ab);
    mux2_32(tcd, tef, ram->addr[5], tcdef);

    mux2_32(t0123, t4567, ram->addr[6], t_lo);
    mux2_32(t89ab, tcdef, ram->addr[6], t_hi);

    mux2_32(t_lo, t_hi, ram->addr[7], ram->data_out);
}

void sram256_read(sram_256x32_t *ram, bit *out) {
    out[0] = ram->data_out[0]; out[1] = ram->data_out[1];
    out[2] = ram->data_out[2]; out[3] = ram->data_out[3];
    out[4] = ram->data_out[4]; out[5] = ram->data_out[5];
    out[6] = ram->data_out[6]; out[7] = ram->data_out[7];
    out[8] = ram->data_out[8]; out[9] = ram->data_out[9];
    out[10] = ram->data_out[10]; out[11] = ram->data_out[11];
    out[12] = ram->data_out[12]; out[13] = ram->data_out[13];
    out[14] = ram->data_out[14]; out[15] = ram->data_out[15];
    out[16] = ram->data_out[16]; out[17] = ram->data_out[17];
    out[18] = ram->data_out[18]; out[19] = ram->data_out[19];
    out[20] = ram->data_out[20]; out[21] = ram->data_out[21];
    out[22] = ram->data_out[22]; out[23] = ram->data_out[23];
    out[24] = ram->data_out[24]; out[25] = ram->data_out[25];
    out[26] = ram->data_out[26]; out[27] = ram->data_out[27];
    out[28] = ram->data_out[28]; out[29] = ram->data_out[29];
    out[30] = ram->data_out[30]; out[31] = ram->data_out[31];
}
