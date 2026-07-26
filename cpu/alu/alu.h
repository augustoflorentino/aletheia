#ifndef ALETHEIA_ALU_H
#define ALETHEIA_ALU_H

#include "../../core/types.h"

#define ALU_OP_ADD  0
#define ALU_OP_SUB  1
#define ALU_OP_AND  2
#define ALU_OP_OR   3
#define ALU_OP_XOR  4
#define ALU_OP_NOT  5
#define ALU_OP_SHL  6
#define ALU_OP_SHR  7
#define ALU_OP_SRA  8
#define ALU_OP_SLT  9
#define ALU_OP_SLTU 10
#define ALU_OP_NOR  11

typedef struct {
    bit zero;
    bit negative;
    bit carry;
    bit overflow;
} alu_flags_t;

void alu_execute(bit *a, bit *b, bit *opcode, bit *result, alu_flags_t *flags);
typedef struct {
    bit alu_src;
    bit imm_zero_ext;
    bit shift_rt;
    bit shift_shamt;
    bit lui;
} alu_operand_sel_t;

void alu_select_operands(const alu_operand_sel_t *sel, bit *imm16, bit *shamt5,
                         bit *rs_val, bit *rt_val, bit *a, bit *b);
void alu_execute_ext(bit *a, bit *b, bit *opcode, bit *result, alu_flags_t *flags);

#include "adder.h"
#include "subtractor.h"
#include "multiplier.h"
#include "divider.h"
#include "shifter.h"
#include "comparator.h"
#include "logic_unit.h"

#endif
