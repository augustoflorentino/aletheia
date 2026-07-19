#include "l2_cache.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

static void cpy32(bit *s, bit *d) {
    d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3];
    d[4] = s[4]; d[5] = s[5]; d[6] = s[6]; d[7] = s[7];
    d[8] = s[8]; d[9] = s[9]; d[10] = s[10]; d[11] = s[11];
    d[12] = s[12]; d[13] = s[13]; d[14] = s[14]; d[15] = s[15];
    d[16] = s[16]; d[17] = s[17]; d[18] = s[18]; d[19] = s[19];
    d[20] = s[20]; d[21] = s[21]; d[22] = s[22]; d[23] = s[23];
    d[24] = s[24]; d[25] = s[25]; d[26] = s[26]; d[27] = s[27];
    d[28] = s[28]; d[29] = s[29]; d[30] = s[30]; d[31] = s[31];
}

static void extract_tag(bit *addr, bit *tag) {
    tag[0] = addr[3];
    tag[1] = addr[4];
    tag[2] = addr[5];
    tag[3] = addr[6];
    tag[4] = addr[7];
    tag[5] = addr[8];
    tag[6] = addr[9];
    tag[7] = addr[10];
    tag[8] = addr[11];
    tag[9] = addr[12];
    tag[10] = addr[13];
    tag[11] = addr[14];
    tag[12] = addr[15];
    tag[13] = addr[16];
    tag[14] = addr[17];
    tag[15] = addr[18];
    tag[16] = addr[19];
    tag[17] = addr[20];
    tag[18] = addr[21];
    tag[19] = addr[22];
}

void l2_init(l2_cache_t *c) {
    cache_line_init(&c->lines[0]);
    cache_line_init(&c->lines[1]);
    cache_line_init(&c->lines[2]);
    cache_line_init(&c->lines[3]);
    cache_line_init(&c->lines[4]);
    cache_line_init(&c->lines[5]);
    cache_line_init(&c->lines[6]);
    cache_line_init(&c->lines[7]);
    c->write = 0;
}

void l2_reset(l2_cache_t *c) {
    cache_line_reset(&c->lines[0]);
    cache_line_reset(&c->lines[1]);
    cache_line_reset(&c->lines[2]);
    cache_line_reset(&c->lines[3]);
    cache_line_reset(&c->lines[4]);
    cache_line_reset(&c->lines[5]);
    cache_line_reset(&c->lines[6]);
    cache_line_reset(&c->lines[7]);
    c->write = 0;
}

void l2_set_addr(l2_cache_t *c, bit *addr) { cpy32(addr, c->addr); }
void l2_set_data(l2_cache_t *c, bit *data) { cpy32(data, c->data_in); }
void l2_set_write(l2_cache_t *c, bit write) { c->write = write; }

void l2_clock_fall(l2_cache_t *c) {
    bit tag[20]; extract_tag(c->addr, tag);
    bit sel[8];
    decoder_3to8(c->addr[0], c->addr[1], c->addr[2], sel);
    bit w0 = AND(c->write, sel[0]);
    cache_line_set_write(&c->lines[0], w0);
    cache_line_set_tag(&c->lines[0], tag);
    cache_line_set_data(&c->lines[0], c->data_in);
    cache_line_set_valid(&c->lines[0], w0);
    cache_line_set_dirty(&c->lines[0], 0, 0);
    bit w1 = AND(c->write, sel[1]);
    cache_line_set_write(&c->lines[1], w1);
    cache_line_set_tag(&c->lines[1], tag);
    cache_line_set_data(&c->lines[1], c->data_in);
    cache_line_set_valid(&c->lines[1], w1);
    cache_line_set_dirty(&c->lines[1], 0, 0);
    bit w2 = AND(c->write, sel[2]);
    cache_line_set_write(&c->lines[2], w2);
    cache_line_set_tag(&c->lines[2], tag);
    cache_line_set_data(&c->lines[2], c->data_in);
    cache_line_set_valid(&c->lines[2], w2);
    cache_line_set_dirty(&c->lines[2], 0, 0);
    bit w3 = AND(c->write, sel[3]);
    cache_line_set_write(&c->lines[3], w3);
    cache_line_set_tag(&c->lines[3], tag);
    cache_line_set_data(&c->lines[3], c->data_in);
    cache_line_set_valid(&c->lines[3], w3);
    cache_line_set_dirty(&c->lines[3], 0, 0);
    bit w4 = AND(c->write, sel[4]);
    cache_line_set_write(&c->lines[4], w4);
    cache_line_set_tag(&c->lines[4], tag);
    cache_line_set_data(&c->lines[4], c->data_in);
    cache_line_set_valid(&c->lines[4], w4);
    cache_line_set_dirty(&c->lines[4], 0, 0);
    bit w5 = AND(c->write, sel[5]);
    cache_line_set_write(&c->lines[5], w5);
    cache_line_set_tag(&c->lines[5], tag);
    cache_line_set_data(&c->lines[5], c->data_in);
    cache_line_set_valid(&c->lines[5], w5);
    cache_line_set_dirty(&c->lines[5], 0, 0);
    bit w6 = AND(c->write, sel[6]);
    cache_line_set_write(&c->lines[6], w6);
    cache_line_set_tag(&c->lines[6], tag);
    cache_line_set_data(&c->lines[6], c->data_in);
    cache_line_set_valid(&c->lines[6], w6);
    cache_line_set_dirty(&c->lines[6], 0, 0);
    bit w7 = AND(c->write, sel[7]);
    cache_line_set_write(&c->lines[7], w7);
    cache_line_set_tag(&c->lines[7], tag);
    cache_line_set_data(&c->lines[7], c->data_in);
    cache_line_set_valid(&c->lines[7], w7);
    cache_line_set_dirty(&c->lines[7], 0, 0);
    cache_line_clock_fall(&c->lines[0]);
    cache_line_clock_fall(&c->lines[1]);
    cache_line_clock_fall(&c->lines[2]);
    cache_line_clock_fall(&c->lines[3]);
    cache_line_clock_fall(&c->lines[4]);
    cache_line_clock_fall(&c->lines[5]);
    cache_line_clock_fall(&c->lines[6]);
    cache_line_clock_fall(&c->lines[7]);
}

