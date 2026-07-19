#include "cell.h"
#include "../core/gates/gates.h"
#include "../core/mux/mux.h"

void cell_init(memory_cell_t *cell) {
    dff_reset(&cell->data);
    cell->write_enable = 0;
    cell->data_in = 0;
}

void cell_reset(memory_cell_t *cell) {
    cell_init(cell);
}

void cell_set_write(memory_cell_t *cell, bit we, bit data) {
    cell->write_enable = we;
    cell->data_in = data;
}

void cell_clock_rise(memory_cell_t *cell) {
    dff_clock_rise(&cell->data);
}

void cell_clock_fall(memory_cell_t *cell) {
    cell->data.d = mux2(cell->data.q, cell->data_in, cell->write_enable);
    dff_clock_fall(&cell->data);
}

bit cell_read(memory_cell_t *cell) {
    return cell->data.q;
}

void cell8_init(memory_cell_8_t *cell) {
    dff_reset(&cell->bits[0]); dff_reset(&cell->bits[1]);
    dff_reset(&cell->bits[2]); dff_reset(&cell->bits[3]);
    dff_reset(&cell->bits[4]); dff_reset(&cell->bits[5]);
    dff_reset(&cell->bits[6]); dff_reset(&cell->bits[7]);
    cell->write_enable = 0;
    cell->data_in[0] = 0; cell->data_in[1] = 0;
    cell->data_in[2] = 0; cell->data_in[3] = 0;
    cell->data_in[4] = 0; cell->data_in[5] = 0;
    cell->data_in[6] = 0; cell->data_in[7] = 0;
}

void cell8_reset(memory_cell_8_t *cell) {
    cell8_init(cell);
}

void cell8_set_write(memory_cell_8_t *cell, bit we, bit *data) {
    cell->write_enable = we;
    cell->data_in[0] = data[0]; cell->data_in[1] = data[1];
    cell->data_in[2] = data[2]; cell->data_in[3] = data[3];
    cell->data_in[4] = data[4]; cell->data_in[5] = data[5];
    cell->data_in[6] = data[6]; cell->data_in[7] = data[7];
}

void cell8_clock_rise(memory_cell_8_t *cell) {
    dff_clock_rise(&cell->bits[0]); dff_clock_rise(&cell->bits[1]);
    dff_clock_rise(&cell->bits[2]); dff_clock_rise(&cell->bits[3]);
    dff_clock_rise(&cell->bits[4]); dff_clock_rise(&cell->bits[5]);
    dff_clock_rise(&cell->bits[6]); dff_clock_rise(&cell->bits[7]);
}

void cell8_clock_fall(memory_cell_8_t *cell) {
    cell->bits[0].d = mux2(cell->bits[0].q, cell->data_in[0], cell->write_enable);
    cell->bits[1].d = mux2(cell->bits[1].q, cell->data_in[1], cell->write_enable);
    cell->bits[2].d = mux2(cell->bits[2].q, cell->data_in[2], cell->write_enable);
    cell->bits[3].d = mux2(cell->bits[3].q, cell->data_in[3], cell->write_enable);
    cell->bits[4].d = mux2(cell->bits[4].q, cell->data_in[4], cell->write_enable);
    cell->bits[5].d = mux2(cell->bits[5].q, cell->data_in[5], cell->write_enable);
    cell->bits[6].d = mux2(cell->bits[6].q, cell->data_in[6], cell->write_enable);
    cell->bits[7].d = mux2(cell->bits[7].q, cell->data_in[7], cell->write_enable);

    dff_clock_fall(&cell->bits[0]); dff_clock_fall(&cell->bits[1]);
    dff_clock_fall(&cell->bits[2]); dff_clock_fall(&cell->bits[3]);
    dff_clock_fall(&cell->bits[4]); dff_clock_fall(&cell->bits[5]);
    dff_clock_fall(&cell->bits[6]); dff_clock_fall(&cell->bits[7]);
}

void cell8_read(memory_cell_8_t *cell, bit *out) {
    out[0] = cell->bits[0].q; out[1] = cell->bits[1].q;
    out[2] = cell->bits[2].q; out[3] = cell->bits[3].q;
    out[4] = cell->bits[4].q; out[5] = cell->bits[5].q;
    out[6] = cell->bits[6].q; out[7] = cell->bits[7].q;
}

void cell32_init(memory_cell_32_t *cell) {
    int i;
    i = 0; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 1; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 2; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 3; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 4; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 5; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 6; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 7; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 8; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 9; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 10; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 11; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 12; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 13; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 14; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 15; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 16; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 17; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 18; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 19; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 20; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 21; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 22; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 23; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 24; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 25; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 26; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 27; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 28; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 29; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 30; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    i = 31; dff_reset(&cell->bits[i]); cell->data_in[i] = 0;
    cell->write_enable = 0;
}

void cell32_reset(memory_cell_32_t *cell) {
    cell32_init(cell);
}

