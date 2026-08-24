#include "disassembler.h"
#include "optable.h"

typedef struct {
    char *pos;
    char *end;
} out_t;

static void put_char(out_t *o, char c) {
    if (o->pos < o->end) *o->pos++ = c;
}

static void put_str(out_t *o, const char *s) {
    while (*s) put_char(o, *s++);
}

static void put_dec(out_t *o, int v) {
    char tmp[12];
    int n = 0;
    unsigned int u = (unsigned int)v;
    if (v < 0) {
        put_char(o, '-');
        u = 0u - u;
    }
    do {
        tmp[n++] = (char)('0' + (u % 10u));
        u /= 10u;
    } while (u > 0);
    while (n > 0) put_char(o, tmp[--n]);
}

static void put_hex(out_t *o, unsigned int v) {
    static const char hx[] = "0123456789abcdef";
    int digits = 1;
    while (digits < 8 && (v >> (digits * 4))) digits++;
    put_str(o, "0x");
    for (int i = digits - 1; i >= 0; i--) put_char(o, hx[(v >> (i * 4)) & 0xF]);
}

static const char *reg_names[32] = {
    "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};

const char *get_register_name(int reg) {
    if (reg >= 0 && reg < 32) return reg_names[reg];
    return "???";
}

static void put_regs(out_t *o, unsigned int a, unsigned int b) {
    put_str(o, reg_names[a]);
    put_str(o, ", ");
    put_str(o, reg_names[b]);
}

static void put_operands(out_t *o, const op_info_t *op, const instruction_t *in) {
    switch (op->format) {
        case FMT_R3:
            put_regs(o, in->rd, in->rs);
            put_str(o, ", ");
            put_str(o, reg_names[in->rt]);
            break;
        case FMT_SHIFT:
            put_regs(o, in->rd, in->rt);
            put_str(o, ", ");
            put_dec(o, (int)in->shamt);
            break;
        case FMT_SHIFTV:
            put_regs(o, in->rd, in->rt);
            put_str(o, ", ");
            put_str(o, reg_names[in->rs]);
            break;
        case FMT_JR: put_str(o, reg_names[in->rs]); break;
        case FMT_JALR: put_regs(o, in->rd, in->rs); break;
        case FMT_IMM_S:
            put_regs(o, in->rt, in->rs);
            put_str(o, ", ");
            put_dec(o, sign_extend_16(in->immediate));
            break;
        case FMT_IMM_U:
            put_regs(o, in->rt, in->rs);
            put_str(o, ", ");
            put_hex(o, in->immediate);
            break;
        case FMT_LUI:
            put_str(o, reg_names[in->rt]);
            put_str(o, ", ");
            put_hex(o, in->immediate);
            break;
        case FMT_BRANCH:
            put_regs(o, in->rs, in->rt);
            put_str(o, ", ");
            put_dec(o, sign_extend_16(in->immediate));
            break;
        case FMT_JUMP: put_hex(o, in->address); break;
        case FMT_MEM:
            put_str(o, reg_names[in->rt]);
            put_str(o, ", ");
            put_dec(o, sign_extend_16(in->immediate));
            put_char(o, '(');
            put_str(o, reg_names[in->rs]);
            put_char(o, ')');
            break;
        default: break;
    }
}

void disassemble_instruction(unsigned int word, char *output, int max_len) {
    if (max_len <= 0) return;
    out_t o = {output, output + max_len - 1};
    const op_info_t *op = optable_find_word(word);

    if (word == 0) {
        put_str(&o, "nop");
    } else if (!op) {
        put_str(&o, ".word ");
        put_hex(&o, word);
    } else {
        instruction_t in;
        decode_instruction(word, &in);
        put_str(&o, op->name);
        if (op->format != FMT_NONE) put_char(&o, ' ');
        put_operands(&o, op, &in);
    }
    *o.pos = '\0';
}
