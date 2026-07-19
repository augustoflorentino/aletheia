#include "disassembler.h"

static void a_puts(char **o, const char *s) {
    while (*s) { **o = *s; (*o)++; s++; }
}
static void a_putc(char **o, char c) {
    **o = c; (*o)++;
}
static void a_dec(char **o, int v) {
    char tmp[12];
    int n = 0;
    unsigned int u;
    if (v < 0) { a_putc(o, '-'); u = (unsigned int)(-(long)v); }
    else u = (unsigned int)v;
    if (u == 0) { a_putc(o, '0'); return; }
    while (u > 0) { tmp[n++] = (char)('0' + (u % 10u)); u /= 10u; }
    while (n > 0) { n--; a_putc(o, tmp[n]); }
}
static void a_hex(char **o, unsigned int v, int digits) {
    static const char hx[] = "0123456789abcdef";
    int i;
    for (i = digits - 1; i >= 0; i--) a_putc(o, hx[(v >> (i * 4)) & 0xF]);
}

static const char *reg_names[32] = {
    "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
};

const char* get_register_name(int reg) {
    if (reg >= 0 && reg < 32) {
        return reg_names[reg];
    }
    return "???";
}

const char* get_opcode_name(unsigned int opcode, unsigned int funct) {
    if (opcode == OP_RTYPE) {
        switch (funct) {
            case FUNCT_ADD: return "add";
            case FUNCT_ADDU: return "addu";
            case FUNCT_SUB: return "sub";
            case FUNCT_SUBU: return "subu";
            case FUNCT_AND: return "and";
            case FUNCT_OR: return "or";
            case FUNCT_XOR: return "xor";
            case FUNCT_NOR: return "nor";
            case FUNCT_SLT: return "slt";
            case FUNCT_SLTU: return "sltu";
            case FUNCT_SLL: return "sll";
            case FUNCT_SRL: return "srl";
            case FUNCT_SRA: return "sra";
            case FUNCT_SLLV: return "sllv";
            case FUNCT_SRLV: return "srlv";
            case FUNCT_SRAV: return "srav";
            case FUNCT_JR: return "jr";
            case FUNCT_JALR: return "jalr";
            case FUNCT_MULT: return "mult";
            case FUNCT_MULTU: return "multu";
            case FUNCT_DIV: return "div";
            case FUNCT_DIVU: return "divu";
            case FUNCT_MFHI: return "mfhi";
            case FUNCT_MFLO: return "mflo";
            default: return "???";
        }
    }

    switch (opcode) {
        case OP_ADDI: return "addi";
        case OP_ANDI: return "andi";
        case OP_ORI: return "ori";
        case OP_XORI: return "xori";
        case OP_SLTI: return "slti";
        case OP_LW: return "lw";
        case OP_SW: return "sw";
        case OP_LB: return "lb";
        case OP_SB: return "sb";
        case OP_BEQ: return "beq";
        case OP_BNE: return "bne";
        case OP_J: return "j";
        case OP_JAL: return "jal";
        case OP_LUI: return "lui";
        case OP_RETI: return "reti";
        case OP_HALT: return "halt";
        default: return "???";
    }
}

void disassemble_instruction(unsigned int word, char *output, int max_len) {
    instruction_t instr;
    decode_instruction(word, &instr);
    (void)max_len;

    const char *name = get_opcode_name(instr.opcode, instr.funct);
    char *o = output;

    if (word == 0) {
        a_puts(&o, "nop");
        *o = '\0';
        return;
    }

    if (instr.opcode == OP_RETI || instr.opcode == OP_HALT) {
        a_puts(&o, name);
        *o = '\0';
        return;
    }

    switch (instr.type) {
        case INSTR_TYPE_R:
            if (instr.funct == FUNCT_JR) {
                a_puts(&o, name); a_putc(&o, ' ');
                a_puts(&o, get_register_name(instr.rs));
            } else if (instr.funct == FUNCT_JALR) {
                a_puts(&o, name); a_putc(&o, ' ');
                a_puts(&o, get_register_name(instr.rd)); a_puts(&o, ", ");
                a_puts(&o, get_register_name(instr.rs));
            } else if (instr.funct == FUNCT_SLL || instr.funct == FUNCT_SRL || instr.funct == FUNCT_SRA) {
                a_puts(&o, name); a_putc(&o, ' ');
                a_puts(&o, get_register_name(instr.rd)); a_puts(&o, ", ");
                a_puts(&o, get_register_name(instr.rt)); a_puts(&o, ", ");
                a_dec(&o, (int)instr.shamt);
            } else if (instr.funct == FUNCT_MFHI || instr.funct == FUNCT_MFLO) {
                a_puts(&o, name); a_putc(&o, ' ');
                a_puts(&o, get_register_name(instr.rd));
            } else if (instr.funct == FUNCT_MULT || instr.funct == FUNCT_MULTU ||
                       instr.funct == FUNCT_DIV || instr.funct == FUNCT_DIVU) {
                a_puts(&o, name); a_putc(&o, ' ');
                a_puts(&o, get_register_name(instr.rs)); a_puts(&o, ", ");
                a_puts(&o, get_register_name(instr.rt));
            } else {
                a_puts(&o, name); a_putc(&o, ' ');
                a_puts(&o, get_register_name(instr.rd)); a_puts(&o, ", ");
                a_puts(&o, get_register_name(instr.rs)); a_puts(&o, ", ");
                a_puts(&o, get_register_name(instr.rt));
            }
            break;

        case INSTR_TYPE_I:
            if (instr.opcode == OP_LW || instr.opcode == OP_SW ||
                instr.opcode == OP_LB || instr.opcode == OP_SB) {
                a_puts(&o, name); a_putc(&o, ' ');
                a_puts(&o, get_register_name(instr.rt)); a_puts(&o, ", ");
                a_dec(&o, sign_extend_16(instr.immediate)); a_putc(&o, '(');
                a_puts(&o, get_register_name(instr.rs)); a_putc(&o, ')');
            } else if (instr.opcode == OP_BEQ || instr.opcode == OP_BNE) {
                a_puts(&o, name); a_putc(&o, ' ');
                a_puts(&o, get_register_name(instr.rs)); a_puts(&o, ", ");
                a_puts(&o, get_register_name(instr.rt)); a_puts(&o, ", ");
                a_dec(&o, sign_extend_16(instr.immediate));
            } else if (instr.opcode == OP_LUI) {
                a_puts(&o, name); a_putc(&o, ' ');
                a_puts(&o, get_register_name(instr.rt)); a_puts(&o, ", 0x");
                a_hex(&o, instr.immediate, 4);
            } else {
                a_puts(&o, name); a_putc(&o, ' ');
                a_puts(&o, get_register_name(instr.rt)); a_puts(&o, ", ");
                a_puts(&o, get_register_name(instr.rs)); a_puts(&o, ", ");
                a_dec(&o, sign_extend_16(instr.immediate));
            }
            break;

        case INSTR_TYPE_J:
            a_puts(&o, name); a_puts(&o, " 0x");
            a_hex(&o, instr.address << 2, 8);
            break;

        default:
            a_puts(&o, ".word 0x");
            a_hex(&o, word, 8);
            break;
    }
    *o = '\0';
}
