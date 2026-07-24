#include "decoder.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

void decoder_init(instruction_decoder_t *dec) {
    dec->opcode[0] = 0; dec->opcode[1] = 0; dec->opcode[2] = 0;
    dec->opcode[3] = 0; dec->opcode[4] = 0; dec->opcode[5] = 0;
    dec->funct[0] = 0; dec->funct[1] = 0; dec->funct[2] = 0;
    dec->funct[3] = 0; dec->funct[4] = 0; dec->funct[5] = 0;
    dec->rs[0] = 0; dec->rs[1] = 0; dec->rs[2] = 0; dec->rs[3] = 0; dec->rs[4] = 0;
    dec->rt[0] = 0; dec->rt[1] = 0; dec->rt[2] = 0; dec->rt[3] = 0; dec->rt[4] = 0;
    dec->rd[0] = 0; dec->rd[1] = 0; dec->rd[2] = 0; dec->rd[3] = 0; dec->rd[4] = 0;
    dec->shamt[0] = 0; dec->shamt[1] = 0; dec->shamt[2] = 0; dec->shamt[3] = 0; dec->shamt[4] = 0;
    dec->imm[0] = 0; dec->imm[1] = 0; dec->imm[2] = 0; dec->imm[3] = 0;
    dec->imm[4] = 0; dec->imm[5] = 0; dec->imm[6] = 0; dec->imm[7] = 0;
    dec->imm[8] = 0; dec->imm[9] = 0; dec->imm[10] = 0; dec->imm[11] = 0;
    dec->imm[12] = 0; dec->imm[13] = 0; dec->imm[14] = 0; dec->imm[15] = 0;
    dec->addr[0] = 0; dec->addr[1] = 0; dec->addr[2] = 0; dec->addr[3] = 0;
    dec->addr[4] = 0; dec->addr[5] = 0; dec->addr[6] = 0; dec->addr[7] = 0;
    dec->addr[8] = 0; dec->addr[9] = 0; dec->addr[10] = 0; dec->addr[11] = 0;
    dec->addr[12] = 0; dec->addr[13] = 0; dec->addr[14] = 0; dec->addr[15] = 0;
    dec->addr[16] = 0; dec->addr[17] = 0; dec->addr[18] = 0; dec->addr[19] = 0;
    dec->addr[20] = 0; dec->addr[21] = 0; dec->addr[22] = 0; dec->addr[23] = 0;
    dec->addr[24] = 0; dec->addr[25] = 0;

    dec->alu_op[0] = 0; dec->alu_op[1] = 0; dec->alu_op[2] = 0; dec->alu_op[3] = 0;
    dec->reg_dst = 0;
    dec->alu_src = 0;
    dec->mem_to_reg = 0;
    dec->reg_write = 0;
    dec->mem_read = 0;
    dec->mem_write = 0;
    dec->branch = 0;
    dec->jump = 0;
    dec->use_imm = 0;
    dec->jal = 0;
    dec->jr = 0;
    dec->reti = 0;
    dec->imm_zero_ext = 0;
    dec->shift_rt = 0;
    dec->shift_shamt = 0;
    dec->lui = 0;
    dec->branch_ne = 0;
    dec->link = 0;
    dec->undefined = 0;
}

void decoder_reset(instruction_decoder_t *dec) {
    decoder_init(dec);
}

