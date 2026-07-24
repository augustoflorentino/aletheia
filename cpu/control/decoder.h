#ifndef ALETHEIA_DECODER_H
#define ALETHEIA_DECODER_H

#include "../../core/types.h"

typedef struct {
    bit opcode[6];
    bit funct[6];
    bit rs[5];
    bit rt[5];
    bit rd[5];
    bit shamt[5];
    bit imm[16];
    bit addr[26];

    bit alu_op[4];
    bit reg_dst;
    bit alu_src;
    bit mem_to_reg;
    bit reg_write;
    bit mem_read;
    bit mem_write;
    bit branch;
    bit jump;
    bit use_imm;
    bit jal;
    bit jr;
    bit reti;
    bit imm_zero_ext;
    bit shift_rt;
    bit shift_shamt;
    bit lui;
    bit branch_ne;
    bit link;
    bit undefined;
} instruction_decoder_t;

void decoder_init(instruction_decoder_t *dec);
void decoder_reset(instruction_decoder_t *dec);
void decoder_set_instruction(instruction_decoder_t *dec, bit *instruction);
void decoder_propagate(instruction_decoder_t *dec);

bit decoder_get_reg_dst(instruction_decoder_t *dec);
bit decoder_get_alu_src(instruction_decoder_t *dec);
bit decoder_get_mem_to_reg(instruction_decoder_t *dec);
bit decoder_get_reg_write(instruction_decoder_t *dec);
bit decoder_get_mem_read(instruction_decoder_t *dec);
bit decoder_get_mem_write(instruction_decoder_t *dec);
bit decoder_get_branch(instruction_decoder_t *dec);
bit decoder_get_jump(instruction_decoder_t *dec);

#endif
