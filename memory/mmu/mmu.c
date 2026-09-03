#include "mmu.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

static void extract_vpn(bit *vaddr, bit *vpn) {
    vpn[0] = vaddr[4]; vpn[1] = vaddr[5]; vpn[2] = vaddr[6]; vpn[3] = vaddr[7];
    vpn[4] = vaddr[8]; vpn[5] = vaddr[9]; vpn[6] = vaddr[10]; vpn[7] = vaddr[11];
    vpn[8] = vaddr[12]; vpn[9] = vaddr[13]; vpn[10] = vaddr[14]; vpn[11] = vaddr[15];
    vpn[12] = vaddr[16]; vpn[13] = vaddr[17]; vpn[14] = vaddr[18]; vpn[15] = vaddr[19];
    vpn[16] = vaddr[20]; vpn[17] = vaddr[21]; vpn[18] = vaddr[22]; vpn[19] = vaddr[23];
}

void mmu_init(mmu_t *m) {
    tlb_init(&m->tlb);
    page_table_init(&m->pt);
    m->fill = 0;
    m->pt_fill = 0;
    m->pt_valid = 0;
}

void mmu_reset(mmu_t *m) {
    tlb_reset(&m->tlb);
    page_table_reset(&m->pt);
    m->fill = 0;
    m->pt_fill = 0;
    m->pt_valid = 0;
}

void mmu_set_vaddr(mmu_t *m, bit *vaddr) {
    m->vaddr[0] = vaddr[0]; m->vaddr[1] = vaddr[1]; m->vaddr[2] = vaddr[2];
    m->vaddr[3] = vaddr[3]; m->vaddr[4] = vaddr[4]; m->vaddr[5] = vaddr[5];
    m->vaddr[6] = vaddr[6]; m->vaddr[7] = vaddr[7]; m->vaddr[8] = vaddr[8];
    m->vaddr[9] = vaddr[9]; m->vaddr[10] = vaddr[10]; m->vaddr[11] = vaddr[11];
    m->vaddr[12] = vaddr[12]; m->vaddr[13] = vaddr[13]; m->vaddr[14] = vaddr[14];
    m->vaddr[15] = vaddr[15]; m->vaddr[16] = vaddr[16]; m->vaddr[17] = vaddr[17];
    m->vaddr[18] = vaddr[18]; m->vaddr[19] = vaddr[19]; m->vaddr[20] = vaddr[20];
    m->vaddr[21] = vaddr[21]; m->vaddr[22] = vaddr[22]; m->vaddr[23] = vaddr[23];
    m->vaddr[24] = vaddr[24]; m->vaddr[25] = vaddr[25]; m->vaddr[26] = vaddr[26];
    m->vaddr[27] = vaddr[27]; m->vaddr[28] = vaddr[28]; m->vaddr[29] = vaddr[29];
    m->vaddr[30] = vaddr[30]; m->vaddr[31] = vaddr[31];
}

void mmu_set_fill_pfn(mmu_t *m, bit *pfn) {
    m->fill_pfn[0] = pfn[0]; m->fill_pfn[1] = pfn[1]; m->fill_pfn[2] = pfn[2];
    m->fill_pfn[3] = pfn[3]; m->fill_pfn[4] = pfn[4]; m->fill_pfn[5] = pfn[5];
    m->fill_pfn[6] = pfn[6]; m->fill_pfn[7] = pfn[7]; m->fill_pfn[8] = pfn[8];
    m->fill_pfn[9] = pfn[9]; m->fill_pfn[10] = pfn[10]; m->fill_pfn[11] = pfn[11];
    m->fill_pfn[12] = pfn[12]; m->fill_pfn[13] = pfn[13]; m->fill_pfn[14] = pfn[14];
    m->fill_pfn[15] = pfn[15]; m->fill_pfn[16] = pfn[16]; m->fill_pfn[17] = pfn[17];
    m->fill_pfn[18] = pfn[18]; m->fill_pfn[19] = pfn[19];
}

void mmu_set_fill(mmu_t *m, bit fill) {
    m->fill = fill;
}

void mmu_set_pt_pfn(mmu_t *m, bit *pfn) {
    m->pt_fill_pfn[0] = pfn[0]; m->pt_fill_pfn[1] = pfn[1];
    m->pt_fill_pfn[2] = pfn[2]; m->pt_fill_pfn[3] = pfn[3];
    m->pt_fill_pfn[4] = pfn[4]; m->pt_fill_pfn[5] = pfn[5];
    m->pt_fill_pfn[6] = pfn[6]; m->pt_fill_pfn[7] = pfn[7];
    m->pt_fill_pfn[8] = pfn[8]; m->pt_fill_pfn[9] = pfn[9];
    m->pt_fill_pfn[10] = pfn[10]; m->pt_fill_pfn[11] = pfn[11];
    m->pt_fill_pfn[12] = pfn[12]; m->pt_fill_pfn[13] = pfn[13];
    m->pt_fill_pfn[14] = pfn[14]; m->pt_fill_pfn[15] = pfn[15];
    m->pt_fill_pfn[16] = pfn[16]; m->pt_fill_pfn[17] = pfn[17];
    m->pt_fill_pfn[18] = pfn[18]; m->pt_fill_pfn[19] = pfn[19];
}

void mmu_set_pt_fill(mmu_t *m, bit fill) {
    m->pt_fill = fill;
}

void mmu_set_pt_valid(mmu_t *m, bit valid) {
    m->pt_valid = valid;
}

