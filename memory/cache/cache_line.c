#include "cache_line.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

void cache_line_init(cache_line_t *line) {
    dff_reset(&line->valid);
    dff_reset(&line->dirty);

    int i;
    i = 0; dff_reset(&line->tag[i]); i = 1; dff_reset(&line->tag[i]);
    i = 2; dff_reset(&line->tag[i]); i = 3; dff_reset(&line->tag[i]);
    i = 4; dff_reset(&line->tag[i]); i = 5; dff_reset(&line->tag[i]);
    i = 6; dff_reset(&line->tag[i]); i = 7; dff_reset(&line->tag[i]);
    i = 8; dff_reset(&line->tag[i]); i = 9; dff_reset(&line->tag[i]);
    i = 10; dff_reset(&line->tag[i]); i = 11; dff_reset(&line->tag[i]);
    i = 12; dff_reset(&line->tag[i]); i = 13; dff_reset(&line->tag[i]);
    i = 14; dff_reset(&line->tag[i]); i = 15; dff_reset(&line->tag[i]);
    i = 16; dff_reset(&line->tag[i]); i = 17; dff_reset(&line->tag[i]);
    i = 18; dff_reset(&line->tag[i]); i = 19; dff_reset(&line->tag[i]);

    i = 0; dff_reset(&line->data[i]); i = 1; dff_reset(&line->data[i]);
    i = 2; dff_reset(&line->data[i]); i = 3; dff_reset(&line->data[i]);
    i = 4; dff_reset(&line->data[i]); i = 5; dff_reset(&line->data[i]);
    i = 6; dff_reset(&line->data[i]); i = 7; dff_reset(&line->data[i]);
    i = 8; dff_reset(&line->data[i]); i = 9; dff_reset(&line->data[i]);
    i = 10; dff_reset(&line->data[i]); i = 11; dff_reset(&line->data[i]);
    i = 12; dff_reset(&line->data[i]); i = 13; dff_reset(&line->data[i]);
    i = 14; dff_reset(&line->data[i]); i = 15; dff_reset(&line->data[i]);
    i = 16; dff_reset(&line->data[i]); i = 17; dff_reset(&line->data[i]);
    i = 18; dff_reset(&line->data[i]); i = 19; dff_reset(&line->data[i]);
    i = 20; dff_reset(&line->data[i]); i = 21; dff_reset(&line->data[i]);
    i = 22; dff_reset(&line->data[i]); i = 23; dff_reset(&line->data[i]);
    i = 24; dff_reset(&line->data[i]); i = 25; dff_reset(&line->data[i]);
    i = 26; dff_reset(&line->data[i]); i = 27; dff_reset(&line->data[i]);
    i = 28; dff_reset(&line->data[i]); i = 29; dff_reset(&line->data[i]);
    i = 30; dff_reset(&line->data[i]); i = 31; dff_reset(&line->data[i]);

    line->write_enable = 0;
    line->set_valid = 0;
    line->set_dirty = 0;
    line->clear_dirty = 0;

    i = 0; line->tag_in[i] = 0; i = 1; line->tag_in[i] = 0;
    i = 2; line->tag_in[i] = 0; i = 3; line->tag_in[i] = 0;
    i = 4; line->tag_in[i] = 0; i = 5; line->tag_in[i] = 0;
    i = 6; line->tag_in[i] = 0; i = 7; line->tag_in[i] = 0;
    i = 8; line->tag_in[i] = 0; i = 9; line->tag_in[i] = 0;
    i = 10; line->tag_in[i] = 0; i = 11; line->tag_in[i] = 0;
    i = 12; line->tag_in[i] = 0; i = 13; line->tag_in[i] = 0;
    i = 14; line->tag_in[i] = 0; i = 15; line->tag_in[i] = 0;
    i = 16; line->tag_in[i] = 0; i = 17; line->tag_in[i] = 0;
    i = 18; line->tag_in[i] = 0; i = 19; line->tag_in[i] = 0;

    i = 0; line->data_in[i] = 0; i = 1; line->data_in[i] = 0;
    i = 2; line->data_in[i] = 0; i = 3; line->data_in[i] = 0;
    i = 4; line->data_in[i] = 0; i = 5; line->data_in[i] = 0;
    i = 6; line->data_in[i] = 0; i = 7; line->data_in[i] = 0;
    i = 8; line->data_in[i] = 0; i = 9; line->data_in[i] = 0;
    i = 10; line->data_in[i] = 0; i = 11; line->data_in[i] = 0;
    i = 12; line->data_in[i] = 0; i = 13; line->data_in[i] = 0;
    i = 14; line->data_in[i] = 0; i = 15; line->data_in[i] = 0;
    i = 16; line->data_in[i] = 0; i = 17; line->data_in[i] = 0;
    i = 18; line->data_in[i] = 0; i = 19; line->data_in[i] = 0;
    i = 20; line->data_in[i] = 0; i = 21; line->data_in[i] = 0;
    i = 22; line->data_in[i] = 0; i = 23; line->data_in[i] = 0;
    i = 24; line->data_in[i] = 0; i = 25; line->data_in[i] = 0;
    i = 26; line->data_in[i] = 0; i = 27; line->data_in[i] = 0;
    i = 28; line->data_in[i] = 0; i = 29; line->data_in[i] = 0;
    i = 30; line->data_in[i] = 0; i = 31; line->data_in[i] = 0;
}

