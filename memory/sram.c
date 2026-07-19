#include "sram.h"
#include "../core/gates/gates.h"
#include "../core/mux/mux.h"

void sram_init(sram_16x32_t *ram) {
    cell32_init(&ram->cells[0]); cell32_init(&ram->cells[1]);
    cell32_init(&ram->cells[2]); cell32_init(&ram->cells[3]);
    cell32_init(&ram->cells[4]); cell32_init(&ram->cells[5]);
    cell32_init(&ram->cells[6]); cell32_init(&ram->cells[7]);
    cell32_init(&ram->cells[8]); cell32_init(&ram->cells[9]);
    cell32_init(&ram->cells[10]); cell32_init(&ram->cells[11]);
    cell32_init(&ram->cells[12]); cell32_init(&ram->cells[13]);
    cell32_init(&ram->cells[14]); cell32_init(&ram->cells[15]);

    ram->addr[0] = 0; ram->addr[1] = 0; ram->addr[2] = 0; ram->addr[3] = 0;
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

void sram_reset(sram_16x32_t *ram) {
    sram_init(ram);
}

void sram_set_addr(sram_16x32_t *ram, bit *addr) {
    ram->addr[0] = addr[0]; ram->addr[1] = addr[1];
    ram->addr[2] = addr[2]; ram->addr[3] = addr[3];
}

void sram_set_data(sram_16x32_t *ram, bit *data) {
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

void sram_set_write(sram_16x32_t *ram, bit we) {
    ram->write_enable = we;
}

void sram_set_cs(sram_16x32_t *ram, bit cs) {
    ram->chip_select = cs;
}

void sram_clock_rise(sram_16x32_t *ram) {
    cell32_clock_rise(&ram->cells[0]); cell32_clock_rise(&ram->cells[1]);
    cell32_clock_rise(&ram->cells[2]); cell32_clock_rise(&ram->cells[3]);
    cell32_clock_rise(&ram->cells[4]); cell32_clock_rise(&ram->cells[5]);
    cell32_clock_rise(&ram->cells[6]); cell32_clock_rise(&ram->cells[7]);
    cell32_clock_rise(&ram->cells[8]); cell32_clock_rise(&ram->cells[9]);
    cell32_clock_rise(&ram->cells[10]); cell32_clock_rise(&ram->cells[11]);
    cell32_clock_rise(&ram->cells[12]); cell32_clock_rise(&ram->cells[13]);
    cell32_clock_rise(&ram->cells[14]); cell32_clock_rise(&ram->cells[15]);
}

void sram_clock_fall(sram_16x32_t *ram) {
    bit n0 = NOT(ram->addr[0]);
    bit n1 = NOT(ram->addr[1]);
    bit n2 = NOT(ram->addr[2]);
    bit n3 = NOT(ram->addr[3]);

    bit sel[16];
    sel[0] = AND(AND(n3, n2), AND(n1, n0));
    sel[1] = AND(AND(n3, n2), AND(n1, ram->addr[0]));
    sel[2] = AND(AND(n3, n2), AND(ram->addr[1], n0));
    sel[3] = AND(AND(n3, n2), AND(ram->addr[1], ram->addr[0]));
    sel[4] = AND(AND(n3, ram->addr[2]), AND(n1, n0));
    sel[5] = AND(AND(n3, ram->addr[2]), AND(n1, ram->addr[0]));
    sel[6] = AND(AND(n3, ram->addr[2]), AND(ram->addr[1], n0));
    sel[7] = AND(AND(n3, ram->addr[2]), AND(ram->addr[1], ram->addr[0]));
    sel[8] = AND(AND(ram->addr[3], n2), AND(n1, n0));
    sel[9] = AND(AND(ram->addr[3], n2), AND(n1, ram->addr[0]));
    sel[10] = AND(AND(ram->addr[3], n2), AND(ram->addr[1], n0));
    sel[11] = AND(AND(ram->addr[3], n2), AND(ram->addr[1], ram->addr[0]));
    sel[12] = AND(AND(ram->addr[3], ram->addr[2]), AND(n1, n0));
    sel[13] = AND(AND(ram->addr[3], ram->addr[2]), AND(n1, ram->addr[0]));
    sel[14] = AND(AND(ram->addr[3], ram->addr[2]), AND(ram->addr[1], n0));
    sel[15] = AND(AND(ram->addr[3], ram->addr[2]), AND(ram->addr[1], ram->addr[0]));

    bit we = AND(ram->write_enable, ram->chip_select);

    cell32_set_write(&ram->cells[0], AND(we, sel[0]), ram->data_in);
    cell32_set_write(&ram->cells[1], AND(we, sel[1]), ram->data_in);
    cell32_set_write(&ram->cells[2], AND(we, sel[2]), ram->data_in);
    cell32_set_write(&ram->cells[3], AND(we, sel[3]), ram->data_in);
    cell32_set_write(&ram->cells[4], AND(we, sel[4]), ram->data_in);
    cell32_set_write(&ram->cells[5], AND(we, sel[5]), ram->data_in);
    cell32_set_write(&ram->cells[6], AND(we, sel[6]), ram->data_in);
    cell32_set_write(&ram->cells[7], AND(we, sel[7]), ram->data_in);
    cell32_set_write(&ram->cells[8], AND(we, sel[8]), ram->data_in);
    cell32_set_write(&ram->cells[9], AND(we, sel[9]), ram->data_in);
    cell32_set_write(&ram->cells[10], AND(we, sel[10]), ram->data_in);
    cell32_set_write(&ram->cells[11], AND(we, sel[11]), ram->data_in);
    cell32_set_write(&ram->cells[12], AND(we, sel[12]), ram->data_in);
    cell32_set_write(&ram->cells[13], AND(we, sel[13]), ram->data_in);
    cell32_set_write(&ram->cells[14], AND(we, sel[14]), ram->data_in);
    cell32_set_write(&ram->cells[15], AND(we, sel[15]), ram->data_in);

    cell32_clock_fall(&ram->cells[0]); cell32_clock_fall(&ram->cells[1]);
    cell32_clock_fall(&ram->cells[2]); cell32_clock_fall(&ram->cells[3]);
    cell32_clock_fall(&ram->cells[4]); cell32_clock_fall(&ram->cells[5]);
    cell32_clock_fall(&ram->cells[6]); cell32_clock_fall(&ram->cells[7]);
    cell32_clock_fall(&ram->cells[8]); cell32_clock_fall(&ram->cells[9]);
    cell32_clock_fall(&ram->cells[10]); cell32_clock_fall(&ram->cells[11]);
    cell32_clock_fall(&ram->cells[12]); cell32_clock_fall(&ram->cells[13]);
    cell32_clock_fall(&ram->cells[14]); cell32_clock_fall(&ram->cells[15]);
}

void sram_propagate(sram_16x32_t *ram) {
    bit d0[32], d1[32], d2[32], d3[32], d4[32], d5[32], d6[32], d7[32];
    bit d8[32], d9[32], d10[32], d11[32], d12[32], d13[32], d14[32], d15[32];

    cell32_read(&ram->cells[0], d0); cell32_read(&ram->cells[1], d1);
    cell32_read(&ram->cells[2], d2); cell32_read(&ram->cells[3], d3);
    cell32_read(&ram->cells[4], d4); cell32_read(&ram->cells[5], d5);
    cell32_read(&ram->cells[6], d6); cell32_read(&ram->cells[7], d7);
    cell32_read(&ram->cells[8], d8); cell32_read(&ram->cells[9], d9);
    cell32_read(&ram->cells[10], d10); cell32_read(&ram->cells[11], d11);
    cell32_read(&ram->cells[12], d12); cell32_read(&ram->cells[13], d13);
    cell32_read(&ram->cells[14], d14); cell32_read(&ram->cells[15], d15);

    bit t01[32], t23[32], t45[32], t67[32], t89[32], tab[32], tcd[32], tef[32];
    bit t0123[32], t4567[32], t89ab[32], tcdef[32];
    bit t_lo[32], t_hi[32];

    mux2_32(d0, d1, ram->addr[0], t01);
    mux2_32(d2, d3, ram->addr[0], t23);
    mux2_32(d4, d5, ram->addr[0], t45);
    mux2_32(d6, d7, ram->addr[0], t67);
    mux2_32(d8, d9, ram->addr[0], t89);
    mux2_32(d10, d11, ram->addr[0], tab);
    mux2_32(d12, d13, ram->addr[0], tcd);
    mux2_32(d14, d15, ram->addr[0], tef);

    mux2_32(t01, t23, ram->addr[1], t0123);
    mux2_32(t45, t67, ram->addr[1], t4567);
    mux2_32(t89, tab, ram->addr[1], t89ab);
    mux2_32(tcd, tef, ram->addr[1], tcdef);

    mux2_32(t0123, t4567, ram->addr[2], t_lo);
    mux2_32(t89ab, tcdef, ram->addr[2], t_hi);

    mux2_32(t_lo, t_hi, ram->addr[3], ram->data_out);
}

void sram_read(sram_16x32_t *ram, bit *out) {
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
