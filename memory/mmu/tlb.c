#include "tlb.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

static void vpn_tag(bit *vpn, bit *tag) {
    tag[0] = vpn[2]; tag[1] = vpn[3]; tag[2] = vpn[4]; tag[3] = vpn[5];
    tag[4] = vpn[6]; tag[5] = vpn[7]; tag[6] = vpn[8]; tag[7] = vpn[9];
    tag[8] = vpn[10]; tag[9] = vpn[11]; tag[10] = vpn[12]; tag[11] = vpn[13];
    tag[12] = vpn[14]; tag[13] = vpn[15]; tag[14] = vpn[16]; tag[15] = vpn[17];
    tag[16] = vpn[18]; tag[17] = vpn[19]; tag[18] = 0; tag[19] = 0;
}

static void pfn_to_data(bit *pfn, bit *data) {
    data[0] = pfn[0]; data[1] = pfn[1]; data[2] = pfn[2]; data[3] = pfn[3];
    data[4] = pfn[4]; data[5] = pfn[5]; data[6] = pfn[6]; data[7] = pfn[7];
    data[8] = pfn[8]; data[9] = pfn[9]; data[10] = pfn[10]; data[11] = pfn[11];
    data[12] = pfn[12]; data[13] = pfn[13]; data[14] = pfn[14]; data[15] = pfn[15];
    data[16] = pfn[16]; data[17] = pfn[17]; data[18] = pfn[18]; data[19] = pfn[19];
    data[20] = 0; data[21] = 0; data[22] = 0; data[23] = 0;
    data[24] = 0; data[25] = 0; data[26] = 0; data[27] = 0;
    data[28] = 0; data[29] = 0; data[30] = 0; data[31] = 0;
}

static void cpy20(bit *s, bit *d) {
    d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3];
    d[4] = s[4]; d[5] = s[5]; d[6] = s[6]; d[7] = s[7];
    d[8] = s[8]; d[9] = s[9]; d[10] = s[10]; d[11] = s[11];
    d[12] = s[12]; d[13] = s[13]; d[14] = s[14]; d[15] = s[15];
    d[16] = s[16]; d[17] = s[17]; d[18] = s[18]; d[19] = s[19];
}

void tlb_init(tlb_t *t) {
    cache_line_init(&t->entries[0]);
    cache_line_init(&t->entries[1]);
    cache_line_init(&t->entries[2]);
    cache_line_init(&t->entries[3]);
    t->write = 0;
    t->flush = 0;
}

void tlb_reset(tlb_t *t) {
    cache_line_reset(&t->entries[0]);
    cache_line_reset(&t->entries[1]);
    cache_line_reset(&t->entries[2]);
    cache_line_reset(&t->entries[3]);
    t->write = 0;
    t->flush = 0;
}

void tlb_set_vpn(tlb_t *t, bit *vpn) {
    cpy20(vpn, t->vpn);
}

void tlb_set_pfn(tlb_t *t, bit *pfn) {
    cpy20(pfn, t->pfn_in);
}

void tlb_set_write(tlb_t *t, bit write) {
    t->write = write;
}

void tlb_set_flush(tlb_t *t, bit flush) {
    t->flush = flush;
}

void tlb_clock_fall(tlb_t *t) {
    bit tag[20];
    vpn_tag(t->vpn, tag);
    bit data[32];
    pfn_to_data(t->pfn_in, data);

    bit sel[4];
    decoder_2to4(t->vpn[0], t->vpn[1], sel);

    bit w0 = AND(t->write, sel[0]);
    bit w1 = AND(t->write, sel[1]);
    bit w2 = AND(t->write, sel[2]);
    bit w3 = AND(t->write, sel[3]);

    cache_line_set_write(&t->entries[0], w0);
    cache_line_set_tag(&t->entries[0], tag);
    cache_line_set_data(&t->entries[0], data);
    cache_line_set_valid(&t->entries[0], w0);
    cache_line_set_invalidate(&t->entries[0], t->flush);
    cache_line_set_dirty(&t->entries[0], 0, 0);

    cache_line_set_write(&t->entries[1], w1);
    cache_line_set_tag(&t->entries[1], tag);
    cache_line_set_data(&t->entries[1], data);
    cache_line_set_valid(&t->entries[1], w1);
    cache_line_set_invalidate(&t->entries[1], t->flush);
    cache_line_set_dirty(&t->entries[1], 0, 0);

    cache_line_set_write(&t->entries[2], w2);
    cache_line_set_tag(&t->entries[2], tag);
    cache_line_set_data(&t->entries[2], data);
    cache_line_set_valid(&t->entries[2], w2);
    cache_line_set_invalidate(&t->entries[2], t->flush);
    cache_line_set_dirty(&t->entries[2], 0, 0);

    cache_line_set_write(&t->entries[3], w3);
    cache_line_set_tag(&t->entries[3], tag);
    cache_line_set_data(&t->entries[3], data);
    cache_line_set_valid(&t->entries[3], w3);
    cache_line_set_invalidate(&t->entries[3], t->flush);
    cache_line_set_dirty(&t->entries[3], 0, 0);

    cache_line_clock_fall(&t->entries[0]);
    cache_line_clock_fall(&t->entries[1]);
    cache_line_clock_fall(&t->entries[2]);
    cache_line_clock_fall(&t->entries[3]);
}

void tlb_clock_rise(tlb_t *t) {
    cache_line_clock_rise(&t->entries[0]);
    cache_line_clock_rise(&t->entries[1]);
    cache_line_clock_rise(&t->entries[2]);
    cache_line_clock_rise(&t->entries[3]);
}

void tlb_propagate(tlb_t *t) {
    bit tag[20];
    vpn_tag(t->vpn, tag);

    bit m0 = cache_line_tag_match(&t->entries[0], tag);
    bit m1 = cache_line_tag_match(&t->entries[1], tag);
    bit m2 = cache_line_tag_match(&t->entries[2], tag);
    bit m3 = cache_line_tag_match(&t->entries[3], tag);

    bit h0 = AND(cache_line_get_valid(&t->entries[0]), m0);
    bit h1 = AND(cache_line_get_valid(&t->entries[1]), m1);
    bit h2 = AND(cache_line_get_valid(&t->entries[2]), m2);
    bit h3 = AND(cache_line_get_valid(&t->entries[3]), m3);

    t->hit = mux4(h0, h1, h2, h3, t->vpn[0], t->vpn[1]);

    bit d0[32], d1[32], d2[32], d3[32];
    cache_line_get_data(&t->entries[0], d0);
    cache_line_get_data(&t->entries[1], d1);
    cache_line_get_data(&t->entries[2], d2);
    cache_line_get_data(&t->entries[3], d3);

    bit sel[32];
    mux4_32(d0, d1, d2, d3, t->vpn[0], t->vpn[1], sel);

    cpy20(sel, t->pfn_out);
}
