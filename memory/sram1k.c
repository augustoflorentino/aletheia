#include "sram1k.h"
#include "../core/gates/gates.h"
#include "../core/mux/mux.h"

static void bank_select(bit a0, bit a1, bit *sel) {
    sel[0] = AND(NOT(a1), NOT(a0));
    sel[1] = AND(NOT(a1), a0);
    sel[2] = AND(a1, NOT(a0));
    sel[3] = AND(a1, a0);
}

static void drive_banks(sram_1kx32_t *ram) {
    bit sel[4];
    bank_select(ram->addr[8], ram->addr[9], sel);
    sram256_set_addr(&ram->banks[0], ram->addr);
    sram256_set_data(&ram->banks[0], ram->data_in);
    sram256_set_cs(&ram->banks[0], AND(ram->chip_select, sel[0]));
    sram256_set_write(&ram->banks[0], AND(ram->write_enable, sel[0]));
    sram256_set_addr(&ram->banks[1], ram->addr);
    sram256_set_data(&ram->banks[1], ram->data_in);
    sram256_set_cs(&ram->banks[1], AND(ram->chip_select, sel[1]));
    sram256_set_write(&ram->banks[1], AND(ram->write_enable, sel[1]));
    sram256_set_addr(&ram->banks[2], ram->addr);
    sram256_set_data(&ram->banks[2], ram->data_in);
    sram256_set_cs(&ram->banks[2], AND(ram->chip_select, sel[2]));
    sram256_set_write(&ram->banks[2], AND(ram->write_enable, sel[2]));
    sram256_set_addr(&ram->banks[3], ram->addr);
    sram256_set_data(&ram->banks[3], ram->data_in);
    sram256_set_cs(&ram->banks[3], AND(ram->chip_select, sel[3]));
    sram256_set_write(&ram->banks[3], AND(ram->write_enable, sel[3]));
}

void sram1k_init(sram_1kx32_t *ram) {
    sram256_init(&ram->banks[0]);
    sram256_init(&ram->banks[1]);
    sram256_init(&ram->banks[2]);
    sram256_init(&ram->banks[3]);
    ram->write_enable = 0;
    ram->chip_select = 0;
}

void sram1k_reset(sram_1kx32_t *ram) {
    sram256_reset(&ram->banks[0]);
    sram256_reset(&ram->banks[1]);
    sram256_reset(&ram->banks[2]);
    sram256_reset(&ram->banks[3]);
    ram->write_enable = 0;
    ram->chip_select = 0;
}

void sram1k_set_addr(sram_1kx32_t *ram, bit *addr) {
    ram->addr[0] = addr[0];
    ram->addr[1] = addr[1];
    ram->addr[2] = addr[2];
    ram->addr[3] = addr[3];
    ram->addr[4] = addr[4];
    ram->addr[5] = addr[5];
    ram->addr[6] = addr[6];
    ram->addr[7] = addr[7];
    ram->addr[8] = addr[8];
    ram->addr[9] = addr[9];
}

void sram1k_set_data(sram_1kx32_t *ram, bit *data) {
    ram->data_in[0] = data[0];
    ram->data_in[1] = data[1];
    ram->data_in[2] = data[2];
    ram->data_in[3] = data[3];
    ram->data_in[4] = data[4];
    ram->data_in[5] = data[5];
    ram->data_in[6] = data[6];
    ram->data_in[7] = data[7];
    ram->data_in[8] = data[8];
    ram->data_in[9] = data[9];
    ram->data_in[10] = data[10];
    ram->data_in[11] = data[11];
    ram->data_in[12] = data[12];
    ram->data_in[13] = data[13];
    ram->data_in[14] = data[14];
    ram->data_in[15] = data[15];
    ram->data_in[16] = data[16];
    ram->data_in[17] = data[17];
    ram->data_in[18] = data[18];
    ram->data_in[19] = data[19];
    ram->data_in[20] = data[20];
    ram->data_in[21] = data[21];
    ram->data_in[22] = data[22];
    ram->data_in[23] = data[23];
    ram->data_in[24] = data[24];
    ram->data_in[25] = data[25];
    ram->data_in[26] = data[26];
    ram->data_in[27] = data[27];
    ram->data_in[28] = data[28];
    ram->data_in[29] = data[29];
    ram->data_in[30] = data[30];
    ram->data_in[31] = data[31];
}

void sram1k_set_write(sram_1kx32_t *ram, bit we) {
    ram->write_enable = we;
}

void sram1k_set_cs(sram_1kx32_t *ram, bit cs) {
    ram->chip_select = cs;
}

void sram1k_clock_fall(sram_1kx32_t *ram) {
    drive_banks(ram);
    sram256_clock_fall(&ram->banks[0]);
    sram256_clock_fall(&ram->banks[1]);
    sram256_clock_fall(&ram->banks[2]);
    sram256_clock_fall(&ram->banks[3]);
}

void sram1k_clock_rise(sram_1kx32_t *ram) {
    sram256_clock_rise(&ram->banks[0]);
    sram256_clock_rise(&ram->banks[1]);
    sram256_clock_rise(&ram->banks[2]);
    sram256_clock_rise(&ram->banks[3]);
}

void sram1k_propagate(sram_1kx32_t *ram) {
    drive_banks(ram);
    bit d0[32], d1[32], d2[32], d3[32];
    sram256_propagate(&ram->banks[0]);
    sram256_propagate(&ram->banks[1]);
    sram256_propagate(&ram->banks[2]);
    sram256_propagate(&ram->banks[3]);
    sram256_read(&ram->banks[0], d0);
    sram256_read(&ram->banks[1], d1);
    sram256_read(&ram->banks[2], d2);
    sram256_read(&ram->banks[3], d3);
    mux4_32(d0, d1, d2, d3, ram->addr[8], ram->addr[9], ram->data_out);
}

void sram1k_read(sram_1kx32_t *ram, bit *out) {
    out[0] = ram->data_out[0];
    out[1] = ram->data_out[1];
    out[2] = ram->data_out[2];
    out[3] = ram->data_out[3];
    out[4] = ram->data_out[4];
    out[5] = ram->data_out[5];
    out[6] = ram->data_out[6];
    out[7] = ram->data_out[7];
    out[8] = ram->data_out[8];
    out[9] = ram->data_out[9];
    out[10] = ram->data_out[10];
    out[11] = ram->data_out[11];
    out[12] = ram->data_out[12];
    out[13] = ram->data_out[13];
    out[14] = ram->data_out[14];
    out[15] = ram->data_out[15];
    out[16] = ram->data_out[16];
    out[17] = ram->data_out[17];
    out[18] = ram->data_out[18];
    out[19] = ram->data_out[19];
    out[20] = ram->data_out[20];
    out[21] = ram->data_out[21];
    out[22] = ram->data_out[22];
    out[23] = ram->data_out[23];
    out[24] = ram->data_out[24];
    out[25] = ram->data_out[25];
    out[26] = ram->data_out[26];
    out[27] = ram->data_out[27];
    out[28] = ram->data_out[28];
    out[29] = ram->data_out[29];
    out[30] = ram->data_out[30];
    out[31] = ram->data_out[31];
}
