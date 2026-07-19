#include "assembler.h"

static int a_strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) { a++; b++; }
    return (int)(unsigned char)*a - (int)(unsigned char)*b;
}
static void a_strncpy(char *d, const char *s, int n) {
    int i = 0;
    while (i < n && s[i]) { d[i] = s[i]; i++; }
    while (i < n) { d[i] = '\0'; i++; }
}
static char *a_strchr(char *s, char c) {
    while (*s) { if (*s == c) return s; s++; }
    if (c == '\0') return s;
    return 0;
}
static char a_tolower(char c) {
    if (c >= 'A' && c <= 'Z') return (char)(c - 'A' + 'a');
    return c;
}
static int a_parse_int(const char *str) {
    while (*str == ' ' || *str == '\t') str++;
    int neg = 0;
    if (*str == '-') { neg = 1; str++; }
    int val = 0;
    if (str[0] == '0' && str[1] == 'x') {
        str += 2;
        while (*str) {
            char c = *str; int d;
            if (c >= '0' && c <= '9') d = c - '0';
            else if (c >= 'a' && c <= 'f') d = c - 'a' + 10;
            else if (c >= 'A' && c <= 'F') d = c - 'A' + 10;
            else break;
            val = val * 16 + d; str++;
        }
    } else {
        while (*str >= '0' && *str <= '9') { val = val * 10 + (*str - '0'); str++; }
    }
    return neg ? -val : val;
}

void assembler_init(assembler_t *as) {
    as->label_count = 0;
    as->current_address = 0;
    as->origin = 0;
}

void assembler_reset(assembler_t *as) {
    as->label_count = 0;
    as->current_address = 0;
}

void assembler_set_origin(assembler_t *as, unsigned int origin) {
    as->origin = origin;
    as->current_address = origin;
}

int assembler_add_label(assembler_t *as, const char *name, unsigned int address) {
    if (as->label_count >= MAX_LABELS) {
        return -1;
    }
    a_strncpy(as->labels[as->label_count].name, name, MAX_LABEL_NAME - 1);
    as->labels[as->label_count].name[MAX_LABEL_NAME - 1] = '\0';
    as->labels[as->label_count].address = address;
    as->label_count++;
    return as->label_count - 1;
}

int assembler_find_label(assembler_t *as, const char *name, unsigned int *address) {
    for (int i = 0; i < as->label_count; i++) {
        if (a_strcmp(as->labels[i].name, name) == 0) {
            *address = as->labels[i].address;
            return 1;
        }
    }
    return 0;
}

int assembler_parse_register(const char *str) {
    while (*str == ' ' || *str == '\t') str++;

    if (*str == '$') str++;

    if (a_strcmp(str, "zero") == 0 || a_strcmp(str, "0") == 0) return REG_ZERO;
    if (a_strcmp(str, "at") == 0 || a_strcmp(str, "1") == 0) return REG_AT;
    if (a_strcmp(str, "v0") == 0 || a_strcmp(str, "2") == 0) return REG_V0;
    if (a_strcmp(str, "v1") == 0 || a_strcmp(str, "3") == 0) return REG_V1;
    if (a_strcmp(str, "a0") == 0 || a_strcmp(str, "4") == 0) return REG_A0;
    if (a_strcmp(str, "a1") == 0 || a_strcmp(str, "5") == 0) return REG_A1;
    if (a_strcmp(str, "a2") == 0 || a_strcmp(str, "6") == 0) return REG_A2;
    if (a_strcmp(str, "a3") == 0 || a_strcmp(str, "7") == 0) return REG_A3;
    if (a_strcmp(str, "t0") == 0 || a_strcmp(str, "8") == 0) return REG_T0;
    if (a_strcmp(str, "t1") == 0 || a_strcmp(str, "9") == 0) return REG_T1;
    if (a_strcmp(str, "t2") == 0 || a_strcmp(str, "10") == 0) return REG_T2;
    if (a_strcmp(str, "t3") == 0 || a_strcmp(str, "11") == 0) return REG_T3;
    if (a_strcmp(str, "t4") == 0 || a_strcmp(str, "12") == 0) return REG_T4;
    if (a_strcmp(str, "t5") == 0 || a_strcmp(str, "13") == 0) return REG_T5;
    if (a_strcmp(str, "t6") == 0 || a_strcmp(str, "14") == 0) return REG_T6;
    if (a_strcmp(str, "t7") == 0 || a_strcmp(str, "15") == 0) return REG_T7;
    if (a_strcmp(str, "s0") == 0 || a_strcmp(str, "16") == 0) return REG_S0;
    if (a_strcmp(str, "s1") == 0 || a_strcmp(str, "17") == 0) return REG_S1;
    if (a_strcmp(str, "s2") == 0 || a_strcmp(str, "18") == 0) return REG_S2;
    if (a_strcmp(str, "s3") == 0 || a_strcmp(str, "19") == 0) return REG_S3;
    if (a_strcmp(str, "s4") == 0 || a_strcmp(str, "20") == 0) return REG_S4;
    if (a_strcmp(str, "s5") == 0 || a_strcmp(str, "21") == 0) return REG_S5;
    if (a_strcmp(str, "s6") == 0 || a_strcmp(str, "22") == 0) return REG_S6;
    if (a_strcmp(str, "s7") == 0 || a_strcmp(str, "23") == 0) return REG_S7;
    if (a_strcmp(str, "t8") == 0 || a_strcmp(str, "24") == 0) return REG_T8;
    if (a_strcmp(str, "t9") == 0 || a_strcmp(str, "25") == 0) return REG_T9;
    if (a_strcmp(str, "k0") == 0 || a_strcmp(str, "26") == 0) return REG_K0;
    if (a_strcmp(str, "k1") == 0 || a_strcmp(str, "27") == 0) return REG_K1;
    if (a_strcmp(str, "gp") == 0 || a_strcmp(str, "28") == 0) return REG_GP;
    if (a_strcmp(str, "sp") == 0 || a_strcmp(str, "29") == 0) return REG_SP;
    if (a_strcmp(str, "fp") == 0 || a_strcmp(str, "30") == 0) return REG_FP;
    if (a_strcmp(str, "ra") == 0 || a_strcmp(str, "31") == 0) return REG_RA;

    return -1;
}

