#ifndef ALETHEIA_HAZARD_UNIT_H
#define ALETHEIA_HAZARD_UNIT_H

#include "../../core/types.h"

typedef struct {
    bit id_ex_mem_read;
    bit id_ex_rt[5];
    bit if_id_rs[5];
    bit if_id_rt[5];

    bit ex_mem_branch;
    bit ex_mem_zero;

    bit stall;
    bit flush_if_id;
    bit flush_id_ex;
    bit pc_write;
    bit if_id_write;
} hazard_unit_t;

void hazard_init(hazard_unit_t *hu);
void hazard_reset(hazard_unit_t *hu);

void hazard_set_id_ex_mem_read(hazard_unit_t *hu, bit mem_read);
void hazard_set_id_ex_rt(hazard_unit_t *hu, bit *rt);
void hazard_set_if_id_rs(hazard_unit_t *hu, bit *rs);
void hazard_set_if_id_rt(hazard_unit_t *hu, bit *rt);
void hazard_set_ex_mem_branch(hazard_unit_t *hu, bit branch);
void hazard_set_ex_mem_zero(hazard_unit_t *hu, bit zero);

void hazard_propagate(hazard_unit_t *hu);

bit hazard_get_stall(hazard_unit_t *hu);
bit hazard_get_flush_if_id(hazard_unit_t *hu);
bit hazard_get_flush_id_ex(hazard_unit_t *hu);
bit hazard_get_pc_write(hazard_unit_t *hu);
bit hazard_get_if_id_write(hazard_unit_t *hu);

#endif