void cache_line_reset(cache_line_t *line) {
    cache_line_init(line);
}

void cache_line_set_write(cache_line_t *line, bit we) {
    line->write_enable = we;
}

void cache_line_set_valid(cache_line_t *line, bit valid) {
    line->set_valid = valid;
}

void cache_line_set_dirty(cache_line_t *line, bit dirty, bit clear) {
    line->set_dirty = dirty;
    line->clear_dirty = clear;
}

void cache_line_set_tag(cache_line_t *line, bit *tag) {
    line->tag_in[0] = tag[0]; line->tag_in[1] = tag[1];
    line->tag_in[2] = tag[2]; line->tag_in[3] = tag[3];
    line->tag_in[4] = tag[4]; line->tag_in[5] = tag[5];
    line->tag_in[6] = tag[6]; line->tag_in[7] = tag[7];
    line->tag_in[8] = tag[8]; line->tag_in[9] = tag[9];
    line->tag_in[10] = tag[10]; line->tag_in[11] = tag[11];
    line->tag_in[12] = tag[12]; line->tag_in[13] = tag[13];
    line->tag_in[14] = tag[14]; line->tag_in[15] = tag[15];
    line->tag_in[16] = tag[16]; line->tag_in[17] = tag[17];
    line->tag_in[18] = tag[18]; line->tag_in[19] = tag[19];
}

void cache_line_set_data(cache_line_t *line, bit *data) {
    line->data_in[0] = data[0]; line->data_in[1] = data[1];
    line->data_in[2] = data[2]; line->data_in[3] = data[3];
    line->data_in[4] = data[4]; line->data_in[5] = data[5];
    line->data_in[6] = data[6]; line->data_in[7] = data[7];
    line->data_in[8] = data[8]; line->data_in[9] = data[9];
    line->data_in[10] = data[10]; line->data_in[11] = data[11];
    line->data_in[12] = data[12]; line->data_in[13] = data[13];
    line->data_in[14] = data[14]; line->data_in[15] = data[15];
    line->data_in[16] = data[16]; line->data_in[17] = data[17];
    line->data_in[18] = data[18]; line->data_in[19] = data[19];
    line->data_in[20] = data[20]; line->data_in[21] = data[21];
    line->data_in[22] = data[22]; line->data_in[23] = data[23];
    line->data_in[24] = data[24]; line->data_in[25] = data[25];
    line->data_in[26] = data[26]; line->data_in[27] = data[27];
    line->data_in[28] = data[28]; line->data_in[29] = data[29];
    line->data_in[30] = data[30]; line->data_in[31] = data[31];
}

