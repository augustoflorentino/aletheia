#ifndef ALETHEIA_PAGE_TABLE_H
#define ALETHEIA_PAGE_TABLE_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"

#define PT_ENTRIES 16

typedef struct {
    d_flip_flop_t pfn[PT_ENTRIES][20];
    d_flip_flop_t present[PT_ENTRIES];

    bit vpn[20];
    bit pfn_in[20];
    bit write;

    bit present_out;
    bit pfn_out[20];
} page_table_t;

void page_table_init(page_table_t *pt);
void page_table_reset(page_table_t *pt);
void page_table_set_vpn(page_table_t *pt, bit *vpn);
void page_table_set_pfn(page_table_t *pt, bit *pfn);
void page_table_set_write(page_table_t *pt, bit write);
void page_table_clock_fall(page_table_t *pt);
void page_table_clock_rise(page_table_t *pt);
void page_table_propagate(page_table_t *pt);

#endif
