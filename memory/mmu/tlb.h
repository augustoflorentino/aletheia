#ifndef ALETHEIA_TLB_H
#define ALETHEIA_TLB_H

#include "../../core/types.h"
#include "../cache/cache_line.h"

#define TLB_ENTRIES 4

typedef struct {
    cache_line_t entries[TLB_ENTRIES];

    bit vpn[20];
    bit pfn_in[20];
    bit write;

    bit hit;
    bit pfn_out[20];
} tlb_t;

void tlb_init(tlb_t *t);
void tlb_reset(tlb_t *t);
void tlb_set_vpn(tlb_t *t, bit *vpn);
void tlb_set_pfn(tlb_t *t, bit *pfn);
void tlb_set_write(tlb_t *t, bit write);
void tlb_clock_fall(tlb_t *t);
void tlb_clock_rise(tlb_t *t);
void tlb_propagate(tlb_t *t);

#endif