void cache_line_clock_rise(cache_line_t *line) {
    dff_clock_rise(&line->valid);
    dff_clock_rise(&line->dirty);

    int i;
    i = 0; dff_clock_rise(&line->tag[i]); i = 1; dff_clock_rise(&line->tag[i]);
    i = 2; dff_clock_rise(&line->tag[i]); i = 3; dff_clock_rise(&line->tag[i]);
    i = 4; dff_clock_rise(&line->tag[i]); i = 5; dff_clock_rise(&line->tag[i]);
    i = 6; dff_clock_rise(&line->tag[i]); i = 7; dff_clock_rise(&line->tag[i]);
    i = 8; dff_clock_rise(&line->tag[i]); i = 9; dff_clock_rise(&line->tag[i]);
    i = 10; dff_clock_rise(&line->tag[i]); i = 11; dff_clock_rise(&line->tag[i]);
    i = 12; dff_clock_rise(&line->tag[i]); i = 13; dff_clock_rise(&line->tag[i]);
    i = 14; dff_clock_rise(&line->tag[i]); i = 15; dff_clock_rise(&line->tag[i]);
    i = 16; dff_clock_rise(&line->tag[i]); i = 17; dff_clock_rise(&line->tag[i]);
    i = 18; dff_clock_rise(&line->tag[i]); i = 19; dff_clock_rise(&line->tag[i]);

    i = 0; dff_clock_rise(&line->data[i]); i = 1; dff_clock_rise(&line->data[i]);
    i = 2; dff_clock_rise(&line->data[i]); i = 3; dff_clock_rise(&line->data[i]);
    i = 4; dff_clock_rise(&line->data[i]); i = 5; dff_clock_rise(&line->data[i]);
    i = 6; dff_clock_rise(&line->data[i]); i = 7; dff_clock_rise(&line->data[i]);
    i = 8; dff_clock_rise(&line->data[i]); i = 9; dff_clock_rise(&line->data[i]);
    i = 10; dff_clock_rise(&line->data[i]); i = 11; dff_clock_rise(&line->data[i]);
    i = 12; dff_clock_rise(&line->data[i]); i = 13; dff_clock_rise(&line->data[i]);
    i = 14; dff_clock_rise(&line->data[i]); i = 15; dff_clock_rise(&line->data[i]);
    i = 16; dff_clock_rise(&line->data[i]); i = 17; dff_clock_rise(&line->data[i]);
    i = 18; dff_clock_rise(&line->data[i]); i = 19; dff_clock_rise(&line->data[i]);
    i = 20; dff_clock_rise(&line->data[i]); i = 21; dff_clock_rise(&line->data[i]);
    i = 22; dff_clock_rise(&line->data[i]); i = 23; dff_clock_rise(&line->data[i]);
    i = 24; dff_clock_rise(&line->data[i]); i = 25; dff_clock_rise(&line->data[i]);
    i = 26; dff_clock_rise(&line->data[i]); i = 27; dff_clock_rise(&line->data[i]);
    i = 28; dff_clock_rise(&line->data[i]); i = 29; dff_clock_rise(&line->data[i]);
    i = 30; dff_clock_rise(&line->data[i]); i = 31; dff_clock_rise(&line->data[i]);
}

