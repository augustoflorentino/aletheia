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

typedef struct {
    bit zero;
    bit negative;
    bit carry;
    bit overflow;
} alu_flags_t;

void alu_execute(bit *a, bit *b, bit *opcode, bit *result, alu_flags_t *flags);

#include "adder.h"
#include "subtractor.h"
#include "multiplier.h"
#include "divider.h"
#include "shifter.h"
#include "comparator.h"
#include "logic_unit.h"

#endif
