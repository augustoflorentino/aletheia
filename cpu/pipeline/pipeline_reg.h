#ifndef ALETHEIA_PIPELINE_REG_H
#define ALETHEIA_PIPELINE_REG_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"

typedef struct {
    d_flip_flop_t pc[32];
    d_flip_flop_t instruction[32];
    d_flip_flop_t valid;
    bit flush;
    bit stall;
} if_id_reg_t;

typedef struct {
    d_flip_flop_t pc[32];
    d_flip_flop_t rs_data[32];
    d_flip_flop_t rt_data[32];
    d_flip_flop_t imm_extended[32];
    d_flip_flop_t rs[5];
    d_flip_flop_t rt[5];
    d_flip_flop_t rd[5];
    d_flip_flop_t alu_op[4];
    d_flip_flop_t alu_src;
    d_flip_flop_t reg_dst;
    d_flip_flop_t mem_read;
    d_flip_flop_t mem_write;
    d_flip_flop_t mem_to_reg;
    d_flip_flop_t reg_write;
    d_flip_flop_t branch;
    d_flip_flop_t branch_ne;
    d_flip_flop_t imm_zero_ext;
    d_flip_flop_t shift_rt;
    d_flip_flop_t shift_shamt;
    d_flip_flop_t lui;
    d_flip_flop_t link;
    d_flip_flop_t jr;
    d_flip_flop_t undef;
    d_flip_flop_t reti;
    d_flip_flop_t valid;
    bit flush;
    bit stall;
} id_ex_reg_t;

typedef struct {
    d_flip_flop_t alu_result[32];
    d_flip_flop_t rt_data[32];
    d_flip_flop_t write_reg[5];
    d_flip_flop_t mem_read;
    d_flip_flop_t mem_write;
    d_flip_flop_t mem_to_reg;
    d_flip_flop_t reg_write;
    d_flip_flop_t zero_flag;
    d_flip_flop_t undef;
    d_flip_flop_t reti;
    d_flip_flop_t pc[32];
    d_flip_flop_t valid;
    bit flush;
} ex_mem_reg_t;

typedef struct {
    d_flip_flop_t alu_result[32];
    d_flip_flop_t mem_data[32];
    d_flip_flop_t write_reg[5];
    d_flip_flop_t mem_to_reg;
    d_flip_flop_t reg_write;
    d_flip_flop_t valid;
} mem_wb_reg_t;

void if_id_init(if_id_reg_t *reg);
void if_id_reset(if_id_reg_t *reg);
void if_id_clock_rise(if_id_reg_t *reg);
void if_id_clock_fall(if_id_reg_t *reg);
void if_id_set_flush(if_id_reg_t *reg, bit flush);
void if_id_set_stall(if_id_reg_t *reg, bit stall);

void id_ex_init(id_ex_reg_t *reg);
void id_ex_reset(id_ex_reg_t *reg);
void id_ex_clock_rise(id_ex_reg_t *reg);
void id_ex_clock_fall(id_ex_reg_t *reg);
void id_ex_set_flush(id_ex_reg_t *reg, bit flush);
void id_ex_set_stall(id_ex_reg_t *reg, bit stall);

void ex_mem_init(ex_mem_reg_t *reg);
void ex_mem_reset(ex_mem_reg_t *reg);
void ex_mem_clock_rise(ex_mem_reg_t *reg);
void ex_mem_clock_fall(ex_mem_reg_t *reg);
void ex_mem_set_flush(ex_mem_reg_t *reg, bit flush);

void mem_wb_init(mem_wb_reg_t *reg);
void mem_wb_reset(mem_wb_reg_t *reg);
void mem_wb_clock_rise(mem_wb_reg_t *reg);
void mem_wb_clock_fall(mem_wb_reg_t *reg);

#endif
