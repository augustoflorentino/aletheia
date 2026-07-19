#ifndef ALETHEIA_MMU_H
#define ALETHEIA_MMU_H

#include "../../core/types.h"
#include "tlb.h"
#include "page_table.h"

typedef struct {
    tlb_t tlb;
    page_table_t pt;

    bit vaddr[32];
    bit fill_pfn[20];
    bit fill;
    bit pt_fill_pfn[20];
    bit pt_fill;

    bit paddr[32];
    bit fault;
    bit translated;
    bit tlb_hit;
} mmu_t;

void mmu_init(mmu_t *m);
void mmu_reset(mmu_t *m);
void mmu_set_vaddr(mmu_t *m, bit *vaddr);
void mmu_set_fill_pfn(mmu_t *m, bit *pfn);
void mmu_set_fill(mmu_t *m, bit fill);
void mmu_set_pt_pfn(mmu_t *m, bit *pfn);
void mmu_set_pt_fill(mmu_t *m, bit fill);
void mmu_clock_fall(mmu_t *m);
void mmu_clock_rise(mmu_t *m);
void mmu_propagate(mmu_t *m);

#endif
