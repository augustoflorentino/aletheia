#include "test.h"

#include "../cpu/registers/register_file.h"
#include "../cpu/pipeline/forwarding.h"
#include "../cpu/alu/alu.h"

typedef struct {
    int valid;
    int rs, rt, rd;
    uint32_t rsv, rtv;
} idex_t;
typedef struct {
    int valid;
    int rd;
    uint32_t result;
} exmem_t;
typedef struct {
    int valid;
    int rd;
    uint32_t value;
} memwb_t;

typedef struct {
    int rs, rt, rd;
} rtype_t;

static void rf_write(register_file_t *rf, int idx, uint32_t v) {
    bit a[5], d[32];
    to_bits_n((uint32_t)idx, a, 5);
    to_bits_32(v, d);
    regfile_set_write_addr(rf, a);
    regfile_set_write_data(rf, d);
    regfile_set_write_enable(rf, 1);
    regfile_clock_fall(rf);
    regfile_clock_rise(rf);
    regfile_propagate(rf);
    regfile_set_write_enable(rf, 0);
}

static uint32_t rf_read(register_file_t *rf, int idx, int port2, int idx2) {
    bit a1[5], a2[5];
    to_bits_n((uint32_t)idx, a1, 5);
    to_bits_n((uint32_t)idx2, a2, 5);
    regfile_set_read_addr1(rf, a1);
    regfile_set_read_addr2(rf, a2);
    regfile_propagate(rf);
    return port2 ? from_bits_32(rf->read_data2) : from_bits_32(rf->read_data1);
}

static uint32_t alu_add(uint32_t a, uint32_t b) {
    bit ba[32], bb[32], op[8], r[32];
    alu_flags_t f;
    to_bits_32(a, ba);
    to_bits_32(b, bb);
    to_bits_n((uint32_t)ALU_OP_ADD, op, 3);
    alu_execute(ba, bb, op, r, &f);
    return from_bits_32(r);
}

static uint32_t fwd_select(bit *fwd, uint32_t base, uint32_t exval, uint32_t memval) {
    if (fwd[1]) return exval;
    if (fwd[0]) return memval;
    return base;
}

static void run_pipeline(rtype_t *prog, int n, int apply_fwd, uint32_t *regs_out) {
    register_file_t rf;
    regfile_init(&rf);
    regfile_reset(&rf);
    rf_write(&rf, 1, 10);
    rf_write(&rf, 2, 20);
    rf_write(&rf, 3, 100);

    forwarding_unit_t fu;
    forwarding_init(&fu);

    int ifid_valid = 0, ifid_idx = 0;
    idex_t idex = {0, 0, 0, 0, 0, 0};
    exmem_t exmem = {0, 0, 0};
    memwb_t memwb = {0, 0, 0};

    int pc = 0;
    int cycles = n + 5;
    int c;
    for (c = 0; c < cycles; c++) {
        if (memwb.valid) rf_write(&rf, memwb.rd, memwb.value);

        idex_t oi = idex;
        exmem_t oe = exmem;
        memwb_t om = memwb;

        uint32_t opa = oi.rsv, opb = oi.rtv;
        if (apply_fwd && oi.valid) {
            bit rs[5], rt[5], erd[5], mrd[5];
            to_bits_n((uint32_t)oi.rs, rs, 5);
            to_bits_n((uint32_t)oi.rt, rt, 5);
            to_bits_n((uint32_t)oe.rd, erd, 5);
            to_bits_n((uint32_t)om.rd, mrd, 5);
            forwarding_set_id_ex_rs(&fu, rs);
            forwarding_set_id_ex_rt(&fu, rt);
            forwarding_set_ex_mem_rd(&fu, erd);
            forwarding_set_mem_wb_rd(&fu, mrd);
            forwarding_set_ex_mem_reg_write(&fu, (bit)oe.valid);
            forwarding_set_mem_wb_reg_write(&fu, (bit)om.valid);
            forwarding_propagate(&fu);
            bit fa[2], fb[2];
            forwarding_get_forward_a(&fu, fa);
            forwarding_get_forward_b(&fu, fb);
            opa = fwd_select(fa, oi.rsv, oe.result, om.value);
            opb = fwd_select(fb, oi.rtv, oe.result, om.value);
        }
        uint32_t exres = alu_add(opa, opb);

        exmem.valid = oi.valid;
        exmem.rd = oi.rd;
        exmem.result = exres;
        memwb.valid = oe.valid;
        memwb.rd = oe.rd;
        memwb.value = oe.result;

        if (ifid_valid) {
            idex.valid = 1;
            idex.rs = prog[ifid_idx].rs;
            idex.rt = prog[ifid_idx].rt;
            idex.rd = prog[ifid_idx].rd;
            idex.rsv = rf_read(&rf, prog[ifid_idx].rs, 0, prog[ifid_idx].rt);
            idex.rtv = rf_read(&rf, prog[ifid_idx].rs, 1, prog[ifid_idx].rt);
        } else {
            idex.valid = 0;
        }

        if (pc < n) {
            ifid_valid = 1;
            ifid_idx = pc;
            pc++;
        } else {
            ifid_valid = 0;
        }
    }

    int i;
    for (i = 0; i < 8; i++) regs_out[i] = rf_read(&rf, i, 0, 0);
}

static void ref_exec(rtype_t *prog, int n, uint32_t *regs) {
    int i;
    for (i = 0; i < 8; i++) regs[i] = 0;
    regs[1] = 10;
    regs[2] = 20;
    regs[3] = 100;
    for (i = 0; i < n; i++) {
        regs[prog[i].rd] = regs[prog[i].rs] + regs[prog[i].rt];
    }
}

static void test_forwarding_resolves(void) {
    test_suite("pipeline regression: forwarding resolves RAW hazards");

    rtype_t prog[] = {{1, 2, 4}, {4, 3, 5}, {4, 5, 6}};
    int n = 3;

    uint32_t ref[8], piped[8];
    ref_exec(prog, n, ref);
    run_pipeline(prog, n, 1, piped);

    CHECK_EQ_U32(piped[4], ref[4], "reg $4 (=30)");
    CHECK_EQ_U32(piped[5], ref[5], "reg $5 (=130, forwarded)");
    CHECK_EQ_U32(piped[6], ref[6], "reg $6 (=160, forwarded)");
}

static void test_without_forwarding_diverges(void) {
    test_suite("pipeline regression: WITHOUT forwarding it diverges (hazard real)");

    rtype_t prog[] = {{1, 2, 4}, {4, 3, 5}, {4, 5, 6}};
    int n = 3;

    uint32_t ref[8], nofwd[8];
    ref_exec(prog, n, ref);
    run_pipeline(prog, n, 0, nofwd);

    CHECK(nofwd[5] != ref[5], "without forwarding, $5 comes out wrong (reads stale $4)");
}

int main(void) {
    test_forwarding_resolves();
    test_without_forwarding_diverges();
    return test_report();
}