void l2_clock_rise(l2_cache_t *c) {
    cache_line_clock_rise(&c->lines[0]);
    cache_line_clock_rise(&c->lines[1]);
    cache_line_clock_rise(&c->lines[2]);
    cache_line_clock_rise(&c->lines[3]);
    cache_line_clock_rise(&c->lines[4]);
    cache_line_clock_rise(&c->lines[5]);
    cache_line_clock_rise(&c->lines[6]);
    cache_line_clock_rise(&c->lines[7]);
}

void l2_propagate(l2_cache_t *c) {
    bit tag[20]; extract_tag(c->addr, tag);
    bit m0 = cache_line_tag_match(&c->lines[0], tag);
    bit h0 = AND(cache_line_get_valid(&c->lines[0]), m0);
    bit m1 = cache_line_tag_match(&c->lines[1], tag);
    bit h1 = AND(cache_line_get_valid(&c->lines[1]), m1);
    bit m2 = cache_line_tag_match(&c->lines[2], tag);
    bit h2 = AND(cache_line_get_valid(&c->lines[2]), m2);
    bit m3 = cache_line_tag_match(&c->lines[3], tag);
    bit h3 = AND(cache_line_get_valid(&c->lines[3]), m3);
    bit m4 = cache_line_tag_match(&c->lines[4], tag);
    bit h4 = AND(cache_line_get_valid(&c->lines[4]), m4);
    bit m5 = cache_line_tag_match(&c->lines[5], tag);
    bit h5 = AND(cache_line_get_valid(&c->lines[5]), m5);
    bit m6 = cache_line_tag_match(&c->lines[6], tag);
    bit h6 = AND(cache_line_get_valid(&c->lines[6]), m6);
    bit m7 = cache_line_tag_match(&c->lines[7], tag);
    bit h7 = AND(cache_line_get_valid(&c->lines[7]), m7);
    bit hg0 = mux4(h0, h1, h2, h3, c->addr[0], c->addr[1]);
    bit hg1 = mux4(h4, h5, h6, h7, c->addr[0], c->addr[1]);
    c->hit = mux2(hg0, hg1, c->addr[2]);
    bit d0[32]; cache_line_get_data(&c->lines[0], d0);
    bit d1[32]; cache_line_get_data(&c->lines[1], d1);
    bit d2[32]; cache_line_get_data(&c->lines[2], d2);
    bit d3[32]; cache_line_get_data(&c->lines[3], d3);
    bit d4[32]; cache_line_get_data(&c->lines[4], d4);
    bit d5[32]; cache_line_get_data(&c->lines[5], d5);
    bit d6[32]; cache_line_get_data(&c->lines[6], d6);
    bit d7[32]; cache_line_get_data(&c->lines[7], d7);
    bit g0[32], g1[32];
    mux4_32(d0, d1, d2, d3, c->addr[0], c->addr[1], g0);
    mux4_32(d4, d5, d6, d7, c->addr[0], c->addr[1], g1);
    mux2_32(g0, g1, c->addr[2], c->data_out);
}