void cell32_set_write(memory_cell_32_t *cell, bit we, bit *data) {
    cell->write_enable = we;
    cell->data_in[0] = data[0]; cell->data_in[1] = data[1];
    cell->data_in[2] = data[2]; cell->data_in[3] = data[3];
    cell->data_in[4] = data[4]; cell->data_in[5] = data[5];
    cell->data_in[6] = data[6]; cell->data_in[7] = data[7];
    cell->data_in[8] = data[8]; cell->data_in[9] = data[9];
    cell->data_in[10] = data[10]; cell->data_in[11] = data[11];
    cell->data_in[12] = data[12]; cell->data_in[13] = data[13];
    cell->data_in[14] = data[14]; cell->data_in[15] = data[15];
    cell->data_in[16] = data[16]; cell->data_in[17] = data[17];
    cell->data_in[18] = data[18]; cell->data_in[19] = data[19];
    cell->data_in[20] = data[20]; cell->data_in[21] = data[21];
    cell->data_in[22] = data[22]; cell->data_in[23] = data[23];
    cell->data_in[24] = data[24]; cell->data_in[25] = data[25];
    cell->data_in[26] = data[26]; cell->data_in[27] = data[27];
    cell->data_in[28] = data[28]; cell->data_in[29] = data[29];
    cell->data_in[30] = data[30]; cell->data_in[31] = data[31];
}

void cell32_clock_rise(memory_cell_32_t *cell) {
    int i;
    i = 0; dff_clock_rise(&cell->bits[i]);
    i = 1; dff_clock_rise(&cell->bits[i]);
    i = 2; dff_clock_rise(&cell->bits[i]);
    i = 3; dff_clock_rise(&cell->bits[i]);
    i = 4; dff_clock_rise(&cell->bits[i]);
    i = 5; dff_clock_rise(&cell->bits[i]);
    i = 6; dff_clock_rise(&cell->bits[i]);
    i = 7; dff_clock_rise(&cell->bits[i]);
    i = 8; dff_clock_rise(&cell->bits[i]);
    i = 9; dff_clock_rise(&cell->bits[i]);
    i = 10; dff_clock_rise(&cell->bits[i]);
    i = 11; dff_clock_rise(&cell->bits[i]);
    i = 12; dff_clock_rise(&cell->bits[i]);
    i = 13; dff_clock_rise(&cell->bits[i]);
    i = 14; dff_clock_rise(&cell->bits[i]);
    i = 15; dff_clock_rise(&cell->bits[i]);
    i = 16; dff_clock_rise(&cell->bits[i]);
    i = 17; dff_clock_rise(&cell->bits[i]);
    i = 18; dff_clock_rise(&cell->bits[i]);
    i = 19; dff_clock_rise(&cell->bits[i]);
    i = 20; dff_clock_rise(&cell->bits[i]);
    i = 21; dff_clock_rise(&cell->bits[i]);
    i = 22; dff_clock_rise(&cell->bits[i]);
    i = 23; dff_clock_rise(&cell->bits[i]);
    i = 24; dff_clock_rise(&cell->bits[i]);
    i = 25; dff_clock_rise(&cell->bits[i]);
    i = 26; dff_clock_rise(&cell->bits[i]);
    i = 27; dff_clock_rise(&cell->bits[i]);
    i = 28; dff_clock_rise(&cell->bits[i]);
    i = 29; dff_clock_rise(&cell->bits[i]);
    i = 30; dff_clock_rise(&cell->bits[i]);
    i = 31; dff_clock_rise(&cell->bits[i]);
}

