#include "assembler.h"
#include "optable.h"

#define MAX_OPERANDS    4
#define MAX_OPERAND_LEN 64

typedef struct {
    char items[MAX_OPERANDS][MAX_OPERAND_LEN];
    int count;
} operands_t;

static const char *reg_names[32] = {
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"};

static int same_str(const char *a, const char *b) {
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}

static char lower(char c) {
    if (c >= 'A' && c <= 'Z') return (char)(c - 'A' + 'a');
    return c;
}

static int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

static int is_ident_start(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static int is_ident_char(char c) {
    return is_ident_start(c) || (c >= '0' && c <= '9');
}

static const char *skip_space(const char *s) {
    while (is_space(*s)) s++;
    return s;
}

static const char *str_end(const char *s) {
    while (*s) s++;
    return s;
}

static void copy_trimmed(char *dst, int cap, const char *begin, const char *end) {
    while (begin < end && is_space(*begin)) begin++;
    while (end > begin && is_space(end[-1])) end--;
    int n = 0;
    while (begin < end && n < cap - 1) dst[n++] = *begin++;
    dst[n] = '\0';
}

static void append(char **o, char *end, const char *s) {
    while (*s && *o < end) {
        **o = *s;
        (*o)++;
        s++;
    }
}

static int fail(assembler_t *as, const char *msg, const char *arg) {
    char *o = as->error;
    char *end = as->error + MAX_ERROR_LEN - 1;
    append(&o, end, msg);
    if (arg) {
        append(&o, end, " '");
        append(&o, end, arg);
        append(&o, end, "'");
    }
    *o = '\0';
    return -1;
}

static int is_identifier(const char *s) {
    if (!is_ident_start(*s)) return 0;
    while (*s && is_ident_char(*s)) s++;
    return *s == '\0';
}

static int digit_value(char c, int base) {
    int d = 99;
    if (c >= '0' && c <= '9') d = c - '0';
    if (c >= 'a' && c <= 'f') d = c - 'a' + 10;
    if (c >= 'A' && c <= 'F') d = c - 'A' + 10;
    return d < base ? d : -1;
}

static int parse_number(const char *s, long long *out) {
    if (s[0] == '\'' && s[1] && s[2] == '\'' && s[3] == '\0') {
        *out = (unsigned char)s[1];
        return 1;
    }
    int neg = 0;
    if (*s == '+' || *s == '-') {
        neg = *s == '-';
        s++;
    }
    int base = 10;
    if (s[0] == '0' && lower(s[1]) == 'x') {
        base = 16;
        s += 2;
    }
    if (*s == '\0') return 0;
    long long v = 0;
    while (*s) {
        int d = digit_value(*s, base);
        if (d < 0) return 0;
        v = v * base + d;
        if (v > 0xFFFFFFFFLL) return 0;
        s++;
    }
    *out = neg ? -v : v;
    return 1;
}

void assembler_init(assembler_t *as) {
    as->label_count = 0;
    as->current_address = 0;
    as->error[0] = '\0';
}

int assembler_find_label(assembler_t *as, const char *name, unsigned int *address) {
    for (int i = 0; i < as->label_count; i++) {
        if (same_str(as->labels[i].name, name)) {
            *address = as->labels[i].address;
            return 1;
        }
    }
    return 0;
}

int assembler_add_label(assembler_t *as, const char *name, unsigned int address) {
    unsigned int existing;
    if (assembler_find_label(as, name, &existing)) return fail(as, "duplicate label", name);
    if (as->label_count >= MAX_LABELS) return fail(as, "too many labels", 0);
    copy_trimmed(as->labels[as->label_count].name, MAX_LABEL_NAME, name, str_end(name));
    as->labels[as->label_count].address = address;
    as->label_count++;
    return 0;
}

int assembler_parse_register(const char *str) {
    if (*str != '$') return -1;
    str++;
    char name[8];
    int n = 0;
    while (str[n] && n < 7) {
        name[n] = lower(str[n]);
        n++;
    }
    if (str[n]) return -1;
    name[n] = '\0';
    long long num;
    if (parse_number(name, &num) && name[0] >= '0' && name[0] <= '9') {
        return num <= 31 ? (int)num : -1;
    }
    for (int i = 0; i < 32; i++) {
        if (same_str(reg_names[i], name)) return i;
    }
    return -1;
}

static const char *strip_label(assembler_t *as, const char *p, asm_line_t *out, int *err) {
    const char *q = p;
    while (is_ident_char(*q)) q++;
    if (*q != ':' || !is_ident_start(*p)) return p;
    if (q - p >= MAX_LABEL_NAME) {
        *err = fail(as, "label too long", 0);
        return p;
    }
    copy_trimmed(out->label, MAX_LABEL_NAME, p, q);
    return skip_space(q + 1);
}

static int parse_directive(assembler_t *as, const char *p, asm_line_t *out) {
    const char *q = p + 1;
    while (is_ident_char(*q)) q++;
    char name[16];
    copy_trimmed(name, sizeof name, p, q);
    int is_org = same_str(name, ".org");
    if (!(is_org || same_str(name, ".word")) || !is_space(*q)) return fail(as, "unknown directive", name);
    char arg[MAX_OPERAND_LEN];
    copy_trimmed(arg, MAX_OPERAND_LEN, q, str_end(q));
    long long v;
    if (!parse_number(arg, &v) || v < -2147483648LL) return fail(as, is_org ? "invalid .org address" : "invalid .word value", arg);
    if (is_org && v < 0) return fail(as, "invalid .org address", arg);
    out->kind = is_org ? LINE_ORG : LINE_WORD;
    out->org = (unsigned int)v;
    return 0;
}

int assembler_split_line(assembler_t *as, const char *line, asm_line_t *out) {
    out->kind = LINE_EMPTY;
    out->label[0] = '\0';
    out->text[0] = '\0';
    out->org = 0;

    const char *end = line;
    while (*end && *end != '#') end++;
    char buf[MAX_LINE_LEN];
    copy_trimmed(buf, MAX_LINE_LEN, line, end);

    int err = 0;
    const char *p = strip_label(as, buf, out, &err);
    if (err) return err;
    if (*p == '\0') return 0;
    if (*p == '.') return parse_directive(as, p, out);

    out->kind = LINE_INSTR;
    copy_trimmed(out->text, MAX_LINE_LEN, p, str_end(p));
    return 0;
}

static int split_operands(assembler_t *as, const char *p, operands_t *ops) {
    ops->count = 0;
    p = skip_space(p);
    if (*p == '\0') return 0;
    while (1) {
        const char *q = p;
        while (*q && *q != ',') q++;
        if (ops->count >= MAX_OPERANDS) return fail(as, "too many operands", 0);
        copy_trimmed(ops->items[ops->count], MAX_OPERAND_LEN, p, q);
        if (ops->items[ops->count][0] == '\0') return fail(as, "empty operand", 0);
        ops->count++;
        if (*q == '\0') return 0;
        p = q + 1;
    }
}

static int expect_count(assembler_t *as, const operands_t *ops, int n, const char *mnemonic) {
    if (ops->count == n) return 0;
    return fail(as, "wrong number of operands for", mnemonic);
}

static int reg_operand(assembler_t *as, const char *s, unsigned int *out) {
    int r = assembler_parse_register(s);
    if (r < 0) return fail(as, "invalid register", s);
    *out = (unsigned int)r;
    return 0;
}

static int imm_operand(assembler_t *as, const char *s, long long lo, long long hi, unsigned int *out) {
    long long v;
    if (!parse_number(s, &v)) return fail(as, "invalid number", s);
    if (v < lo || v > hi) return fail(as, "value out of range", s);
    *out = (unsigned int)v;
    return 0;
}

static int target_operand(assembler_t *as, const char *s, long long *out) {
    if (parse_number(s, out)) return 0;
    if (!is_identifier(s)) return fail(as, "invalid target", s);
    unsigned int addr;
    if (!assembler_find_label(as, s, &addr)) return fail(as, "undefined label", s);
    *out = addr;
    return 1;
}

static int mem_operand(assembler_t *as, const char *s, unsigned int *offset, unsigned int *base) {
    const char *open = s;
    while (*open && *open != '(') open++;
    if (*open == '\0') return fail(as, "expected offset($reg)", s);
    const char *close = open + 1;
    while (*close && *close != ')') close++;
    if (*close != ')' || *skip_space(close + 1) != '\0') return fail(as, "expected offset($reg)", s);
    char off[MAX_OPERAND_LEN], reg[MAX_OPERAND_LEN];
    copy_trimmed(off, MAX_OPERAND_LEN, s, open);
    copy_trimmed(reg, MAX_OPERAND_LEN, open + 1, close);
    *offset = 0;
    if (off[0] && imm_operand(as, off, -32768, 0xFFFF, offset)) return -1;
    return reg_operand(as, reg, base);
}

static int encode_r(assembler_t *as, const op_info_t *op, const operands_t *o, unsigned int *word) {
    unsigned int rd = 0, rs = 0, rt = 0, sh = 0;
    int e = 0;
    switch (op->format) {
        case FMT_R3:
            e = expect_count(as, o, 3, op->name) || reg_operand(as, o->items[0], &rd) || reg_operand(as, o->items[1], &rs) || reg_operand(as, o->items[2], &rt);
            break;
        case FMT_SHIFT:
            e = expect_count(as, o, 3, op->name) || reg_operand(as, o->items[0], &rd) || reg_operand(as, o->items[1], &rt) || imm_operand(as, o->items[2], 0, 31, &sh);
            break;
        case FMT_SHIFTV:
            e = expect_count(as, o, 3, op->name) || reg_operand(as, o->items[0], &rd) || reg_operand(as, o->items[1], &rt) || reg_operand(as, o->items[2], &rs);
            break;
        case FMT_JR:
            e = expect_count(as, o, 1, op->name) || reg_operand(as, o->items[0], &rs);
            break;
        default:
            rd = REG_RA;
            if (o->count == 1) e = reg_operand(as, o->items[0], &rs);
            else e = expect_count(as, o, 2, op->name) || reg_operand(as, o->items[0], &rd) || reg_operand(as, o->items[1], &rs);
            break;
    }
    *word = encode_r_type(rs, rt, rd, sh, op->funct);
    return e ? -1 : 0;
}

static int encode_branch(assembler_t *as, const op_info_t *op, const operands_t *o, unsigned int *word) {
    unsigned int rs = 0, rt = 0;
    long long off = 0;
    if (expect_count(as, o, 3, op->name) || reg_operand(as, o->items[0], &rs) || reg_operand(as, o->items[1], &rt)) return -1;
    int kind = target_operand(as, o->items[2], &off);
    if (kind < 0) return -1;
    if (kind == 1) off -= (long long)as->current_address + 1;
    if (off < -32768 || off > 32767) return fail(as, "branch target out of range", o->items[2]);
    *word = encode_i_type(op->opcode, rs, rt, (unsigned int)off & 0xFFFF);
    return 0;
}

static int encode_jump(assembler_t *as, const op_info_t *op, const operands_t *o, unsigned int *word) {
    long long target = 0;
    if (expect_count(as, o, 1, op->name)) return -1;
    if (target_operand(as, o->items[0], &target) < 0) return -1;
    if (target < 0 || target > 0x3FFFFFF) return fail(as, "jump target out of range", o->items[0]);
    *word = encode_j_type(op->opcode, (unsigned int)target);
    return 0;
}

static int encode_i(assembler_t *as, const op_info_t *op, const operands_t *o, unsigned int *word) {
    unsigned int rs = 0, rt = 0, imm = 0;
    int e = 0;
    switch (op->format) {
        case FMT_IMM_S:
            e = expect_count(as, o, 3, op->name) || reg_operand(as, o->items[0], &rt) || reg_operand(as, o->items[1], &rs) || imm_operand(as, o->items[2], -32768, 0xFFFF, &imm);
            break;
        case FMT_IMM_U:
            e = expect_count(as, o, 3, op->name) || reg_operand(as, o->items[0], &rt) || reg_operand(as, o->items[1], &rs) || imm_operand(as, o->items[2], 0, 0xFFFF, &imm);
            break;
        case FMT_LUI:
            e = expect_count(as, o, 2, op->name) || reg_operand(as, o->items[0], &rt) || imm_operand(as, o->items[1], 0, 0xFFFF, &imm);
            break;
        default:
            e = expect_count(as, o, 2, op->name) || reg_operand(as, o->items[0], &rt) || mem_operand(as, o->items[1], &imm, &rs);
            break;
    }
    *word = encode_i_type(op->opcode, rs, rt, imm & 0xFFFF);
    return e ? -1 : 0;
}

static int encode_move(assembler_t *as, const operands_t *o, unsigned int *word) {
    unsigned int rd = 0, rs = 0;
    if (expect_count(as, o, 2, "move") || reg_operand(as, o->items[0], &rd) || reg_operand(as, o->items[1], &rs)) return -1;
    *word = encode_r_type(rs, REG_ZERO, rd, 0, FUNCT_ADD);
    return 0;
}

static int encode_li(assembler_t *as, const operands_t *o, unsigned int *word) {
    unsigned int rt = 0;
    long long v;
    if (expect_count(as, o, 2, "li") || reg_operand(as, o->items[0], &rt)) return -1;
    if (target_operand(as, o->items[1], &v) < 0) return -1;
    unsigned int u = (unsigned int)v;
    if ((v >= -32768 && v <= 32767) || (v >= 0xFFFF8000LL && v <= 0xFFFFFFFFLL)) *word = encode_i_type(OP_ADDI, REG_ZERO, rt, u & 0xFFFF);
    else if ((u & 0xFFFF) == 0) *word = encode_i_type(OP_LUI, REG_ZERO, rt, u >> 16);
    else return fail(as, "li value needs lui + ori", o->items[1]);
    return 0;
}

static int encode_b(assembler_t *as, const operands_t *o, unsigned int *word) {
    if (expect_count(as, o, 1, "b")) return -1;
    op_info_t beq = *optable_find_name("beq");
    operands_t full = {.count = 3};
    const char *zero = "$zero";
    copy_trimmed(full.items[0], MAX_OPERAND_LEN, zero, zero + 5);
    copy_trimmed(full.items[1], MAX_OPERAND_LEN, zero, zero + 5);
    copy_trimmed(full.items[2], MAX_OPERAND_LEN, o->items[0], str_end(o->items[0]));
    return encode_branch(as, &beq, &full, word);
}

int assembler_assemble_line(assembler_t *as, const char *text, unsigned int *word) {
    *word = 0;
    const char *p = skip_space(text);
    char mnemonic[16];
    int n = 0;
    while (*p && !is_space(*p) && n < 15) mnemonic[n++] = lower(*p++);
    mnemonic[n] = '\0';

    operands_t ops;
    if (split_operands(as, p, &ops)) return -1;

    if (same_str(mnemonic, "nop")) return expect_count(as, &ops, 0, mnemonic);
    if (same_str(mnemonic, "move")) return encode_move(as, &ops, word);
    if (same_str(mnemonic, "li")) return encode_li(as, &ops, word);
    if (same_str(mnemonic, "b")) return encode_b(as, &ops, word);

    const op_info_t *op = optable_find_name(mnemonic);
    if (!op) return fail(as, "unknown instruction", mnemonic);

    switch (op->format) {
        case FMT_R3:
        case FMT_SHIFT:
        case FMT_SHIFTV:
        case FMT_JR:
        case FMT_JALR:
            return encode_r(as, op, &ops, word);
        case FMT_BRANCH:
            return encode_branch(as, op, &ops, word);
        case FMT_JUMP:
            return encode_jump(as, op, &ops, word);
        case FMT_NONE:
            *word = encode_j_type(op->opcode, 0);
            return expect_count(as, &ops, 0, mnemonic);
        default:
            return encode_i(as, op, &ops, word);
    }
}
