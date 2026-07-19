#include "rom256.h"
#include "../core/gates/gates.h"
#include "../core/mux/mux.h"

void rom256_init(rom_256x32_t *rom) {
    rom_init(&rom->chips[0]); rom_init(&rom->chips[1]);
    rom_init(&rom->chips[2]); rom_init(&rom->chips[3]);
    rom_init(&rom->chips[4]); rom_init(&rom->chips[5]);
    rom_init(&rom->chips[6]); rom_init(&rom->chips[7]);
    rom_init(&rom->chips[8]); rom_init(&rom->chips[9]);
    rom_init(&rom->chips[10]); rom_init(&rom->chips[11]);
    rom_init(&rom->chips[12]); rom_init(&rom->chips[13]);
    rom_init(&rom->chips[14]); rom_init(&rom->chips[15]);

    rom->addr[0] = 0; rom->addr[1] = 0; rom->addr[2] = 0; rom->addr[3] = 0;
    rom->addr[4] = 0; rom->addr[5] = 0; rom->addr[6] = 0; rom->addr[7] = 0;
    rom->chip_select = 0;

    int j;
    j = 0; rom->data_out[j] = 0; j = 1; rom->data_out[j] = 0;
    j = 2; rom->data_out[j] = 0; j = 3; rom->data_out[j] = 0;
    j = 4; rom->data_out[j] = 0; j = 5; rom->data_out[j] = 0;
    j = 6; rom->data_out[j] = 0; j = 7; rom->data_out[j] = 0;
    j = 8; rom->data_out[j] = 0; j = 9; rom->data_out[j] = 0;
    j = 10; rom->data_out[j] = 0; j = 11; rom->data_out[j] = 0;
    j = 12; rom->data_out[j] = 0; j = 13; rom->data_out[j] = 0;
    j = 14; rom->data_out[j] = 0; j = 15; rom->data_out[j] = 0;
    j = 16; rom->data_out[j] = 0; j = 17; rom->data_out[j] = 0;
    j = 18; rom->data_out[j] = 0; j = 19; rom->data_out[j] = 0;
    j = 20; rom->data_out[j] = 0; j = 21; rom->data_out[j] = 0;
    j = 22; rom->data_out[j] = 0; j = 23; rom->data_out[j] = 0;
    j = 24; rom->data_out[j] = 0; j = 25; rom->data_out[j] = 0;
    j = 26; rom->data_out[j] = 0; j = 27; rom->data_out[j] = 0;
    j = 28; rom->data_out[j] = 0; j = 29; rom->data_out[j] = 0;
    j = 30; rom->data_out[j] = 0; j = 31; rom->data_out[j] = 0;
}

void rom256_reset(rom_256x32_t *rom) {
    rom256_init(rom);
}

void rom256_load_word(rom_256x32_t *rom, int index, bit *data) {
    int chip = (index >> 4) & 15;
    int offset = index & 15;
    rom_load_word(&rom->chips[chip], offset, data);
}

void rom256_set_addr(rom_256x32_t *rom, bit *addr) {
    rom->addr[0] = addr[0]; rom->addr[1] = addr[1];
    rom->addr[2] = addr[2]; rom->addr[3] = addr[3];
    rom->addr[4] = addr[4]; rom->addr[5] = addr[5];
    rom->addr[6] = addr[6]; rom->addr[7] = addr[7];
}

void rom256_set_cs(rom_256x32_t *rom, bit cs) {
    rom->chip_select = cs;
}

