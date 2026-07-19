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

void decoder_propagate(instruction_decoder_t *dec) {
    bit op_all_zero = AND(NOT(dec->opcode[0]), AND(NOT(dec->opcode[1]),
                      AND(NOT(dec->opcode[2]), AND(NOT(dec->opcode[3]),
                      AND(NOT(dec->opcode[4]), NOT(dec->opcode[5]))))));

    bit is_r_type = op_all_zero;

    bit is_lw = AND(AND(dec->opcode[5], NOT(dec->opcode[4])),
                AND(NOT(dec->opcode[3]), AND(NOT(dec->opcode[2]),
                AND(dec->opcode[1], dec->opcode[0]))));

    bit is_sw = AND(AND(dec->opcode[5], NOT(dec->opcode[4])),
                AND(dec->opcode[3], AND(NOT(dec->opcode[2]),
                AND(dec->opcode[1], dec->opcode[0]))));

    bit is_beq = AND(NOT(dec->opcode[5]), AND(NOT(dec->opcode[4]),
                 AND(NOT(dec->opcode[3]), AND(dec->opcode[2],
                 AND(NOT(dec->opcode[1]), NOT(dec->opcode[0]))))));

    bit is_j = AND(NOT(dec->opcode[5]), AND(NOT(dec->opcode[4]),
               AND(NOT(dec->opcode[3]), AND(NOT(dec->opcode[2]),
               AND(dec->opcode[1], NOT(dec->opcode[0]))))));

    bit is_addi = AND(NOT(dec->opcode[5]), AND(NOT(dec->opcode[4]),
                  AND(dec->opcode[3], AND(NOT(dec->opcode[2]),
                  AND(NOT(dec->opcode[1]), NOT(dec->opcode[0]))))));

    bit is_jal = AND(NOT(dec->opcode[5]), AND(NOT(dec->opcode[4]),
                 AND(NOT(dec->opcode[3]), AND(NOT(dec->opcode[2]),
                 AND(dec->opcode[1], dec->opcode[0])))));

    bit funct_jr = AND(NOT(dec->funct[5]), AND(NOT(dec->funct[4]),
                   AND(dec->funct[3], AND(NOT(dec->funct[2]),
                   AND(NOT(dec->funct[1]), NOT(dec->funct[0]))))));
    bit is_jr = AND(is_r_type, funct_jr);

    bit is_reti = AND(NOT(dec->opcode[5]), AND(dec->opcode[4],
                  AND(NOT(dec->opcode[3]), AND(NOT(dec->opcode[2]),
                  AND(NOT(dec->opcode[1]), NOT(dec->opcode[0]))))));

    dec->jal = is_jal;
    dec->jr = is_jr;
    dec->reti = is_reti;

    dec->reg_dst = is_r_type;
    dec->alu_src = OR(is_lw, OR(is_sw, is_addi));
    dec->mem_to_reg = is_lw;

    dec->reg_write = OR(AND(OR(is_r_type, OR(is_lw, is_addi)), NOT(is_jr)), is_jal);
    dec->mem_read = is_lw;
    dec->mem_write = is_sw;
    dec->branch = is_beq;
    dec->jump = OR(is_j, is_jal);
    dec->use_imm = OR(is_lw, OR(is_sw, is_addi));

    bit funct_add = AND(dec->funct[5], AND(NOT(dec->funct[4]),
                    AND(NOT(dec->funct[3]), AND(NOT(dec->funct[2]),
                    AND(NOT(dec->funct[1]), NOT(dec->funct[0]))))));

    bit funct_sub = AND(dec->funct[5], AND(NOT(dec->funct[4]),
                    AND(NOT(dec->funct[3]), AND(NOT(dec->funct[2]),
                    AND(dec->funct[1], NOT(dec->funct[0]))))));

    bit funct_and = AND(dec->funct[5], AND(NOT(dec->funct[4]),
                    AND(NOT(dec->funct[3]), AND(dec->funct[2],
                    AND(NOT(dec->funct[1]), NOT(dec->funct[0]))))));

    bit funct_or = AND(dec->funct[5], AND(NOT(dec->funct[4]),
                   AND(NOT(dec->funct[3]), AND(dec->funct[2],
                   AND(NOT(dec->funct[1]), dec->funct[0])))));

    bit funct_slt = AND(dec->funct[5], AND(NOT(dec->funct[4]),
                    AND(dec->funct[3], AND(NOT(dec->funct[2]),
                    AND(dec->funct[1], NOT(dec->funct[0]))))));

    dec->alu_op[0] = mux2(0, OR(funct_sub, funct_slt), is_r_type);
    dec->alu_op[1] = mux2(0, OR(funct_and, funct_or), is_r_type);
    dec->alu_op[2] = mux2(0, funct_or, is_r_type);
    dec->alu_op[3] = mux2(0, funct_slt, is_r_type);

    (void)funct_add;
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
