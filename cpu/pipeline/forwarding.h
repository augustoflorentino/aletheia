#ifndef ALETHEIA_FORWARDING_H
#define ALETHEIA_FORWARDING_H

#include "../../core/types.h"

typedef struct {
    bit id_ex_rs[5];
    bit id_ex_rt[5];
    bit ex_mem_rd[5];
    bit mem_wb_rd[5];
    bit ex_mem_reg_write;
    bit mem_wb_reg_write;

    bit forward_a[2];
    bit forward_b[2];
} forwarding_unit_t;

void forwarding_init(forwarding_unit_t *fu);
void forwarding_reset(forwarding_unit_t *fu);

void forwarding_set_id_ex_rs(forwarding_unit_t *fu, bit *rs);
void forwarding_set_id_ex_rt(forwarding_unit_t *fu, bit *rt);
void forwarding_set_ex_mem_rd(forwarding_unit_t *fu, bit *rd);
void forwarding_set_mem_wb_rd(forwarding_unit_t *fu, bit *rd);
void forwarding_set_ex_mem_reg_write(forwarding_unit_t *fu, bit reg_write);
void forwarding_set_mem_wb_reg_write(forwarding_unit_t *fu, bit reg_write);

void forwarding_propagate(forwarding_unit_t *fu);

void forwarding_get_forward_a(forwarding_unit_t *fu, bit *out);
void forwarding_get_forward_b(forwarding_unit_t *fu, bit *out);

#endif
