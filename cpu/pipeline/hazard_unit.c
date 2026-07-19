#include "hazard_unit.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

void hazard_init(hazard_unit_t *hu) {
    hu->id_ex_mem_read = 0;
    hu->id_ex_rt[0] = 0; hu->id_ex_rt[1] = 0; hu->id_ex_rt[2] = 0;
    hu->id_ex_rt[3] = 0; hu->id_ex_rt[4] = 0;
    hu->if_id_rs[0] = 0; hu->if_id_rs[1] = 0; hu->if_id_rs[2] = 0;
    hu->if_id_rs[3] = 0; hu->if_id_rs[4] = 0;
    hu->if_id_rt[0] = 0; hu->if_id_rt[1] = 0; hu->if_id_rt[2] = 0;
    hu->if_id_rt[3] = 0; hu->if_id_rt[4] = 0;
    hu->ex_mem_branch = 0;
    hu->ex_mem_zero = 0;
    hu->stall = 0;
    hu->flush_if_id = 0;
    hu->flush_id_ex = 0;
    hu->pc_write = 1;
    hu->if_id_write = 1;
}

void hazard_reset(hazard_unit_t *hu) {
    hazard_init(hu);
}

void hazard_set_id_ex_mem_read(hazard_unit_t *hu, bit mem_read) {
    hu->id_ex_mem_read = mem_read;
}

void hazard_set_id_ex_rt(hazard_unit_t *hu, bit *rt) {
    hu->id_ex_rt[0] = rt[0]; hu->id_ex_rt[1] = rt[1]; hu->id_ex_rt[2] = rt[2];
    hu->id_ex_rt[3] = rt[3]; hu->id_ex_rt[4] = rt[4];
}

void hazard_set_if_id_rs(hazard_unit_t *hu, bit *rs) {
    hu->if_id_rs[0] = rs[0]; hu->if_id_rs[1] = rs[1]; hu->if_id_rs[2] = rs[2];
    hu->if_id_rs[3] = rs[3]; hu->if_id_rs[4] = rs[4];
}

void hazard_set_if_id_rt(hazard_unit_t *hu, bit *rt) {
    hu->if_id_rt[0] = rt[0]; hu->if_id_rt[1] = rt[1]; hu->if_id_rt[2] = rt[2];
    hu->if_id_rt[3] = rt[3]; hu->if_id_rt[4] = rt[4];
}

void hazard_set_ex_mem_branch(hazard_unit_t *hu, bit branch) {
    hu->ex_mem_branch = branch;
}

void hazard_set_ex_mem_zero(hazard_unit_t *hu, bit zero) {
    hu->ex_mem_zero = zero;
}

void hazard_propagate(hazard_unit_t *hu) {
    bit eq_rs_0 = XNOR(hu->id_ex_rt[0], hu->if_id_rs[0]);
    bit eq_rs_1 = XNOR(hu->id_ex_rt[1], hu->if_id_rs[1]);
    bit eq_rs_2 = XNOR(hu->id_ex_rt[2], hu->if_id_rs[2]);
    bit eq_rs_3 = XNOR(hu->id_ex_rt[3], hu->if_id_rs[3]);
    bit eq_rs_4 = XNOR(hu->id_ex_rt[4], hu->if_id_rs[4]);
    bit rt_eq_rs = AND(AND(AND(eq_rs_0, eq_rs_1), AND(eq_rs_2, eq_rs_3)), eq_rs_4);

    bit eq_rt_0 = XNOR(hu->id_ex_rt[0], hu->if_id_rt[0]);
    bit eq_rt_1 = XNOR(hu->id_ex_rt[1], hu->if_id_rt[1]);
    bit eq_rt_2 = XNOR(hu->id_ex_rt[2], hu->if_id_rt[2]);
    bit eq_rt_3 = XNOR(hu->id_ex_rt[3], hu->if_id_rt[3]);
    bit eq_rt_4 = XNOR(hu->id_ex_rt[4], hu->if_id_rt[4]);
    bit rt_eq_rt = AND(AND(AND(eq_rt_0, eq_rt_1), AND(eq_rt_2, eq_rt_3)), eq_rt_4);

    bit load_use_hazard = AND(hu->id_ex_mem_read, OR(rt_eq_rs, rt_eq_rt));

    bit branch_taken = AND(hu->ex_mem_branch, hu->ex_mem_zero);

    hu->stall = load_use_hazard;
    hu->flush_if_id = branch_taken;
    hu->flush_id_ex = OR(load_use_hazard, branch_taken);
    hu->pc_write = NOT(load_use_hazard);
    hu->if_id_write = NOT(load_use_hazard);
}

bit hazard_get_stall(hazard_unit_t *hu) {
    return hu->stall;
}

bit hazard_get_flush_if_id(hazard_unit_t *hu) {
    return hu->flush_if_id;
}

bit hazard_get_flush_id_ex(hazard_unit_t *hu) {
    return hu->flush_id_ex;
}

bit hazard_get_pc_write(hazard_unit_t *hu) {
    return hu->pc_write;
}

bit hazard_get_if_id_write(hazard_unit_t *hu) {
    return hu->if_id_write;
}
