#include "forwarding.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

void forwarding_init(forwarding_unit_t *fu) {
    fu->id_ex_rs[0] = 0; fu->id_ex_rs[1] = 0; fu->id_ex_rs[2] = 0;
    fu->id_ex_rs[3] = 0; fu->id_ex_rs[4] = 0;
    fu->id_ex_rt[0] = 0; fu->id_ex_rt[1] = 0; fu->id_ex_rt[2] = 0;
    fu->id_ex_rt[3] = 0; fu->id_ex_rt[4] = 0;
    fu->ex_mem_rd[0] = 0; fu->ex_mem_rd[1] = 0; fu->ex_mem_rd[2] = 0;
    fu->ex_mem_rd[3] = 0; fu->ex_mem_rd[4] = 0;
    fu->mem_wb_rd[0] = 0; fu->mem_wb_rd[1] = 0; fu->mem_wb_rd[2] = 0;
    fu->mem_wb_rd[3] = 0; fu->mem_wb_rd[4] = 0;
    fu->ex_mem_reg_write = 0;
    fu->mem_wb_reg_write = 0;
    fu->forward_a[0] = 0; fu->forward_a[1] = 0;
    fu->forward_b[0] = 0; fu->forward_b[1] = 0;
}

void forwarding_reset(forwarding_unit_t *fu) {
    forwarding_init(fu);
}

void forwarding_set_id_ex_rs(forwarding_unit_t *fu, bit *rs) {
    fu->id_ex_rs[0] = rs[0]; fu->id_ex_rs[1] = rs[1]; fu->id_ex_rs[2] = rs[2];
    fu->id_ex_rs[3] = rs[3]; fu->id_ex_rs[4] = rs[4];
}

void forwarding_set_id_ex_rt(forwarding_unit_t *fu, bit *rt) {
    fu->id_ex_rt[0] = rt[0]; fu->id_ex_rt[1] = rt[1]; fu->id_ex_rt[2] = rt[2];
    fu->id_ex_rt[3] = rt[3]; fu->id_ex_rt[4] = rt[4];
}

void forwarding_set_ex_mem_rd(forwarding_unit_t *fu, bit *rd) {
    fu->ex_mem_rd[0] = rd[0]; fu->ex_mem_rd[1] = rd[1]; fu->ex_mem_rd[2] = rd[2];
    fu->ex_mem_rd[3] = rd[3]; fu->ex_mem_rd[4] = rd[4];
}

void forwarding_set_mem_wb_rd(forwarding_unit_t *fu, bit *rd) {
    fu->mem_wb_rd[0] = rd[0]; fu->mem_wb_rd[1] = rd[1]; fu->mem_wb_rd[2] = rd[2];
    fu->mem_wb_rd[3] = rd[3]; fu->mem_wb_rd[4] = rd[4];
}

void forwarding_set_ex_mem_reg_write(forwarding_unit_t *fu, bit reg_write) {
    fu->ex_mem_reg_write = reg_write;
}

void forwarding_set_mem_wb_reg_write(forwarding_unit_t *fu, bit reg_write) {
    fu->mem_wb_reg_write = reg_write;
}