void decoder_set_instruction(instruction_decoder_t *dec, bit *instruction) {
    dec->opcode[0] = instruction[26];
    dec->opcode[1] = instruction[27];
    dec->opcode[2] = instruction[28];
    dec->opcode[3] = instruction[29];
    dec->opcode[4] = instruction[30];
    dec->opcode[5] = instruction[31];

    dec->rs[0] = instruction[21];
    dec->rs[1] = instruction[22];
    dec->rs[2] = instruction[23];
    dec->rs[3] = instruction[24];
    dec->rs[4] = instruction[25];

    dec->rt[0] = instruction[16];
    dec->rt[1] = instruction[17];
    dec->rt[2] = instruction[18];
    dec->rt[3] = instruction[19];
    dec->rt[4] = instruction[20];

    dec->rd[0] = instruction[11];
    dec->rd[1] = instruction[12];
    dec->rd[2] = instruction[13];
    dec->rd[3] = instruction[14];
    dec->rd[4] = instruction[15];

    dec->shamt[0] = instruction[6];
    dec->shamt[1] = instruction[7];
    dec->shamt[2] = instruction[8];
    dec->shamt[3] = instruction[9];
    dec->shamt[4] = instruction[10];

    dec->funct[0] = instruction[0];
    dec->funct[1] = instruction[1];
    dec->funct[2] = instruction[2];
    dec->funct[3] = instruction[3];
    dec->funct[4] = instruction[4];
    dec->funct[5] = instruction[5];

    dec->imm[0] = instruction[0];
    dec->imm[1] = instruction[1];
    dec->imm[2] = instruction[2];
    dec->imm[3] = instruction[3];
    dec->imm[4] = instruction[4];
    dec->imm[5] = instruction[5];
    dec->imm[6] = instruction[6];
    dec->imm[7] = instruction[7];
    dec->imm[8] = instruction[8];
    dec->imm[9] = instruction[9];
    dec->imm[10] = instruction[10];
    dec->imm[11] = instruction[11];
    dec->imm[12] = instruction[12];
    dec->imm[13] = instruction[13];
    dec->imm[14] = instruction[14];
    dec->imm[15] = instruction[15];

    dec->addr[0] = instruction[0];
    dec->addr[1] = instruction[1];
    dec->addr[2] = instruction[2];
    dec->addr[3] = instruction[3];
    dec->addr[4] = instruction[4];
    dec->addr[5] = instruction[5];
    dec->addr[6] = instruction[6];
    dec->addr[7] = instruction[7];
    dec->addr[8] = instruction[8];
    dec->addr[9] = instruction[9];
    dec->addr[10] = instruction[10];
    dec->addr[11] = instruction[11];
    dec->addr[12] = instruction[12];
    dec->addr[13] = instruction[13];
    dec->addr[14] = instruction[14];
    dec->addr[15] = instruction[15];
    dec->addr[16] = instruction[16];
    dec->addr[17] = instruction[17];
    dec->addr[18] = instruction[18];
    dec->addr[19] = instruction[19];
    dec->addr[20] = instruction[20];
    dec->addr[21] = instruction[21];
    dec->addr[22] = instruction[22];
    dec->addr[23] = instruction[23];
    dec->addr[24] = instruction[24];
    dec->addr[25] = instruction[25];
}

static bit match6(bit *f, bit v5, bit v4, bit v3, bit v2, bit v1, bit v0) {
    return AND(AND(XNOR(f[5], v5), XNOR(f[4], v4)),
               AND(AND(XNOR(f[3], v3), XNOR(f[2], v2)),
                   AND(XNOR(f[1], v1), XNOR(f[0], v0))));
}