void cell32_clock_fall(memory_cell_32_t *cell) {
    bit we = cell->write_enable;
    cell->bits[0].d = mux2(cell->bits[0].q, cell->data_in[0], we);
    cell->bits[1].d = mux2(cell->bits[1].q, cell->data_in[1], we);
    cell->bits[2].d = mux2(cell->bits[2].q, cell->data_in[2], we);
    cell->bits[3].d = mux2(cell->bits[3].q, cell->data_in[3], we);
    cell->bits[4].d = mux2(cell->bits[4].q, cell->data_in[4], we);
    cell->bits[5].d = mux2(cell->bits[5].q, cell->data_in[5], we);
    cell->bits[6].d = mux2(cell->bits[6].q, cell->data_in[6], we);
    cell->bits[7].d = mux2(cell->bits[7].q, cell->data_in[7], we);
    cell->bits[8].d = mux2(cell->bits[8].q, cell->data_in[8], we);
    cell->bits[9].d = mux2(cell->bits[9].q, cell->data_in[9], we);
    cell->bits[10].d = mux2(cell->bits[10].q, cell->data_in[10], we);
    cell->bits[11].d = mux2(cell->bits[11].q, cell->data_in[11], we);
    cell->bits[12].d = mux2(cell->bits[12].q, cell->data_in[12], we);
    cell->bits[13].d = mux2(cell->bits[13].q, cell->data_in[13], we);
    cell->bits[14].d = mux2(cell->bits[14].q, cell->data_in[14], we);
    cell->bits[15].d = mux2(cell->bits[15].q, cell->data_in[15], we);
    cell->bits[16].d = mux2(cell->bits[16].q, cell->data_in[16], we);
    cell->bits[17].d = mux2(cell->bits[17].q, cell->data_in[17], we);
    cell->bits[18].d = mux2(cell->bits[18].q, cell->data_in[18], we);
    cell->bits[19].d = mux2(cell->bits[19].q, cell->data_in[19], we);
    cell->bits[20].d = mux2(cell->bits[20].q, cell->data_in[20], we);
    cell->bits[21].d = mux2(cell->bits[21].q, cell->data_in[21], we);
    cell->bits[22].d = mux2(cell->bits[22].q, cell->data_in[22], we);
    cell->bits[23].d = mux2(cell->bits[23].q, cell->data_in[23], we);
    cell->bits[24].d = mux2(cell->bits[24].q, cell->data_in[24], we);
    cell->bits[25].d = mux2(cell->bits[25].q, cell->data_in[25], we);
    cell->bits[26].d = mux2(cell->bits[26].q, cell->data_in[26], we);
    cell->bits[27].d = mux2(cell->bits[27].q, cell->data_in[27], we);
    cell->bits[28].d = mux2(cell->bits[28].q, cell->data_in[28], we);
    cell->bits[29].d = mux2(cell->bits[29].q, cell->data_in[29], we);
    cell->bits[30].d = mux2(cell->bits[30].q, cell->data_in[30], we);
    cell->bits[31].d = mux2(cell->bits[31].q, cell->data_in[31], we);

    int i;
    i = 0; dff_clock_fall(&cell->bits[i]);
    i = 1; dff_clock_fall(&cell->bits[i]);
    i = 2; dff_clock_fall(&cell->bits[i]);
    i = 3; dff_clock_fall(&cell->bits[i]);
    i = 4; dff_clock_fall(&cell->bits[i]);
    i = 5; dff_clock_fall(&cell->bits[i]);
    i = 6; dff_clock_fall(&cell->bits[i]);
    i = 7; dff_clock_fall(&cell->bits[i]);
    i = 8; dff_clock_fall(&cell->bits[i]);
    i = 9; dff_clock_fall(&cell->bits[i]);
    i = 10; dff_clock_fall(&cell->bits[i]);
    i = 11; dff_clock_fall(&cell->bits[i]);
    i = 12; dff_clock_fall(&cell->bits[i]);
    i = 13; dff_clock_fall(&cell->bits[i]);
    i = 14; dff_clock_fall(&cell->bits[i]);
    i = 15; dff_clock_fall(&cell->bits[i]);
    i = 16; dff_clock_fall(&cell->bits[i]);
    i = 17; dff_clock_fall(&cell->bits[i]);
    i = 18; dff_clock_fall(&cell->bits[i]);
    i = 19; dff_clock_fall(&cell->bits[i]);
    i = 20; dff_clock_fall(&cell->bits[i]);
    i = 21; dff_clock_fall(&cell->bits[i]);
    i = 22; dff_clock_fall(&cell->bits[i]);
    i = 23; dff_clock_fall(&cell->bits[i]);
    i = 24; dff_clock_fall(&cell->bits[i]);
    i = 25; dff_clock_fall(&cell->bits[i]);
    i = 26; dff_clock_fall(&cell->bits[i]);
    i = 27; dff_clock_fall(&cell->bits[i]);
    i = 28; dff_clock_fall(&cell->bits[i]);
    i = 29; dff_clock_fall(&cell->bits[i]);
    i = 30; dff_clock_fall(&cell->bits[i]);
    i = 31; dff_clock_fall(&cell->bits[i]);
}

void cell32_read(memory_cell_32_t *cell, bit *out) {
    out[0] = cell->bits[0].q; out[1] = cell->bits[1].q;
    out[2] = cell->bits[2].q; out[3] = cell->bits[3].q;
    out[4] = cell->bits[4].q; out[5] = cell->bits[5].q;
    out[6] = cell->bits[6].q; out[7] = cell->bits[7].q;
    out[8] = cell->bits[8].q; out[9] = cell->bits[9].q;
    out[10] = cell->bits[10].q; out[11] = cell->bits[11].q;
    out[12] = cell->bits[12].q; out[13] = cell->bits[13].q;
    out[14] = cell->bits[14].q; out[15] = cell->bits[15].q;
    out[16] = cell->bits[16].q; out[17] = cell->bits[17].q;
    out[18] = cell->bits[18].q; out[19] = cell->bits[19].q;
    out[20] = cell->bits[20].q; out[21] = cell->bits[21].q;
    out[22] = cell->bits[22].q; out[23] = cell->bits[23].q;
    out[24] = cell->bits[24].q; out[25] = cell->bits[25].q;
    out[26] = cell->bits[26].q; out[27] = cell->bits[27].q;
    out[28] = cell->bits[28].q; out[29] = cell->bits[29].q;
    out[30] = cell->bits[30].q; out[31] = cell->bits[31].q;
}
