#include "rom1k.h"
#include "../core/gates/gates.h"
#include "../core/mux/mux.h"

void rom1k_init(rom_1kx32_t *rom) {
    rom256_init(&rom->banks[0]);
    rom256_init(&rom->banks[1]);
    rom256_init(&rom->banks[2]);
    rom256_init(&rom->banks[3]);
}

void rom1k_program(rom_1kx32_t *rom, bit *a, bit *data) {
    bit n8 = NOT(a[8]), n9 = NOT(a[9]);
    rom256_program(&rom->banks[0], a, AND(n8, n9), data);
    rom256_program(&rom->banks[1], a, AND(a[8], n9), data);
    rom256_program(&rom->banks[2], a, AND(n8, a[9]), data);
    rom256_program(&rom->banks[3], a, AND(a[8], a[9]), data);
}

static void read_bank(rom_256x32_t *bank, bit *a, bit *out) {
    rom256_set_addr(bank, a);
    rom256_set_cs(bank, 1);
    rom256_propagate(bank);
    rom256_read(bank, out);
}

void rom1k_fetch(rom_1kx32_t *rom, bit *a, bit *out) {
    bit w0[32], w1[32], w2[32], w3[32];
    read_bank(&rom->banks[0], a, w0);
    read_bank(&rom->banks[1], a, w1);
    read_bank(&rom->banks[2], a, w2);
    read_bank(&rom->banks[3], a, w3);
    mux4_32(w0, w1, w2, w3, a[8], a[9], rom->data_out);
    out[0] = rom->data_out[0]; out[1] = rom->data_out[1]; out[2] = rom->data_out[2]; out[3] = rom->data_out[3];
    out[4] = rom->data_out[4]; out[5] = rom->data_out[5]; out[6] = rom->data_out[6]; out[7] = rom->data_out[7];
    out[8] = rom->data_out[8]; out[9] = rom->data_out[9]; out[10] = rom->data_out[10]; out[11] = rom->data_out[11];
    out[12] = rom->data_out[12]; out[13] = rom->data_out[13]; out[14] = rom->data_out[14]; out[15] = rom->data_out[15];
    out[16] = rom->data_out[16]; out[17] = rom->data_out[17]; out[18] = rom->data_out[18]; out[19] = rom->data_out[19];
    out[20] = rom->data_out[20]; out[21] = rom->data_out[21]; out[22] = rom->data_out[22]; out[23] = rom->data_out[23];
    out[24] = rom->data_out[24]; out[25] = rom->data_out[25]; out[26] = rom->data_out[26]; out[27] = rom->data_out[27];
    out[28] = rom->data_out[28]; out[29] = rom->data_out[29]; out[30] = rom->data_out[30]; out[31] = rom->data_out[31];
}
