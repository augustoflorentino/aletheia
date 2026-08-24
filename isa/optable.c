#include "optable.h"
#include "opcodes.h"

static const op_info_t ops[] = {
    {"add", OP_RTYPE, FUNCT_ADD, FMT_R3},
    {"sub", OP_RTYPE, FUNCT_SUB, FMT_R3},
    {"and", OP_RTYPE, FUNCT_AND, FMT_R3},
    {"or", OP_RTYPE, FUNCT_OR, FMT_R3},
    {"xor", OP_RTYPE, FUNCT_XOR, FMT_R3},
    {"nor", OP_RTYPE, FUNCT_NOR, FMT_R3},
    {"slt", OP_RTYPE, FUNCT_SLT, FMT_R3},
    {"sltu", OP_RTYPE, FUNCT_SLTU, FMT_R3},
    {"sll", OP_RTYPE, FUNCT_SLL, FMT_SHIFT},
    {"srl", OP_RTYPE, FUNCT_SRL, FMT_SHIFT},
    {"sra", OP_RTYPE, FUNCT_SRA, FMT_SHIFT},
    {"sllv", OP_RTYPE, FUNCT_SLLV, FMT_SHIFTV},
    {"srlv", OP_RTYPE, FUNCT_SRLV, FMT_SHIFTV},
    {"srav", OP_RTYPE, FUNCT_SRAV, FMT_SHIFTV},
    {"jr", OP_RTYPE, FUNCT_JR, FMT_JR},
    {"jalr", OP_RTYPE, FUNCT_JALR, FMT_JALR},
    {"addi", OP_ADDI, 0, FMT_IMM_S},
    {"slti", OP_SLTI, 0, FMT_IMM_S},
    {"sltiu", OP_SLTIU, 0, FMT_IMM_S},
    {"andi", OP_ANDI, 0, FMT_IMM_U},
    {"ori", OP_ORI, 0, FMT_IMM_U},
    {"xori", OP_XORI, 0, FMT_IMM_U},
    {"lui", OP_LUI, 0, FMT_LUI},
    {"lw", OP_LW, 0, FMT_MEM},
    {"sw", OP_SW, 0, FMT_MEM},
    {"beq", OP_BEQ, 0, FMT_BRANCH},
    {"bne", OP_BNE, 0, FMT_BRANCH},
    {"j", OP_J, 0, FMT_JUMP},
    {"jal", OP_JAL, 0, FMT_JUMP},
    {"reti", OP_RETI, 0, FMT_NONE},
    {"halt", OP_HALT, 0, FMT_NONE},
};

#define OP_COUNT ((int)(sizeof(ops) / sizeof(ops[0])))

static int same_name(const char *a, const char *b) {
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}

const op_info_t *optable_find_name(const char *name) {
    for (int i = 0; i < OP_COUNT; i++) {
        if (same_name(ops[i].name, name)) return &ops[i];
    }
    return 0;
}

const op_info_t *optable_find_word(unsigned int word) {
    unsigned int opcode = (word >> 26) & 0x3F;
    unsigned int funct = word & 0x3F;
    for (int i = 0; i < OP_COUNT; i++) {
        if (ops[i].opcode != opcode) continue;
        if (opcode == OP_RTYPE && ops[i].funct != funct) continue;
        return &ops[i];
    }
    return 0;
}
