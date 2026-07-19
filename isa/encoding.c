#include "encoding.h"

unsigned int encode_r_type(unsigned int rs, unsigned int rt, unsigned int rd, unsigned int shamt, unsigned int funct) {
    unsigned int word = 0;
    word |= (rs & 0x1F) << 21;
    word |= (rt & 0x1F) << 16;
    word |= (rd & 0x1F) << 11;
    word |= (shamt & 0x1F) << 6;
    word |= (funct & 0x3F);
    return word;
}

unsigned int encode_i_type(unsigned int opcode, unsigned int rs, unsigned int rt, unsigned int immediate) {
    unsigned int word = 0;
    word |= (opcode & 0x3F) << 26;
    word |= (rs & 0x1F) << 21;
    word |= (rt & 0x1F) << 16;
    word |= (immediate & 0xFFFF);
    return word;
}

unsigned int encode_j_type(unsigned int opcode, unsigned int address) {
    unsigned int word = 0;
    word |= (opcode & 0x3F) << 26;
    word |= (address & 0x3FFFFFF);
    return word;
}

unsigned int extract_opcode(unsigned int word) {
    return (word >> 26) & 0x3F;
}

unsigned int extract_rs(unsigned int word) {
    return (word >> 21) & 0x1F;
}

unsigned int extract_rt(unsigned int word) {
    return (word >> 16) & 0x1F;
}

unsigned int extract_rd(unsigned int word) {
    return (word >> 11) & 0x1F;
}

unsigned int extract_shamt(unsigned int word) {
    return (word >> 6) & 0x1F;
}

unsigned int extract_funct(unsigned int word) {
    return word & 0x3F;
}

unsigned int extract_immediate(unsigned int word) {
    return word & 0xFFFF;
}

unsigned int extract_address(unsigned int word) {
    return word & 0x3FFFFFF;
}

instr_type_t get_instruction_type(unsigned int opcode) {
    if (opcode == OP_RTYPE) {
        return INSTR_TYPE_R;
    }
    if (opcode == OP_J || opcode == OP_JAL) {
        return INSTR_TYPE_J;
    }
    if (opcode == OP_ADDI || opcode == OP_ANDI || opcode == OP_ORI ||
        opcode == OP_XORI || opcode == OP_SLTI || opcode == OP_LW ||
        opcode == OP_SW || opcode == OP_LB || opcode == OP_SB ||
        opcode == OP_BEQ || opcode == OP_BNE || opcode == OP_LUI) {
        return INSTR_TYPE_I;
    }
    return INSTR_TYPE_UNKNOWN;
}

void decode_instruction(unsigned int word, instruction_t *instr) {
    instr->opcode = extract_opcode(word);
    instr->rs = extract_rs(word);
    instr->rt = extract_rt(word);
    instr->rd = extract_rd(word);
    instr->shamt = extract_shamt(word);
    instr->funct = extract_funct(word);
    instr->immediate = extract_immediate(word);
    instr->address = extract_address(word);
    instr->type = get_instruction_type(instr->opcode);
}

int sign_extend_16(unsigned int value) {
    if (value & 0x8000) {
        return (int)(value | 0xFFFF0000);
    }
    return (int)value;
}

int sign_extend_26(unsigned int value) {
    if (value & 0x2000000) {
        return (int)(value | 0xFC000000);
    }
    return (int)value;
}
