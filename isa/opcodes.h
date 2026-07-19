#ifndef ALETHEIA_OPCODES_H
#define ALETHEIA_OPCODES_H

#define OP_RTYPE    0x00
#define OP_ADDI     0x08
#define OP_ANDI     0x0C
#define OP_ORI      0x0D
#define OP_XORI     0x0E
#define OP_SLTI     0x0A
#define OP_LW       0x23
#define OP_SW       0x2B
#define OP_LB       0x20
#define OP_SB       0x28
#define OP_BEQ      0x04
#define OP_BNE      0x05
#define OP_J        0x02
#define OP_JAL      0x03
#define OP_LUI      0x0F
#define OP_RETI     0x10
#define OP_HALT     0x3F

#define FUNCT_ADD   0x20
#define FUNCT_ADDU  0x21
#define FUNCT_SUB   0x22
#define FUNCT_SUBU  0x23
#define FUNCT_AND   0x24
#define FUNCT_OR    0x25
#define FUNCT_XOR   0x26
#define FUNCT_NOR   0x27
#define FUNCT_SLT   0x2A
#define FUNCT_SLTU  0x2B
#define FUNCT_SLL   0x00
#define FUNCT_SRL   0x02
#define FUNCT_SRA   0x03
#define FUNCT_SLLV  0x04
#define FUNCT_SRLV  0x06
#define FUNCT_SRAV  0x07
#define FUNCT_JR    0x08
#define FUNCT_JALR  0x09
#define FUNCT_MULT  0x18
#define FUNCT_MULTU 0x19
#define FUNCT_DIV   0x1A
#define FUNCT_DIVU  0x1B
#define FUNCT_MFHI  0x10
#define FUNCT_MFLO  0x12
#define FUNCT_MTHI  0x11
#define FUNCT_MTLO  0x13

#define REG_ZERO    0
#define REG_AT      1
#define REG_V0      2
#define REG_V1      3
#define REG_A0      4
#define REG_A1      5
#define REG_A2      6
#define REG_A3      7
#define REG_T0      8
#define REG_T1      9
#define REG_T2      10
#define REG_T3      11
#define REG_T4      12
#define REG_T5      13
#define REG_T6      14
#define REG_T7      15
#define REG_S0      16
#define REG_S1      17
#define REG_S2      18
#define REG_S3      19
#define REG_S4      20
#define REG_S5      21
#define REG_S6      22
#define REG_S7      23
#define REG_T8      24
#define REG_T9      25
#define REG_K0      26
#define REG_K1      27
#define REG_GP      28
#define REG_SP      29
#define REG_FP      30
#define REG_RA      31

typedef enum {
    INSTR_TYPE_R,
    INSTR_TYPE_I,
    INSTR_TYPE_J,
    INSTR_TYPE_UNKNOWN
} instr_type_t;

typedef struct {
    unsigned int opcode;
    unsigned int rs;
    unsigned int rt;
    unsigned int rd;
    unsigned int shamt;
    unsigned int funct;
    unsigned int immediate;
    unsigned int address;
    instr_type_t type;
} instruction_t;

#endif
