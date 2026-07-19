#include "test.h"

#include "../cpu/pipeline/hazard_unit.h"
#include "../cpu/pipeline/forwarding.h"

static void set5(bit *arr, uint32_t v) {
    to_bits_n(v & 31u, arr, 5);
}

static void ref_hazard(uint32_t id_ex_rt, uint32_t if_id_rs, uint32_t if_id_rt,
                       int mem_read, int branch, int zero,
                       int *stall, int *flush_if_id, int *flush_id_ex,
                       int *pc_write, int *if_id_write) {
    int rt_eq_rs = (id_ex_rt == if_id_rs);
    int rt_eq_rt = (id_ex_rt == if_id_rt);
    int load_use = mem_read && (rt_eq_rs || rt_eq_rt);
    int branch_taken = branch && zero;
    *stall = load_use;
    *flush_if_id = branch_taken;
    *flush_id_ex = load_use || branch_taken;
    *pc_write = !load_use;
    *if_id_write = !load_use;
}

static void test_hazard(void) {
    test_suite("pipeline: hazard unit");

    hazard_unit_t hu;
    hazard_init(&hu);

    rng_seed(0x4A2Du);
    int k;
    for (k = 0; k < 6000; k++) {
        uint32_t id_ex_rt = rng_next() & 31u;
        uint32_t if_id_rs = rng_next() & 31u;
        uint32_t if_id_rt = rng_next() & 31u;
        int mem_read = rng_next() & 1;
        int branch = rng_next() & 1;
        int zero = rng_next() & 1;

        if ((k % 5) == 0) if_id_rs = id_ex_rt;
        if ((k % 5) == 1) if_id_rt = id_ex_rt;

        bit b_rt[5], b_rs[5], b_rt2[5];
        set5(b_rt, id_ex_rt);
        set5(b_rs, if_id_rs);
        set5(b_rt2, if_id_rt);
        hazard_set_id_ex_mem_read(&hu, mem_read);
        hazard_set_id_ex_rt(&hu, b_rt);
        hazard_set_if_id_rs(&hu, b_rs);
        hazard_set_if_id_rt(&hu, b_rt2);
        hazard_set_ex_mem_branch(&hu, branch);
        hazard_set_ex_mem_zero(&hu, zero);
        hazard_propagate(&hu);

        int stall, fifid, fidex, pcw, ifidw;
        ref_hazard(id_ex_rt, if_id_rs, if_id_rt, mem_read, branch, zero,
                   &stall, &fifid, &fidex, &pcw, &ifidw);

        CHECK_EQ_BIT(hazard_get_stall(&hu), stall, "hazard stall");
        CHECK_EQ_BIT(hazard_get_flush_if_id(&hu), fifid, "hazard flush_if_id");
        CHECK_EQ_BIT(hazard_get_flush_id_ex(&hu), fidex, "hazard flush_id_ex");
        CHECK_EQ_BIT(hazard_get_pc_write(&hu), pcw, "hazard pc_write");
        CHECK_EQ_BIT(hazard_get_if_id_write(&hu), ifidw, "hazard if_id_write");
    }
}

static void ref_forward(uint32_t rs, uint32_t rt, uint32_t ex_rd, uint32_t wb_rd,
                        int ex_rw, int wb_rw, int *fa0, int *fa1, int *fb0, int *fb1) {
    int ex_nz = (ex_rd != 0);
    int wb_nz = (wb_rd != 0);
    int fwd_ex_a = ex_rw && ex_nz && (ex_rd == rs);
    int fwd_wb_a = wb_rw && wb_nz && (wb_rd == rs) && !fwd_ex_a;
    int fwd_ex_b = ex_rw && ex_nz && (ex_rd == rt);
    int fwd_wb_b = wb_rw && wb_nz && (wb_rd == rt) && !fwd_ex_b;
    *fa0 = fwd_ex_a || fwd_wb_a;
    *fa1 = fwd_ex_a;
    *fb0 = fwd_ex_b || fwd_wb_b;
    *fb1 = fwd_ex_b;
}

static void test_forwarding(void) {
    test_suite("pipeline: forwarding unit");

    forwarding_unit_t fu;
    forwarding_init(&fu);

    rng_seed(0x7F0Du);
    int k;
    for (k = 0; k < 8000; k++) {
        uint32_t rs = rng_next() & 31u;
        uint32_t rt = rng_next() & 31u;
        uint32_t ex_rd = rng_next() & 31u;
        uint32_t wb_rd = rng_next() & 31u;
        int ex_rw = rng_next() & 1;
        int wb_rw = rng_next() & 1;

        if ((k % 6) == 0) ex_rd = rs;
        if ((k % 6) == 1) wb_rd = rs;
        if ((k % 6) == 2) { ex_rd = rs; wb_rd = rs; }
        if ((k % 6) == 3) ex_rd = rt;

        bit b_rs[5], b_rt[5], b_ex[5], b_wb[5];
        set5(b_rs, rs); set5(b_rt, rt); set5(b_ex, ex_rd); set5(b_wb, wb_rd);
        forwarding_set_id_ex_rs(&fu, b_rs);
        forwarding_set_id_ex_rt(&fu, b_rt);
        forwarding_set_ex_mem_rd(&fu, b_ex);
        forwarding_set_mem_wb_rd(&fu, b_wb);
        forwarding_set_ex_mem_reg_write(&fu, ex_rw);
        forwarding_set_mem_wb_reg_write(&fu, wb_rw);
        forwarding_propagate(&fu);

        bit fa[2], fb[2];
        forwarding_get_forward_a(&fu, fa);
        forwarding_get_forward_b(&fu, fb);

        int rfa0, rfa1, rfb0, rfb1;
        ref_forward(rs, rt, ex_rd, wb_rd, ex_rw, wb_rw, &rfa0, &rfa1, &rfb0, &rfb1);
        CHECK_EQ_BIT(fa[0], rfa0, "forward_a[0]");
        CHECK_EQ_BIT(fa[1], rfa1, "forward_a[1]");
        CHECK_EQ_BIT(fb[0], rfb0, "forward_b[0]");
        CHECK_EQ_BIT(fb[1], rfb1, "forward_b[1]");
    }
}

int main(void) {
    test_hazard();
    test_forwarding();
    return test_report();
}
