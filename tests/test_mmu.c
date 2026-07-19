#include "test.h"

#include "../memory/mmu/mmu.h"

static uint32_t make_vaddr(uint32_t vpn, uint32_t offset) {
    return (vpn << 12) | (offset & 0xFFFu);
}

static void mmu_fill(mmu_t *m, uint32_t vpn, uint32_t pfn) {
    bit va[32], pf[20];
    to_bits_32(make_vaddr(vpn, 0), va);
    to_bits_n(pfn, pf, 20);
    mmu_set_vaddr(m, va);
    mmu_set_fill_pfn(m, pf);
    mmu_set_fill(m, 1);
    mmu_clock_fall(m);
    mmu_clock_rise(m);
    mmu_set_fill(m, 0);
}

static int mmu_xlate(mmu_t *m, uint32_t vaddr, uint32_t *paddr, int *fault) {
    bit va[32];
    to_bits_32(vaddr, va);
    mmu_set_vaddr(m, va);
    mmu_set_fill(m, 0);
    mmu_propagate(m);
    *paddr = from_bits_32(m->paddr);
    *fault = m->fault;
    return m->translated;
}

static void test_fault_before_fill(void) {
    test_suite("mmu: fault on cold TLB");

    mmu_t m;
    mmu_init(&m);
    mmu_reset(&m);

    uint32_t pa;
    int fault;
    int ok = mmu_xlate(&m, make_vaddr(0x5, 0x123), &pa, &fault);
    CHECK_EQ_BIT(ok, 0, "not translated (miss)");
    CHECK_EQ_BIT(fault, 1, "page fault on cold TLB");
}

static void test_translate(void) {
    test_suite("mmu: translate after fill");

    mmu_t m;
    mmu_init(&m);
    mmu_reset(&m);

    mmu_fill(&m, 0x5, 0xABC);
    uint32_t pa;
    int fault;
    int ok = mmu_xlate(&m, make_vaddr(0x5, 0x123), &pa, &fault);
    CHECK_EQ_BIT(ok, 1, "translated after fill");
    CHECK_EQ_BIT(fault, 0, "no fault");
    CHECK_EQ_U32(pa, (0xABCu << 12) | 0x123u, "paddr = pfn:offset");

    ok = mmu_xlate(&m, make_vaddr(0x5, 0x7FF), &pa, &fault);
    CHECK_EQ_U32(pa, (0xABCu << 12) | 0x7FFu, "offset preserved, same page");

    ok = mmu_xlate(&m, make_vaddr(0x9, 0x000), &pa, &fault);
    CHECK_EQ_BIT(ok, 0, "unfilled VPN -> fault");
}

static void test_multiple_pages(void) {
    test_suite("mmu: 4 independent TLB entries");

    mmu_t m;
    mmu_init(&m);
    mmu_reset(&m);

    mmu_fill(&m, 0x0, 0x100);
    mmu_fill(&m, 0x1, 0x200);
    mmu_fill(&m, 0x2, 0x300);
    mmu_fill(&m, 0x3, 0x400);

    uint32_t pa;
    int fault;
    mmu_xlate(&m, make_vaddr(0x0, 0x11), &pa, &fault);
    CHECK_EQ_U32(pa, (0x100u << 12) | 0x11u, "page 0");
    mmu_xlate(&m, make_vaddr(0x1, 0x22), &pa, &fault);
    CHECK_EQ_U32(pa, (0x200u << 12) | 0x22u, "page 1");
    mmu_xlate(&m, make_vaddr(0x2, 0x33), &pa, &fault);
    CHECK_EQ_U32(pa, (0x300u << 12) | 0x33u, "page 2");
    mmu_xlate(&m, make_vaddr(0x3, 0x44), &pa, &fault);
    CHECK_EQ_U32(pa, (0x400u << 12) | 0x44u, "page 3");
}

static void test_tlb_eviction(void) {
    test_suite("mmu: TLB eviction (same index)");

    mmu_t m;
    mmu_init(&m);
    mmu_reset(&m);

    mmu_fill(&m, 0x4, 0xAAA);
    uint32_t pa;
    int fault;
    CHECK(mmu_xlate(&m, make_vaddr(0x4, 0x1), &pa, &fault) && pa == ((0xAAAu << 12) | 0x1u),
          "vpn 0x4 maps");

    mmu_fill(&m, 0x8, 0xBBB);
    CHECK_EQ_BIT(mmu_xlate(&m, make_vaddr(0x4, 0x1), &pa, &fault), 0, "vpn 0x4 evicted");
    CHECK(mmu_xlate(&m, make_vaddr(0x8, 0x1), &pa, &fault) && pa == ((0xBBBu << 12) | 0x1u),
          "vpn 0x8 maps");
}

static void mmu_map_pt(mmu_t *m, uint32_t vpn, uint32_t pfn) {
    bit va[32], pf[20];
    to_bits_32(make_vaddr(vpn, 0), va);
    to_bits_n(pfn, pf, 20);
    mmu_set_vaddr(m, va);
    mmu_set_pt_pfn(m, pf);
    mmu_set_pt_fill(m, 1);
    mmu_clock_fall(m);
    mmu_clock_rise(m);
    mmu_set_pt_fill(m, 0);
}

static void test_page_table_walk(void) {
    test_suite("mmu: page table walk on TLB miss");

    mmu_t m;
    mmu_init(&m);
    mmu_reset(&m);

    mmu_map_pt(&m, 0x6, 0x321);
    uint32_t pa;
    int fault;
    int ok = mmu_xlate(&m, make_vaddr(0x6, 0x0AB), &pa, &fault);
    CHECK_EQ_BIT(ok, 1, "translated via page table (TLB miss)");
    CHECK_EQ_BIT(m.tlb_hit, 0, "TLB missed (slow path)");
    CHECK_EQ_BIT(fault, 0, "no fault when PT has mapping");
    CHECK_EQ_U32(pa, (0x321u << 12) | 0x0ABu, "paddr from page table");

    ok = mmu_xlate(&m, make_vaddr(0x7, 0x0), &pa, &fault);
    CHECK_EQ_BIT(fault, 1, "unmapped VPN faults (not in TLB nor PT)");
}

static void test_tlb_fast_path_priority(void) {
    test_suite("mmu: TLB fast path takes priority over PT");

    mmu_t m;
    mmu_init(&m);
    mmu_reset(&m);

    mmu_map_pt(&m, 0x2, 0x111);
    mmu_fill(&m, 0x2, 0x999);

    uint32_t pa;
    int fault;
    int ok = mmu_xlate(&m, make_vaddr(0x2, 0x5), &pa, &fault);
    CHECK_EQ_BIT(ok, 1, "translated");
    CHECK_EQ_BIT(m.tlb_hit, 1, "TLB hit (fast path)");
    CHECK_EQ_U32(pa, (0x999u << 12) | 0x5u, "paddr from TLB, not PT");
}

int main(void) {
    test_fault_before_fill();
    test_translate();
    test_multiple_pages();
    test_tlb_eviction();
    test_page_table_walk();
    test_tlb_fast_path_priority();
    return test_report();
}