void rom256_propagate(rom_256x32_t *rom) {
    rom_set_addr(&rom->chips[0], rom->addr);
    rom_set_addr(&rom->chips[1], rom->addr);
    rom_set_addr(&rom->chips[2], rom->addr);
    rom_set_addr(&rom->chips[3], rom->addr);
    rom_set_addr(&rom->chips[4], rom->addr);
    rom_set_addr(&rom->chips[5], rom->addr);
    rom_set_addr(&rom->chips[6], rom->addr);
    rom_set_addr(&rom->chips[7], rom->addr);
    rom_set_addr(&rom->chips[8], rom->addr);
    rom_set_addr(&rom->chips[9], rom->addr);
    rom_set_addr(&rom->chips[10], rom->addr);
    rom_set_addr(&rom->chips[11], rom->addr);
    rom_set_addr(&rom->chips[12], rom->addr);
    rom_set_addr(&rom->chips[13], rom->addr);
    rom_set_addr(&rom->chips[14], rom->addr);
    rom_set_addr(&rom->chips[15], rom->addr);

    rom_set_cs(&rom->chips[0], 1); rom_set_cs(&rom->chips[1], 1);
    rom_set_cs(&rom->chips[2], 1); rom_set_cs(&rom->chips[3], 1);
    rom_set_cs(&rom->chips[4], 1); rom_set_cs(&rom->chips[5], 1);
    rom_set_cs(&rom->chips[6], 1); rom_set_cs(&rom->chips[7], 1);
    rom_set_cs(&rom->chips[8], 1); rom_set_cs(&rom->chips[9], 1);
    rom_set_cs(&rom->chips[10], 1); rom_set_cs(&rom->chips[11], 1);
    rom_set_cs(&rom->chips[12], 1); rom_set_cs(&rom->chips[13], 1);
    rom_set_cs(&rom->chips[14], 1); rom_set_cs(&rom->chips[15], 1);

    rom_propagate(&rom->chips[0]); rom_propagate(&rom->chips[1]);
    rom_propagate(&rom->chips[2]); rom_propagate(&rom->chips[3]);
    rom_propagate(&rom->chips[4]); rom_propagate(&rom->chips[5]);
    rom_propagate(&rom->chips[6]); rom_propagate(&rom->chips[7]);
    rom_propagate(&rom->chips[8]); rom_propagate(&rom->chips[9]);
    rom_propagate(&rom->chips[10]); rom_propagate(&rom->chips[11]);
    rom_propagate(&rom->chips[12]); rom_propagate(&rom->chips[13]);
    rom_propagate(&rom->chips[14]); rom_propagate(&rom->chips[15]);

    bit d0[32], d1[32], d2[32], d3[32], d4[32], d5[32], d6[32], d7[32];
    bit d8[32], d9[32], d10[32], d11[32], d12[32], d13[32], d14[32], d15[32];

    rom_read(&rom->chips[0], d0); rom_read(&rom->chips[1], d1);
    rom_read(&rom->chips[2], d2); rom_read(&rom->chips[3], d3);
    rom_read(&rom->chips[4], d4); rom_read(&rom->chips[5], d5);
    rom_read(&rom->chips[6], d6); rom_read(&rom->chips[7], d7);
    rom_read(&rom->chips[8], d8); rom_read(&rom->chips[9], d9);
    rom_read(&rom->chips[10], d10); rom_read(&rom->chips[11], d11);
    rom_read(&rom->chips[12], d12); rom_read(&rom->chips[13], d13);
    rom_read(&rom->chips[14], d14); rom_read(&rom->chips[15], d15);

    bit t01[32], t23[32], t45[32], t67[32], t89[32], tab[32], tcd[32], tef[32];
    bit t0123[32], t4567[32], t89ab[32], tcdef[32];
    bit t_lo[32], t_hi[32];

    mux2_32(d0, d1, rom->addr[4], t01);
    mux2_32(d2, d3, rom->addr[4], t23);
    mux2_32(d4, d5, rom->addr[4], t45);
    mux2_32(d6, d7, rom->addr[4], t67);
    mux2_32(d8, d9, rom->addr[4], t89);
    mux2_32(d10, d11, rom->addr[4], tab);
    mux2_32(d12, d13, rom->addr[4], tcd);
    mux2_32(d14, d15, rom->addr[4], tef);

    mux2_32(t01, t23, rom->addr[5], t0123);
    mux2_32(t45, t67, rom->addr[5], t4567);
    mux2_32(t89, tab, rom->addr[5], t89ab);
    mux2_32(tcd, tef, rom->addr[5], tcdef);

    mux2_32(t0123, t4567, rom->addr[6], t_lo);
    mux2_32(t89ab, tcdef, rom->addr[6], t_hi);

    mux2_32(t_lo, t_hi, rom->addr[7], rom->data_out);
}

void rom256_read(rom_256x32_t *rom, bit *out) {
    out[0] = AND(rom->data_out[0], rom->chip_select);
    out[1] = AND(rom->data_out[1], rom->chip_select);
    out[2] = AND(rom->data_out[2], rom->chip_select);
    out[3] = AND(rom->data_out[3], rom->chip_select);
    out[4] = AND(rom->data_out[4], rom->chip_select);
    out[5] = AND(rom->data_out[5], rom->chip_select);
    out[6] = AND(rom->data_out[6], rom->chip_select);
    out[7] = AND(rom->data_out[7], rom->chip_select);
    out[8] = AND(rom->data_out[8], rom->chip_select);
    out[9] = AND(rom->data_out[9], rom->chip_select);
    out[10] = AND(rom->data_out[10], rom->chip_select);
    out[11] = AND(rom->data_out[11], rom->chip_select);
    out[12] = AND(rom->data_out[12], rom->chip_select);
    out[13] = AND(rom->data_out[13], rom->chip_select);
    out[14] = AND(rom->data_out[14], rom->chip_select);
    out[15] = AND(rom->data_out[15], rom->chip_select);
    out[16] = AND(rom->data_out[16], rom->chip_select);
    out[17] = AND(rom->data_out[17], rom->chip_select);
    out[18] = AND(rom->data_out[18], rom->chip_select);
    out[19] = AND(rom->data_out[19], rom->chip_select);
    out[20] = AND(rom->data_out[20], rom->chip_select);
    out[21] = AND(rom->data_out[21], rom->chip_select);
    out[22] = AND(rom->data_out[22], rom->chip_select);
    out[23] = AND(rom->data_out[23], rom->chip_select);
    out[24] = AND(rom->data_out[24], rom->chip_select);
    out[25] = AND(rom->data_out[25], rom->chip_select);
    out[26] = AND(rom->data_out[26], rom->chip_select);
    out[27] = AND(rom->data_out[27], rom->chip_select);
    out[28] = AND(rom->data_out[28], rom->chip_select);
    out[29] = AND(rom->data_out[29], rom->chip_select);
    out[30] = AND(rom->data_out[30], rom->chip_select);
    out[31] = AND(rom->data_out[31], rom->chip_select);
}