void forwarding_propagate(forwarding_unit_t *fu) {
    bit ex_mem_rd_not_zero = OR(OR(fu->ex_mem_rd[0], fu->ex_mem_rd[1]),
                                OR(OR(fu->ex_mem_rd[2], fu->ex_mem_rd[3]), fu->ex_mem_rd[4]));
    bit mem_wb_rd_not_zero = OR(OR(fu->mem_wb_rd[0], fu->mem_wb_rd[1]),
                                OR(OR(fu->mem_wb_rd[2], fu->mem_wb_rd[3]), fu->mem_wb_rd[4]));

    bit eq_ex_mem_rs_0 = XNOR(fu->ex_mem_rd[0], fu->id_ex_rs[0]);
    bit eq_ex_mem_rs_1 = XNOR(fu->ex_mem_rd[1], fu->id_ex_rs[1]);
    bit eq_ex_mem_rs_2 = XNOR(fu->ex_mem_rd[2], fu->id_ex_rs[2]);
    bit eq_ex_mem_rs_3 = XNOR(fu->ex_mem_rd[3], fu->id_ex_rs[3]);
    bit eq_ex_mem_rs_4 = XNOR(fu->ex_mem_rd[4], fu->id_ex_rs[4]);
    bit ex_mem_rd_eq_rs = AND(AND(AND(eq_ex_mem_rs_0, eq_ex_mem_rs_1),
                                  AND(eq_ex_mem_rs_2, eq_ex_mem_rs_3)), eq_ex_mem_rs_4);

    bit eq_ex_mem_rt_0 = XNOR(fu->ex_mem_rd[0], fu->id_ex_rt[0]);
    bit eq_ex_mem_rt_1 = XNOR(fu->ex_mem_rd[1], fu->id_ex_rt[1]);
    bit eq_ex_mem_rt_2 = XNOR(fu->ex_mem_rd[2], fu->id_ex_rt[2]);
    bit eq_ex_mem_rt_3 = XNOR(fu->ex_mem_rd[3], fu->id_ex_rt[3]);
    bit eq_ex_mem_rt_4 = XNOR(fu->ex_mem_rd[4], fu->id_ex_rt[4]);
    bit ex_mem_rd_eq_rt = AND(AND(AND(eq_ex_mem_rt_0, eq_ex_mem_rt_1),
                                  AND(eq_ex_mem_rt_2, eq_ex_mem_rt_3)), eq_ex_mem_rt_4);

    bit eq_mem_wb_rs_0 = XNOR(fu->mem_wb_rd[0], fu->id_ex_rs[0]);
    bit eq_mem_wb_rs_1 = XNOR(fu->mem_wb_rd[1], fu->id_ex_rs[1]);
    bit eq_mem_wb_rs_2 = XNOR(fu->mem_wb_rd[2], fu->id_ex_rs[2]);
    bit eq_mem_wb_rs_3 = XNOR(fu->mem_wb_rd[3], fu->id_ex_rs[3]);
    bit eq_mem_wb_rs_4 = XNOR(fu->mem_wb_rd[4], fu->id_ex_rs[4]);
    bit mem_wb_rd_eq_rs = AND(AND(AND(eq_mem_wb_rs_0, eq_mem_wb_rs_1),
                                  AND(eq_mem_wb_rs_2, eq_mem_wb_rs_3)), eq_mem_wb_rs_4);

    bit eq_mem_wb_rt_0 = XNOR(fu->mem_wb_rd[0], fu->id_ex_rt[0]);
    bit eq_mem_wb_rt_1 = XNOR(fu->mem_wb_rd[1], fu->id_ex_rt[1]);
    bit eq_mem_wb_rt_2 = XNOR(fu->mem_wb_rd[2], fu->id_ex_rt[2]);
    bit eq_mem_wb_rt_3 = XNOR(fu->mem_wb_rd[3], fu->id_ex_rt[3]);
    bit eq_mem_wb_rt_4 = XNOR(fu->mem_wb_rd[4], fu->id_ex_rt[4]);
    bit mem_wb_rd_eq_rt = AND(AND(AND(eq_mem_wb_rt_0, eq_mem_wb_rt_1),
                                  AND(eq_mem_wb_rt_2, eq_mem_wb_rt_3)), eq_mem_wb_rt_4);

    bit forward_from_ex_mem_a = AND(AND(fu->ex_mem_reg_write, ex_mem_rd_not_zero), ex_mem_rd_eq_rs);
    bit forward_from_mem_wb_a = AND(AND(AND(fu->mem_wb_reg_write, mem_wb_rd_not_zero), mem_wb_rd_eq_rs),
                                    NOT(forward_from_ex_mem_a));

    bit forward_from_ex_mem_b = AND(AND(fu->ex_mem_reg_write, ex_mem_rd_not_zero), ex_mem_rd_eq_rt);
    bit forward_from_mem_wb_b = AND(AND(AND(fu->mem_wb_reg_write, mem_wb_rd_not_zero), mem_wb_rd_eq_rt),
                                    NOT(forward_from_ex_mem_b));

    fu->forward_a[0] = OR(forward_from_ex_mem_a, forward_from_mem_wb_a);
    fu->forward_a[1] = forward_from_ex_mem_a;

    fu->forward_b[0] = OR(forward_from_ex_mem_b, forward_from_mem_wb_b);
    fu->forward_b[1] = forward_from_ex_mem_b;
}

void forwarding_get_forward_a(forwarding_unit_t *fu, bit *out) {
    out[0] = fu->forward_a[0];
    out[1] = fu->forward_a[1];
}

void forwarding_get_forward_b(forwarding_unit_t *fu, bit *out) {
    out[0] = fu->forward_b[0];
    out[1] = fu->forward_b[1];
}