void mmu_clock_fall(mmu_t *m) {
    bit vpn[20];
    extract_vpn(m->vaddr, vpn);
    tlb_set_vpn(&m->tlb, vpn);
    tlb_set_pfn(&m->tlb, m->fill_pfn);
    tlb_set_write(&m->tlb, m->fill);
    tlb_set_flush(&m->tlb, m->pt_fill);
    tlb_clock_fall(&m->tlb);

    page_table_set_vpn(&m->pt, vpn);
    page_table_set_pfn(&m->pt, m->pt_fill_pfn);
    page_table_set_write(&m->pt, m->pt_fill);
    page_table_set_present(&m->pt, m->pt_valid);
    page_table_clock_fall(&m->pt);
}

void mmu_clock_rise(mmu_t *m) {
    tlb_clock_rise(&m->tlb);
    page_table_clock_rise(&m->pt);
}

void mmu_propagate(mmu_t *m) {
    bit vpn[20];
    extract_vpn(m->vaddr, vpn);

    tlb_set_vpn(&m->tlb, vpn);
    tlb_propagate(&m->tlb);
    page_table_set_vpn(&m->pt, vpn);
    page_table_propagate(&m->pt);

    bit hit = m->tlb.hit;

    bit pfn[20];
    pfn[0] = mux2(m->pt.pfn_out[0], m->tlb.pfn_out[0], hit);
    pfn[1] = mux2(m->pt.pfn_out[1], m->tlb.pfn_out[1], hit);
    pfn[2] = mux2(m->pt.pfn_out[2], m->tlb.pfn_out[2], hit);
    pfn[3] = mux2(m->pt.pfn_out[3], m->tlb.pfn_out[3], hit);
    pfn[4] = mux2(m->pt.pfn_out[4], m->tlb.pfn_out[4], hit);
    pfn[5] = mux2(m->pt.pfn_out[5], m->tlb.pfn_out[5], hit);
    pfn[6] = mux2(m->pt.pfn_out[6], m->tlb.pfn_out[6], hit);
    pfn[7] = mux2(m->pt.pfn_out[7], m->tlb.pfn_out[7], hit);
    pfn[8] = mux2(m->pt.pfn_out[8], m->tlb.pfn_out[8], hit);
    pfn[9] = mux2(m->pt.pfn_out[9], m->tlb.pfn_out[9], hit);
    pfn[10] = mux2(m->pt.pfn_out[10], m->tlb.pfn_out[10], hit);
    pfn[11] = mux2(m->pt.pfn_out[11], m->tlb.pfn_out[11], hit);
    pfn[12] = mux2(m->pt.pfn_out[12], m->tlb.pfn_out[12], hit);
    pfn[13] = mux2(m->pt.pfn_out[13], m->tlb.pfn_out[13], hit);
    pfn[14] = mux2(m->pt.pfn_out[14], m->tlb.pfn_out[14], hit);
    pfn[15] = mux2(m->pt.pfn_out[15], m->tlb.pfn_out[15], hit);
    pfn[16] = mux2(m->pt.pfn_out[16], m->tlb.pfn_out[16], hit);
    pfn[17] = mux2(m->pt.pfn_out[17], m->tlb.pfn_out[17], hit);
    pfn[18] = mux2(m->pt.pfn_out[18], m->tlb.pfn_out[18], hit);
    pfn[19] = mux2(m->pt.pfn_out[19], m->tlb.pfn_out[19], hit);

    m->paddr[0] = m->vaddr[0]; m->paddr[1] = m->vaddr[1];
    m->paddr[2] = m->vaddr[2]; m->paddr[3] = m->vaddr[3];
    m->paddr[4] = pfn[0]; m->paddr[5] = pfn[1]; m->paddr[6] = pfn[2]; m->paddr[7] = pfn[3];
    m->paddr[8] = pfn[4]; m->paddr[9] = pfn[5]; m->paddr[10] = pfn[6]; m->paddr[11] = pfn[7];
    m->paddr[12] = pfn[8]; m->paddr[13] = pfn[9]; m->paddr[14] = pfn[10]; m->paddr[15] = pfn[11];
    m->paddr[16] = pfn[12]; m->paddr[17] = pfn[13]; m->paddr[18] = pfn[14]; m->paddr[19] = pfn[15];
    m->paddr[20] = pfn[16]; m->paddr[21] = pfn[17]; m->paddr[22] = pfn[18]; m->paddr[23] = pfn[19];
    m->paddr[24] = 0; m->paddr[25] = 0; m->paddr[26] = 0; m->paddr[27] = 0;
    m->paddr[28] = 0; m->paddr[29] = 0; m->paddr[30] = 0; m->paddr[31] = 0;
    bit top_zero = NOT(OR(OR(OR(m->vaddr[24], m->vaddr[25]), OR(m->vaddr[26], m->vaddr[27])),
                          OR(OR(m->vaddr[28], m->vaddr[29]), OR(m->vaddr[30], m->vaddr[31]))));
    bit vpn_in_table = NOT(OR(OR(OR(OR(vpn[4], vpn[5]), OR(vpn[6], vpn[7])),
                                 OR(OR(vpn[8], vpn[9]), OR(vpn[10], vpn[11]))),
                              OR(OR(OR(vpn[12], vpn[13]), OR(vpn[14], vpn[15])),
                                 OR(OR(vpn[16], vpn[17]), OR(vpn[18], vpn[19])))));
    m->pt_hit = AND(AND(m->pt.present_out, vpn_in_table), top_zero);
    m->tlb_hit = AND(hit, top_zero);
    m->translated = OR(m->tlb_hit, m->pt_hit);
    m->fault = NOT(m->translated);
}
