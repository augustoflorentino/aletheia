#include "test.h"
#include "../simulation/simulator.h"

#include "../system/board.h"
#include "../isa/isa.h"

static void load_at(board_t *b, int base, const uint32_t *words, int n) {
    bit w[32];
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < 32; j++) w[j] = (words[i] >> j) & 1u;
        rom_load_at(&b->rom, (unsigned)(base + i), w);
    }
}

static uint32_t reg(board_t *b, int i) {
    regfile_propagate(&b->regs);
    return from_bits_32(b->regs.regs[i].data_out);
}

static uint32_t ram(board_t *b, uint32_t addr) {
    bit a[32], out[32];
    to_bits_32(addr, a);
    ram_set_addr(&b->ram, a);
    ram_set_write(&b->ram, 0);
    ram_set_cs(&b->ram, 1);
    ram_propagate(&b->ram);
    ram_read(&b->ram, out);
    return from_bits_32(out);
}

static void (*step)(board_t *) = board_step;

static void run(board_t *b, int steps) {
    int c;
    for (c = 0; c < steps * 6 && !b->halt; c++) step(b);
}

#define PT_VALID  0x0100u
#define HALT_WORD 0xFC000000u

static int map_page(uint32_t *prog, int n, uint32_t vpn, uint32_t pfn, int valid) {
    prog[n++] = encode_i_type(OP_LUI, 0, REG_AT, (valid ? PT_VALID : 0u) | (vpn << 4));
    prog[n++] = encode_i_type(OP_ORI, REG_AT, REG_AT, pfn);
    prog[n++] = encode_i_type(OP_SW, REG_ZERO, REG_AT, 0xFF35);
    return n;
}

static int mmu_on(uint32_t *prog, int n) {
    prog[n++] = encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 1);
    prog[n++] = encode_i_type(OP_SW, REG_ZERO, REG_T0, 0xFF36);
    return n;
}

static void test_translation_and_alias(void) {
    test_suite("vm: 16-word pages, two VPNs aliased to the same frame");

    static board_t b;
    board_init(&b);
    board_pipe_init(&b);

    uint32_t prog[64];
    int n = 0;
    n = map_page(prog, n, 0, 0, 1);
    n = mmu_on(prog, n);
    prog[n++] = encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 42);
    prog[n++] = encode_i_type(OP_SW, REG_ZERO, REG_T1, 7);
    prog[n++] = encode_i_type(OP_LW, REG_ZERO, REG_T2, 7);
    n = map_page(prog, n, 3, 0, 1);
    prog[n++] = encode_i_type(OP_ADDI, REG_ZERO, REG_T3, 77);
    prog[n++] = encode_i_type(OP_SW, REG_ZERO, REG_T3, 0x37);
    prog[n++] = encode_i_type(OP_LW, REG_ZERO, REG_T4, 0x37);
    prog[n++] = encode_i_type(OP_LW, REG_ZERO, REG_T5, 7);
    prog[n++] = HALT_WORD;
    load_at(&b, 0, prog, n);

    run(&b, n + 5);

    CHECK_EQ_BIT(b.halt, 1, "program finished");
    CHECK_EQ_U32(reg(&b, REG_T2), 42u, "translated lw reads the written value");
    CHECK_EQ_U32(reg(&b, REG_T4), 77u, "access via VPN 3 works");
    CHECK_EQ_U32(reg(&b, REG_T5), 77u, "VPN 0 and VPN 3 point to the same physical frame");
    CHECK_EQ_U32(ram(&b, 7), 77u, "physical word 7 holds the last value");

    bit vaddr[32];
    to_bits_32(7u, vaddr);
    mmu_set_vaddr(&b.mmu, vaddr);
    mmu_propagate(&b.mmu);
    CHECK_EQ_BIT(b.mmu.tlb_hit, 1, "TLB was filled in hardware on the first access");
}

static void test_pfn_selects_frame(void) {
    test_suite("vm: the PFN chooses which RAM frame is accessed");

    static board_t b;
    board_init(&b);
    board_pipe_init(&b);

    uint32_t prog[64];
    int n = 0;
    n = map_page(prog, n, 2, 5, 1);
    n = map_page(prog, n, 0, 0, 1);
    n = mmu_on(prog, n);
    prog[n++] = encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 9);
    prog[n++] = encode_i_type(OP_SW, REG_ZERO, REG_T1, 0x23);
    prog[n++] = encode_i_type(OP_LW, REG_ZERO, REG_T2, 0x23);
    prog[n++] = HALT_WORD;
    load_at(&b, 0, prog, n);

    run(&b, n + 5);

    CHECK_EQ_BIT(b.halt, 1, "program finished");
    CHECK_EQ_U32(reg(&b, REG_T2), 9u, "load through the mapping");
    CHECK_EQ_U32(ram(&b, 0x53), 9u, "virtual 0x23 landed in physical 0x53 (frame 5)");
    CHECK_EQ_U32(ram(&b, 0x23), 0u, "virtual address used as physical was not touched");
}