int assembler_parse_immediate(const char *str) {
    return a_parse_int(str);
}

int assembler_parse_address(const char *str) {
    return assembler_parse_immediate(str);
}

static char* skip_whitespace(char *str) {
    while (*str == ' ' || *str == '\t') str++;
    return str;
}

static char* get_token(char *str, char *token, int max_len) {
    str = skip_whitespace(str);
    int i = 0;
    while (*str && *str != ' ' && *str != '\t' && *str != ',' && *str != '(' && *str != ')' && *str != '\n' && *str != '\r' && i < max_len - 1) {
        token[i++] = *str++;
    }
    token[i] = '\0';
    return str;
}

unsigned int assembler_assemble_line(assembler_t *as, const char *line) {
    (void)as;
    char buf[MAX_LINE_LEN];
    a_strncpy(buf, line, MAX_LINE_LEN - 1);
    buf[MAX_LINE_LEN - 1] = '\0';

    char *p = buf;
    while (*p == ' ' || *p == '\t') p++;

    if (*p == '#' || *p == '\0' || *p == '\n') {
        return 0;
    }

    char *comment = a_strchr(p, '#');
    if (comment) *comment = '\0';

    char mnemonic[32];
    p = get_token(p, mnemonic, 32);

    for (int i = 0; mnemonic[i]; i++) {
        mnemonic[i] = a_tolower(mnemonic[i]);
    }

    char op1[32], op2[32], op3[32];
    op1[0] = op2[0] = op3[0] = '\0';

    if (*p == ',') p++;
    p = get_token(p, op1, 32);
    if (*p == ',') p++;
    p = get_token(p, op2, 32);
    if (*p == ',') p++;
    if (*p == '(') p++;
    p = get_token(p, op3, 32);

    int rd, rs, rt, shamt, imm;

    if (a_strcmp(mnemonic, "add") == 0) {
        rd = assembler_parse_register(op1);
        rs = assembler_parse_register(op2);
        rt = assembler_parse_register(op3);
        return encode_r_type(rs, rt, rd, 0, FUNCT_ADD);
    }
    if (a_strcmp(mnemonic, "sub") == 0) {
        rd = assembler_parse_register(op1);
        rs = assembler_parse_register(op2);
        rt = assembler_parse_register(op3);
        return encode_r_type(rs, rt, rd, 0, FUNCT_SUB);
    }
    if (a_strcmp(mnemonic, "and") == 0) {
        rd = assembler_parse_register(op1);
        rs = assembler_parse_register(op2);
        rt = assembler_parse_register(op3);
        return encode_r_type(rs, rt, rd, 0, FUNCT_AND);
    }
    if (a_strcmp(mnemonic, "or") == 0) {
        rd = assembler_parse_register(op1);
        rs = assembler_parse_register(op2);
        rt = assembler_parse_register(op3);
        return encode_r_type(rs, rt, rd, 0, FUNCT_OR);
    }
    if (a_strcmp(mnemonic, "xor") == 0) {
        rd = assembler_parse_register(op1);
        rs = assembler_parse_register(op2);
        rt = assembler_parse_register(op3);
        return encode_r_type(rs, rt, rd, 0, FUNCT_XOR);
    }
    if (a_strcmp(mnemonic, "slt") == 0) {
        rd = assembler_parse_register(op1);
        rs = assembler_parse_register(op2);
        rt = assembler_parse_register(op3);
        return encode_r_type(rs, rt, rd, 0, FUNCT_SLT);
    }
    if (a_strcmp(mnemonic, "sll") == 0) {
        rd = assembler_parse_register(op1);
        rt = assembler_parse_register(op2);
        shamt = assembler_parse_immediate(op3);
        return encode_r_type(0, rt, rd, shamt, FUNCT_SLL);
    }
    if (a_strcmp(mnemonic, "srl") == 0) {
        rd = assembler_parse_register(op1);
        rt = assembler_parse_register(op2);
        shamt = assembler_parse_immediate(op3);
        return encode_r_type(0, rt, rd, shamt, FUNCT_SRL);
    }
    if (a_strcmp(mnemonic, "jr") == 0) {
        rs = assembler_parse_register(op1);
        return encode_r_type(rs, 0, 0, 0, FUNCT_JR);
    }
    if (a_strcmp(mnemonic, "addi") == 0) {
        rt = assembler_parse_register(op1);
        rs = assembler_parse_register(op2);
        imm = assembler_parse_immediate(op3);
        return encode_i_type(OP_ADDI, rs, rt, imm & 0xFFFF);
    }
    if (a_strcmp(mnemonic, "andi") == 0) {
        rt = assembler_parse_register(op1);
        rs = assembler_parse_register(op2);
        imm = assembler_parse_immediate(op3);
        return encode_i_type(OP_ANDI, rs, rt, imm & 0xFFFF);
    }
    if (a_strcmp(mnemonic, "ori") == 0) {
        rt = assembler_parse_register(op1);
        rs = assembler_parse_register(op2);
        imm = assembler_parse_immediate(op3);
        return encode_i_type(OP_ORI, rs, rt, imm & 0xFFFF);
    }
    if (a_strcmp(mnemonic, "lui") == 0) {
        rt = assembler_parse_register(op1);
        imm = assembler_parse_immediate(op2);
        return encode_i_type(OP_LUI, 0, rt, imm & 0xFFFF);
    }
    if (a_strcmp(mnemonic, "lw") == 0) {
        rt = assembler_parse_register(op1);
        imm = assembler_parse_immediate(op2);
        rs = op3[0] ? assembler_parse_register(op3) : 0;
        return encode_i_type(OP_LW, rs, rt, imm & 0xFFFF);
    }
    if (a_strcmp(mnemonic, "sw") == 0) {
        rt = assembler_parse_register(op1);
        imm = assembler_parse_immediate(op2);
        rs = op3[0] ? assembler_parse_register(op3) : 0;
        return encode_i_type(OP_SW, rs, rt, imm & 0xFFFF);
    }
    if (a_strcmp(mnemonic, "beq") == 0) {
        rs = assembler_parse_register(op1);
        rt = assembler_parse_register(op2);
        imm = assembler_parse_immediate(op3);
        return encode_i_type(OP_BEQ, rs, rt, imm & 0xFFFF);
    }
    if (a_strcmp(mnemonic, "bne") == 0) {
        rs = assembler_parse_register(op1);
        rt = assembler_parse_register(op2);
        imm = assembler_parse_immediate(op3);
        return encode_i_type(OP_BNE, rs, rt, imm & 0xFFFF);
    }
    if (a_strcmp(mnemonic, "j") == 0) {
        unsigned int addr = assembler_parse_address(op1);
        return encode_j_type(OP_J, (addr >> 2) & 0x3FFFFFF);
    }
    if (a_strcmp(mnemonic, "jal") == 0) {
        unsigned int addr = assembler_parse_address(op1);
        return encode_j_type(OP_JAL, (addr >> 2) & 0x3FFFFFF);
    }
    if (a_strcmp(mnemonic, "reti") == 0) {
        return encode_j_type(OP_RETI, 0);
    }
    if (a_strcmp(mnemonic, "halt") == 0) {
        return encode_j_type(OP_HALT, 0);
    }
    if (a_strcmp(mnemonic, "nop") == 0) {
        return 0;
    }

    return 0;
}