void cache_line_clock_fall(cache_line_t *line) {
    bit we = line->write_enable;

    line->valid.d = mux2(line->valid.q, 1, line->set_valid);

    bit new_dirty = mux2(line->dirty.q, 1, line->set_dirty);
    new_dirty = mux2(new_dirty, 0, line->clear_dirty);
    line->dirty.d = new_dirty;

    line->tag[0].d = mux2(line->tag[0].q, line->tag_in[0], we);
    line->tag[1].d = mux2(line->tag[1].q, line->tag_in[1], we);
    line->tag[2].d = mux2(line->tag[2].q, line->tag_in[2], we);
    line->tag[3].d = mux2(line->tag[3].q, line->tag_in[3], we);
    line->tag[4].d = mux2(line->tag[4].q, line->tag_in[4], we);
    line->tag[5].d = mux2(line->tag[5].q, line->tag_in[5], we);
    line->tag[6].d = mux2(line->tag[6].q, line->tag_in[6], we);
    line->tag[7].d = mux2(line->tag[7].q, line->tag_in[7], we);
    line->tag[8].d = mux2(line->tag[8].q, line->tag_in[8], we);
    line->tag[9].d = mux2(line->tag[9].q, line->tag_in[9], we);
    line->tag[10].d = mux2(line->tag[10].q, line->tag_in[10], we);
    line->tag[11].d = mux2(line->tag[11].q, line->tag_in[11], we);
    line->tag[12].d = mux2(line->tag[12].q, line->tag_in[12], we);
    line->tag[13].d = mux2(line->tag[13].q, line->tag_in[13], we);
    line->tag[14].d = mux2(line->tag[14].q, line->tag_in[14], we);
    line->tag[15].d = mux2(line->tag[15].q, line->tag_in[15], we);
    line->tag[16].d = mux2(line->tag[16].q, line->tag_in[16], we);
    line->tag[17].d = mux2(line->tag[17].q, line->tag_in[17], we);
    line->tag[18].d = mux2(line->tag[18].q, line->tag_in[18], we);
    line->tag[19].d = mux2(line->tag[19].q, line->tag_in[19], we);

    line->data[0].d = mux2(line->data[0].q, line->data_in[0], we);
    line->data[1].d = mux2(line->data[1].q, line->data_in[1], we);
    line->data[2].d = mux2(line->data[2].q, line->data_in[2], we);
    line->data[3].d = mux2(line->data[3].q, line->data_in[3], we);
    line->data[4].d = mux2(line->data[4].q, line->data_in[4], we);
    line->data[5].d = mux2(line->data[5].q, line->data_in[5], we);
    line->data[6].d = mux2(line->data[6].q, line->data_in[6], we);
    line->data[7].d = mux2(line->data[7].q, line->data_in[7], we);
    line->data[8].d = mux2(line->data[8].q, line->data_in[8], we);
    line->data[9].d = mux2(line->data[9].q, line->data_in[9], we);
    line->data[10].d = mux2(line->data[10].q, line->data_in[10], we);
    line->data[11].d = mux2(line->data[11].q, line->data_in[11], we);
    line->data[12].d = mux2(line->data[12].q, line->data_in[12], we);
    line->data[13].d = mux2(line->data[13].q, line->data_in[13], we);
    line->data[14].d = mux2(line->data[14].q, line->data_in[14], we);
    line->data[15].d = mux2(line->data[15].q, line->data_in[15], we);
    line->data[16].d = mux2(line->data[16].q, line->data_in[16], we);
    line->data[17].d = mux2(line->data[17].q, line->data_in[17], we);
    line->data[18].d = mux2(line->data[18].q, line->data_in[18], we);
    line->data[19].d = mux2(line->data[19].q, line->data_in[19], we);
    line->data[20].d = mux2(line->data[20].q, line->data_in[20], we);
    line->data[21].d = mux2(line->data[21].q, line->data_in[21], we);
    line->data[22].d = mux2(line->data[22].q, line->data_in[22], we);
    line->data[23].d = mux2(line->data[23].q, line->data_in[23], we);
    line->data[24].d = mux2(line->data[24].q, line->data_in[24], we);
    line->data[25].d = mux2(line->data[25].q, line->data_in[25], we);
    line->data[26].d = mux2(line->data[26].q, line->data_in[26], we);
    line->data[27].d = mux2(line->data[27].q, line->data_in[27], we);
    line->data[28].d = mux2(line->data[28].q, line->data_in[28], we);
    line->data[29].d = mux2(line->data[29].q, line->data_in[29], we);
    line->data[30].d = mux2(line->data[30].q, line->data_in[30], we);
    line->data[31].d = mux2(line->data[31].q, line->data_in[31], we);

    dff_clock_fall(&line->valid);
    dff_clock_fall(&line->dirty);

    int i;
    i = 0; dff_clock_fall(&line->tag[i]); i = 1; dff_clock_fall(&line->tag[i]);
    i = 2; dff_clock_fall(&line->tag[i]); i = 3; dff_clock_fall(&line->tag[i]);
    i = 4; dff_clock_fall(&line->tag[i]); i = 5; dff_clock_fall(&line->tag[i]);
    i = 6; dff_clock_fall(&line->tag[i]); i = 7; dff_clock_fall(&line->tag[i]);
    i = 8; dff_clock_fall(&line->tag[i]); i = 9; dff_clock_fall(&line->tag[i]);
    i = 10; dff_clock_fall(&line->tag[i]); i = 11; dff_clock_fall(&line->tag[i]);
    i = 12; dff_clock_fall(&line->tag[i]); i = 13; dff_clock_fall(&line->tag[i]);
    i = 14; dff_clock_fall(&line->tag[i]); i = 15; dff_clock_fall(&line->tag[i]);
    i = 16; dff_clock_fall(&line->tag[i]); i = 17; dff_clock_fall(&line->tag[i]);
    i = 18; dff_clock_fall(&line->tag[i]); i = 19; dff_clock_fall(&line->tag[i]);

    i = 0; dff_clock_fall(&line->data[i]); i = 1; dff_clock_fall(&line->data[i]);
    i = 2; dff_clock_fall(&line->data[i]); i = 3; dff_clock_fall(&line->data[i]);
    i = 4; dff_clock_fall(&line->data[i]); i = 5; dff_clock_fall(&line->data[i]);
    i = 6; dff_clock_fall(&line->data[i]); i = 7; dff_clock_fall(&line->data[i]);
    i = 8; dff_clock_fall(&line->data[i]); i = 9; dff_clock_fall(&line->data[i]);
    i = 10; dff_clock_fall(&line->data[i]); i = 11; dff_clock_fall(&line->data[i]);
    i = 12; dff_clock_fall(&line->data[i]); i = 13; dff_clock_fall(&line->data[i]);
    i = 14; dff_clock_fall(&line->data[i]); i = 15; dff_clock_fall(&line->data[i]);
    i = 16; dff_clock_fall(&line->data[i]); i = 17; dff_clock_fall(&line->data[i]);
    i = 18; dff_clock_fall(&line->data[i]); i = 19; dff_clock_fall(&line->data[i]);
    i = 20; dff_clock_fall(&line->data[i]); i = 21; dff_clock_fall(&line->data[i]);
    i = 22; dff_clock_fall(&line->data[i]); i = 23; dff_clock_fall(&line->data[i]);
    i = 24; dff_clock_fall(&line->data[i]); i = 25; dff_clock_fall(&line->data[i]);
    i = 26; dff_clock_fall(&line->data[i]); i = 27; dff_clock_fall(&line->data[i]);
    i = 28; dff_clock_fall(&line->data[i]); i = 29; dff_clock_fall(&line->data[i]);
    i = 30; dff_clock_fall(&line->data[i]); i = 31; dff_clock_fall(&line->data[i]);
}