void decoder_propagate(instruction_decoder_t *dec) {
    bit *op = dec->opcode;
    bit *fn = dec->funct;

    bit is_r = match6(op, 0, 0, 0, 0, 0, 0);
    bit is_addi = match6(op, 0, 0, 1, 0, 0, 0);
    bit is_slti = match6(op, 0, 0, 1, 0, 1, 0);
    bit is_sltiu = match6(op, 0, 0, 1, 0, 1, 1);
    bit is_andi = match6(op, 0, 0, 1, 1, 0, 0);
    bit is_ori = match6(op, 0, 0, 1, 1, 0, 1);
    bit is_xori = match6(op, 0, 0, 1, 1, 1, 0);
    bit is_lui = match6(op, 0, 0, 1, 1, 1, 1);
    bit is_lw = match6(op, 1, 0, 0, 0, 1, 1);
    bit is_sw = match6(op, 1, 0, 1, 0, 1, 1);
    bit is_beq = match6(op, 0, 0, 0, 1, 0, 0);
    bit is_bne = match6(op, 0, 0, 0, 1, 0, 1);
    bit is_j = match6(op, 0, 0, 0, 0, 1, 0);
    bit is_jal = match6(op, 0, 0, 0, 0, 1, 1);
    bit is_reti = match6(op, 0, 1, 0, 0, 0, 0);
    bit is_halt = match6(op, 1, 1, 1, 1, 1, 1);

    bit f_add = AND(is_r, match6(fn, 1, 0, 0, 0, 0, 0));
    bit f_sub = AND(is_r, match6(fn, 1, 0, 0, 0, 1, 0));
    bit f_and = AND(is_r, match6(fn, 1, 0, 0, 1, 0, 0));
    bit f_or = AND(is_r, match6(fn, 1, 0, 0, 1, 0, 1));
    bit f_xor = AND(is_r, match6(fn, 1, 0, 0, 1, 1, 0));
    bit f_nor = AND(is_r, match6(fn, 1, 0, 0, 1, 1, 1));
    bit f_slt = AND(is_r, match6(fn, 1, 0, 1, 0, 1, 0));
    bit f_sltu = AND(is_r, match6(fn, 1, 0, 1, 0, 1, 1));
    bit f_sll = AND(is_r, match6(fn, 0, 0, 0, 0, 0, 0));
    bit f_srl = AND(is_r, match6(fn, 0, 0, 0, 0, 1, 0));
    bit f_sra = AND(is_r, match6(fn, 0, 0, 0, 0, 1, 1));
    bit f_sllv = AND(is_r, match6(fn, 0, 0, 0, 1, 0, 0));
    bit f_srlv = AND(is_r, match6(fn, 0, 0, 0, 1, 1, 0));
    bit f_srav = AND(is_r, match6(fn, 0, 0, 0, 1, 1, 1));
    bit f_jr = AND(is_r, match6(fn, 0, 0, 1, 0, 0, 0));
    bit f_jalr = AND(is_r, match6(fn, 0, 0, 1, 0, 0, 1));

    bit shift_imm = OR(f_sll, OR(f_srl, f_sra));
    bit shift_var = OR(f_sllv, OR(f_srlv, f_srav));
    bit r_alu = OR(OR(OR(f_add, f_sub), OR(f_and, f_or)),
                   OR(OR(f_xor, f_nor), OR(f_slt, f_sltu)));
    bit r_valid = OR(OR(r_alu, shift_imm), OR(shift_var, OR(f_jr, f_jalr)));
    bit i_signed = OR(is_addi, OR(is_slti, is_sltiu));
    bit i_logic = OR(is_andi, OR(is_ori, is_xori));
    bit mem = OR(is_lw, is_sw);
    bit branch = OR(is_beq, is_bne);

    bit op_sub = OR(f_sub, branch);
    bit op_and = OR(f_and, is_andi);
    bit op_or = OR(f_or, is_ori);
    bit op_xor = OR(f_xor, is_xori);
    bit op_sll = OR(OR(f_sll, f_sllv), is_lui);
    bit op_srl = OR(f_srl, f_srlv);
    bit op_sra = OR(f_sra, f_srav);
    bit op_slt = OR(f_slt, is_slti);
    bit op_sltu = OR(f_sltu, is_sltiu);

    dec->alu_op[0] = OR(OR(op_sub, op_or), OR(OR(op_srl, op_slt), f_nor));
    dec->alu_op[1] = OR(OR(OR(op_and, op_or), OR(op_sll, op_srl)), OR(op_sltu, f_nor));
    dec->alu_op[2] = OR(op_xor, OR(op_sll, op_srl));
    dec->alu_op[3] = OR(OR(op_sra, op_slt), OR(op_sltu, f_nor));

    dec->reg_dst = is_r;
    dec->alu_src = OR(OR(i_signed, i_logic), mem);
    dec->use_imm = dec->alu_src;
    dec->imm_zero_ext = OR(i_logic, is_lui);
    dec->shift_rt = OR(shift_imm, shift_var);
    dec->shift_shamt = shift_imm;
    dec->lui = is_lui;
    dec->mem_to_reg = is_lw;
    dec->mem_read = is_lw;
    dec->mem_write = is_sw;
    dec->branch = branch;
    dec->branch_ne = is_bne;
    dec->jump = OR(is_j, is_jal);
    dec->jal = is_jal;
    dec->jr = OR(f_jr, f_jalr);
    dec->link = OR(is_jal, f_jalr);
    dec->reti = is_reti;
    dec->reg_write = OR(OR(AND(r_valid, NOT(f_jr)), OR(i_signed, i_logic)),
                        OR(OR(is_lui, is_lw), is_jal));

    bit known = OR(OR(OR(r_valid, i_signed), OR(i_logic, is_lui)),
                   OR(OR(mem, branch), OR(OR(is_j, is_jal), OR(is_reti, is_halt))));
    dec->undefined = NOT(known);
}

bit decoder_get_reg_dst(instruction_decoder_t *dec) {
    return dec->reg_dst;
}

bit decoder_get_alu_src(instruction_decoder_t *dec) {
    return dec->alu_src;
}

bit decoder_get_mem_to_reg(instruction_decoder_t *dec) {
    return dec->mem_to_reg;
}

bit decoder_get_reg_write(instruction_decoder_t *dec) {
    return dec->reg_write;
}

bit decoder_get_mem_read(instruction_decoder_t *dec) {
    return dec->mem_read;
}

bit decoder_get_mem_write(instruction_decoder_t *dec) {
    return dec->mem_write;
}

bit decoder_get_branch(instruction_decoder_t *dec) {
    return dec->branch;
}

bit decoder_get_jump(instruction_decoder_t *dec) {
    return dec->jump;
}