static void test_unmap_faults(void) {
    test_suite("vm: unmapping a page makes the next access fault");

    static board_t b;
    board_init(&b);
    board_pipe_init(&b);

    uint32_t prog[64];
    int n = 0;
    n = map_page(prog, n, 2, 5, 1);
    n = mmu_on(prog, n);
    prog[n++] = encode_i_type(OP_LW, REG_ZERO, REG_T1, 0x21);
    n = map_page(prog, n, 2, 5, 0);
    prog[n++] = encode_i_type(OP_LW, REG_ZERO, REG_T2, 0x21);
    prog[n++] = HALT_WORD;
    uint32_t handler[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_S0, 0x99),
        HALT_WORD,
    };
    load_at(&b, 0, prog, n);
    load_at(&b, 0x50, handler, 2);

    run(&b, n + 10);

    CHECK_EQ_BIT(b.halt, 1, "program finished");
    CHECK_EQ_U32(reg(&b, REG_S0), 0x99u, "second access went to the fault handler");
    bit cbits[4];
    exc_get_cause(&b.exc, cbits);
    CHECK_EQ_U32(from_bits_n(cbits, 4), (uint32_t)EXC_ADDR_ERROR, "cause = EXC_ADDR_ERROR");
}

static void test_page_fault_handler_maps_and_retries(void) {
    test_suite("vm: page fault -> handler maps the page -> RETI re-executes");

    static board_t b;
    board_init(&b);
    board_pipe_init(&b);

    uint32_t prog[64];
    int n = 0;
    n = map_page(prog, n, 0, 0, 1);
    n = mmu_on(prog, n);
    prog[n++] = encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 55);
    prog[n++] = encode_i_type(OP_SW, REG_ZERO, REG_T1, 0x14);
    prog[n++] = encode_i_type(OP_LW, REG_ZERO, REG_T2, 0x14);
    prog[n++] = encode_i_type(OP_LW, REG_ZERO, REG_T3, 4);
    prog[n++] = HALT_WORD;
    uint32_t handler[8];
    int h = 0;
    h = map_page(handler, h, 1, 0, 1);
    handler[h++] = encode_j_type(OP_RETI, 0);
    load_at(&b, 0, prog, n);
    load_at(&b, 0x50, handler, h);

    run(&b, n + h + 10);

    CHECK_EQ_BIT(b.halt, 1, "program finished");
    CHECK_EQ_U32(reg(&b, REG_T2), 55u, "the missing sw was re-executed after the handler");
    CHECK_EQ_U32(reg(&b, REG_T3), 55u, "physical word 4 received the write");
    CHECK_EQ_BIT(b.exc.in_exception.q, 0, "in_exception cleared after RETI");

    bit cbits[4];
    exc_get_cause(&b.exc, cbits);
    CHECK_EQ_U32(from_bits_n(cbits, 4), (uint32_t)EXC_ADDR_ERROR, "cause = EXC_ADDR_ERROR");
}

static void test_mmu_off_is_identity(void) {
    test_suite("vm: with MMU off nothing changes");

    board_t b;
    board_init(&b);
    board_pipe_init(&b);

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 9),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 12),
        encode_i_type(OP_LW, REG_ZERO, REG_T1, 12),
        0xFC000000u,
    };
    load_at(&b, 0, prog, 4);

    run(&b, 6);

    CHECK_EQ_U32(reg(&b, REG_T1), 9u, "direct physical access works");
    CHECK_EQ_BIT(b.exc.in_exception.q, 0, "no exception without MMU");
}

static void run_all(void) {
    test_translation_and_alias();
    test_pfn_selects_frame();
    test_unmap_faults();
    test_page_fault_handler_maps_and_retries();
    test_mmu_off_is_identity();
}

int main(void) {
    run_all();
    test_suite("vm: same programs on the multi-cycle core");
    step = board_step_mc;
    run_all();
    test_suite("vm: same programs on the pipeline");
    step = board_step_pipe;
    run_all();
    return test_report();
}