bit cache_line_get_valid(cache_line_t *line) {
    return line->valid.q;
}

bit cache_line_get_dirty(cache_line_t *line) {
    return line->dirty.q;
}

void cache_line_get_tag(cache_line_t *line, bit *out) {
    out[0] = line->tag[0].q; out[1] = line->tag[1].q;
    out[2] = line->tag[2].q; out[3] = line->tag[3].q;
    out[4] = line->tag[4].q; out[5] = line->tag[5].q;
    out[6] = line->tag[6].q; out[7] = line->tag[7].q;
    out[8] = line->tag[8].q; out[9] = line->tag[9].q;
    out[10] = line->tag[10].q; out[11] = line->tag[11].q;
    out[12] = line->tag[12].q; out[13] = line->tag[13].q;
    out[14] = line->tag[14].q; out[15] = line->tag[15].q;
    out[16] = line->tag[16].q; out[17] = line->tag[17].q;
    out[18] = line->tag[18].q; out[19] = line->tag[19].q;
}

void cache_line_get_data(cache_line_t *line, bit *out) {
    out[0] = line->data[0].q; out[1] = line->data[1].q;
    out[2] = line->data[2].q; out[3] = line->data[3].q;
    out[4] = line->data[4].q; out[5] = line->data[5].q;
    out[6] = line->data[6].q; out[7] = line->data[7].q;
    out[8] = line->data[8].q; out[9] = line->data[9].q;
    out[10] = line->data[10].q; out[11] = line->data[11].q;
    out[12] = line->data[12].q; out[13] = line->data[13].q;
    out[14] = line->data[14].q; out[15] = line->data[15].q;
    out[16] = line->data[16].q; out[17] = line->data[17].q;
    out[18] = line->data[18].q; out[19] = line->data[19].q;
    out[20] = line->data[20].q; out[21] = line->data[21].q;
    out[22] = line->data[22].q; out[23] = line->data[23].q;
    out[24] = line->data[24].q; out[25] = line->data[25].q;
    out[26] = line->data[26].q; out[27] = line->data[27].q;
    out[28] = line->data[28].q; out[29] = line->data[29].q;
    out[30] = line->data[30].q; out[31] = line->data[31].q;
}

bit cache_line_tag_match(cache_line_t *line, bit *tag) {
    bit eq[20];
    eq[0] = XNOR(line->tag[0].q, tag[0]);
    eq[1] = XNOR(line->tag[1].q, tag[1]);
    eq[2] = XNOR(line->tag[2].q, tag[2]);
    eq[3] = XNOR(line->tag[3].q, tag[3]);
    eq[4] = XNOR(line->tag[4].q, tag[4]);
    eq[5] = XNOR(line->tag[5].q, tag[5]);
    eq[6] = XNOR(line->tag[6].q, tag[6]);
    eq[7] = XNOR(line->tag[7].q, tag[7]);
    eq[8] = XNOR(line->tag[8].q, tag[8]);
    eq[9] = XNOR(line->tag[9].q, tag[9]);
    eq[10] = XNOR(line->tag[10].q, tag[10]);
    eq[11] = XNOR(line->tag[11].q, tag[11]);
    eq[12] = XNOR(line->tag[12].q, tag[12]);
    eq[13] = XNOR(line->tag[13].q, tag[13]);
    eq[14] = XNOR(line->tag[14].q, tag[14]);
    eq[15] = XNOR(line->tag[15].q, tag[15]);
    eq[16] = XNOR(line->tag[16].q, tag[16]);
    eq[17] = XNOR(line->tag[17].q, tag[17]);
    eq[18] = XNOR(line->tag[18].q, tag[18]);
    eq[19] = XNOR(line->tag[19].q, tag[19]);

    bit t0 = AND(AND(AND(eq[0], eq[1]), AND(eq[2], eq[3])), AND(AND(eq[4], eq[5]), AND(eq[6], eq[7])));
    bit t1 = AND(AND(AND(eq[8], eq[9]), AND(eq[10], eq[11])), AND(AND(eq[12], eq[13]), AND(eq[14], eq[15])));
    bit t2 = AND(AND(eq[16], eq[17]), AND(eq[18], eq[19]));

    return AND(AND(t0, t1), t2);
}
