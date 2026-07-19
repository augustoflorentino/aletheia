#include "test.h"

#include "../system/board.h"
#include "../isa/isa.h"

#define REF_OP_HALT 0x3F

typedef struct {
    uint32_t reg[32];
    uint32_t ram[16];
    uint32_t pc;
    int halted;
} ref_cpu_t;

static void ref_reset(ref_cpu_t *c) {
    int i;
    for (i = 0; i < 32; i++) c->reg[i] = 0;
    for (i = 0; i < 16; i++) c->ram[i] = 0;
    c->pc = 0;
    c->halted = 0;
}

static void ref_step(ref_cpu_t *c, const uint32_t *rom) {
    if (c->halted) return;
    uint32_t instr = rom[c->pc & 15u];
    uint32_t op = instr >> 26;
    uint32_t rs = (instr >> 21) & 31u;
    uint32_t rt = (instr >> 16) & 31u;
    uint32_t rd = (instr >> 11) & 31u;
    uint32_t funct = instr & 0x3Fu;
    int32_t imm = (int32_t)(int16_t)(instr & 0xFFFFu);
    uint32_t addr = instr & 0x3FFFFFFu;

    uint32_t vs = (rs == 0) ? 0u : c->reg[rs];
    uint32_t vt = (rt == 0) ? 0u : c->reg[rt];
    uint32_t next = c->pc + 1u;

    if (op == REF_OP_HALT) {
        c->halted = 1;
        return;
    } else if (op == OP_RTYPE && funct == FUNCT_JR) {
        next = vs;
    } else if (op == OP_JAL) {
        c->reg[31] = c->pc + 1u;
        next = addr;
    } else if (op == OP_RTYPE) {
        uint32_t res;
        if (funct == FUNCT_SUB) res = vs - vt;
        else if (funct == FUNCT_AND) res = vs & vt;
        else if (funct == FUNCT_OR) res = vs | vt;
        else res = vs + vt;
        if (rd != 0) c->reg[rd] = res;
    } else if (op == OP_ADDI) {
        if (rt != 0) c->reg[rt] = vs + (uint32_t)imm;
    } else if (op == OP_LW) {
        uint32_t a = (vs + (uint32_t)imm) & 15u;
        if (rt != 0) c->reg[rt] = c->ram[a];
    } else if (op == OP_SW) {
        uint32_t a = (vs + (uint32_t)imm) & 15u;
        c->ram[a] = vt;
    } else if (op == OP_BEQ) {
        if (vs == vt) next = c->pc + 1u + (uint32_t)imm;
    } else if (op == OP_J) {
        next = addr;
    }

    c->pc = next;
    c->reg[0] = 0;
}

static void board_load(board_t *b, const uint32_t *words, int n) {
    bit prog[16][32];
    int i, j;
    for (i = 0; i < 16; i++) {
        uint32_t w = (i < n) ? words[i] : 0u;
        for (j = 0; j < 32; j++) prog[i][j] = (w >> j) & 1u;
    }
    for (i = 0; i < 16; i++) rom256_load_word(&b->rom, i, prog[i]);
}

static uint32_t board_reg(board_t *b, int i) {
    regfile_propagate(&b->regs);
    return from_bits_32(b->regs.regs[i].data_out);
}

static uint32_t board_ram(board_t *b, int i) {
    bit addr[8], out[32];
    to_bits_n((uint32_t)i, addr, 8);
    sram256_set_addr(&b->ram, addr);
    sram256_set_write(&b->ram, 0);
    sram256_set_cs(&b->ram, 1);
    sram256_propagate(&b->ram);
    sram256_read(&b->ram, out);
    return from_bits_32(out);
}

static void cosim(const char *name, const uint32_t *prog, int n, int steps) {
    test_suite(name);

    uint32_t rom16[16];
    int k;
    for (k = 0; k < 16; k++) rom16[k] = (k < n) ? prog[k] : 0u;

    board_t board;
    board_init(&board);
    board_load(&board, rom16, 16);

    ref_cpu_t ref;
    ref_reset(&ref);

    int s, i;
    for (s = 0; s < steps; s++) {
        board_step(&board);
        ref_step(&ref, rom16);
    }

    for (i = 0; i < 32; i++) {
        char label[32];
        snprintf(label, sizeof(label), "reg[%d]", i);
        CHECK_EQ_U32(board_reg(&board, i), ref.reg[i], label);
    }
    for (i = 0; i < 16; i++) {
        char label[32];
        snprintf(label, sizeof(label), "ram[%d]", i);
        CHECK_EQ_U32(board_ram(&board, i), ref.ram[i], label);
    }
}

static void prog_demo(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 5),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 10),
        encode_r_type(REG_T0, REG_T1, REG_T2, 0, FUNCT_ADD),
        encode_i_type(OP_SW, REG_ZERO, REG_T2, 0),
    };
    cosim("cosim: demo (add + store)", p, 4, 8);
}

static void prog_arith(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 100),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 37),
        encode_r_type(REG_T0, REG_T1, REG_T2, 0, FUNCT_SUB),
        encode_r_type(REG_T0, REG_T1, REG_T3, 0, FUNCT_AND),
        encode_r_type(REG_T0, REG_T1, REG_T4, 0, FUNCT_OR),
        encode_i_type(OP_ADDI, REG_T2, REG_T5, -5),
    };
    cosim("cosim: arithmetic (sub/and/or/addi neg)", p, 6, 10);
}

static void prog_mem(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 1234 & 0x7FFF),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 3),
        encode_i_type(OP_LW, REG_ZERO, REG_T1, 3),
        encode_r_type(REG_T1, REG_T1, REG_T2, 0, FUNCT_ADD),
        encode_i_type(OP_SW, REG_ZERO, REG_T2, 7),
    };
    cosim("cosim: memory (sw/lw roundtrip)", p, 5, 10);
}

static void prog_branch_taken(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 4),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 4),
        encode_i_type(OP_BEQ, REG_T0, REG_T1, 1),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T2, 99),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T3, 7),
    };
    cosim("cosim: branch taken (skips instr)", p, 5, 8);
}

static void prog_branch_not_taken(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 4),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 5),
        encode_i_type(OP_BEQ, REG_T0, REG_T1, 1),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T2, 99),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T3, 7),
    };
    cosim("cosim: branch not taken (runs instr)", p, 5, 8);
}

static void prog_jump(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 1),
        encode_j_type(OP_J, 3),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 99),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T2, 42),
    };
    cosim("cosim: jump (skips instr 2)", p, 4, 6);
}

static void prog_subroutine(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 7),
        encode_j_type(OP_JAL, 5),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T3, 1),
        encode_i_type(OP_SW, REG_ZERO, REG_T1, 2),
        encode_j_type(OP_J, 8),
        encode_r_type(REG_T0, REG_T0, REG_T1, 0, FUNCT_ADD),
        encode_r_type(REG_T1, REG_T0, REG_T2, 0, FUNCT_SUB),
        encode_r_type(REG_RA, 0, 0, 0, FUNCT_JR),
    };
    cosim("cosim: subroutine (jal/jr) vs golden model", p, 8, 14);
}

static void prog_nested_stack(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_SP, 15),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 5),
        encode_j_type(OP_JAL, 4),
        (uint32_t)REF_OP_HALT << 26,
        encode_i_type(OP_ADDI, REG_SP, REG_SP, -1),
        encode_i_type(OP_SW, REG_SP, REG_RA, 0),
        encode_j_type(OP_JAL, 11),
        encode_i_type(OP_ADDI, REG_T1, REG_T1, 1),
        encode_i_type(OP_LW, REG_SP, REG_RA, 0),
        encode_i_type(OP_ADDI, REG_SP, REG_SP, 1),
        encode_r_type(REG_RA, 0, 0, 0, FUNCT_JR),
        encode_r_type(REG_T0, REG_T0, REG_T1, 0, FUNCT_ADD),
        encode_r_type(REG_RA, 0, 0, 0, FUNCT_JR),
    };
    cosim("cosim: nested calls with stack vs golden model", p, 13, 24);
}

static void cosim_mc(const char *name, const uint32_t *prog, int n,
                     int ref_steps, int mc_cycles) {
    test_suite(name);

    uint32_t rom16[16];
    int k;
    for (k = 0; k < 16; k++) rom16[k] = (k < n) ? prog[k] : 0u;

    board_t board;
    board_init(&board);
    board_load(&board, rom16, 16);

    ref_cpu_t ref;
    ref_reset(&ref);

    int s, i;
    for (s = 0; s < mc_cycles; s++) board_step_mc(&board);
    for (s = 0; s < ref_steps; s++) ref_step(&ref, rom16);

    for (i = 0; i < 32; i++) {
        char label[40];
        snprintf(label, sizeof(label), "mc reg[%d]", i);
        CHECK_EQ_U32(board_reg(&board, i), ref.reg[i], label);
    }
    for (i = 0; i < 16; i++) {
        char label[40];
        snprintf(label, sizeof(label), "mc ram[%d]", i);
        CHECK_EQ_U32(board_ram(&board, i), ref.ram[i], label);
    }
}

static void mc_prog_arith(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 100),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 37),
        encode_r_type(REG_T0, REG_T1, REG_T2, 0, FUNCT_SUB),
        encode_r_type(REG_T0, REG_T1, REG_T3, 0, FUNCT_AND),
        encode_r_type(REG_T0, REG_T1, REG_T4, 0, FUNCT_OR),
        encode_i_type(OP_ADDI, REG_T2, REG_T5, -5),
        (uint32_t)REF_OP_HALT << 26,
    };
    cosim_mc("cosim-mc: arithmetic (multi-cycle vs golden model)", p, 7, 10, 60);
}

static void mc_prog_mem(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 1234 & 0x7FFF),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 3),
        encode_i_type(OP_LW, REG_ZERO, REG_T1, 3),
        encode_r_type(REG_T1, REG_T1, REG_T2, 0, FUNCT_ADD),
        encode_i_type(OP_SW, REG_ZERO, REG_T2, 7),
        (uint32_t)REF_OP_HALT << 26,
    };
    cosim_mc("cosim-mc: memory sw/lw (multi-cycle)", p, 6, 10, 60);
}

static void mc_prog_branch(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 4),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 4),
        encode_i_type(OP_BEQ, REG_T0, REG_T1, 1),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T2, 99),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T3, 7),
        (uint32_t)REF_OP_HALT << 26,
    };
    cosim_mc("cosim-mc: branch taken (multi-cycle)", p, 6, 10, 60);
}

static void mc_prog_nested_stack(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_SP, 15),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 5),
        encode_j_type(OP_JAL, 4),
        (uint32_t)REF_OP_HALT << 26,
        encode_i_type(OP_ADDI, REG_SP, REG_SP, -1),
        encode_i_type(OP_SW, REG_SP, REG_RA, 0),
        encode_j_type(OP_JAL, 11),
        encode_i_type(OP_ADDI, REG_T1, REG_T1, 1),
        encode_i_type(OP_LW, REG_SP, REG_RA, 0),
        encode_i_type(OP_ADDI, REG_SP, REG_SP, 1),
        encode_r_type(REG_RA, 0, 0, 0, FUNCT_JR),
        encode_r_type(REG_T0, REG_T0, REG_T1, 0, FUNCT_ADD),
        encode_r_type(REG_RA, 0, 0, 0, FUNCT_JR),
    };
    cosim_mc("cosim-mc: nested subroutine with stack (multi-cycle)", p, 13, 24, 140);
}

static void cosim_pipe(const char *name, const uint32_t *prog, int n,
                       int ref_steps, int pipe_cycles) {
    test_suite(name);

    uint32_t rom16[16];
    int k;
    for (k = 0; k < 16; k++) rom16[k] = (k < n) ? prog[k] : 0u;

    board_t board;
    board_init(&board);
    board_pipe_init(&board);
    board_load(&board, rom16, 16);

    ref_cpu_t ref;
    ref_reset(&ref);

    int s, i;
    for (s = 0; s < pipe_cycles; s++) board_step_pipe(&board);
    for (s = 0; s < ref_steps; s++) ref_step(&ref, rom16);

    for (i = 0; i < 32; i++) {
        char label[40];
        snprintf(label, sizeof(label), "pipe reg[%d]", i);
        CHECK_EQ_U32(board_reg(&board, i), ref.reg[i], label);
    }
    for (i = 0; i < 16; i++) {
        char label[40];
        snprintf(label, sizeof(label), "pipe ram[%d]", i);
        CHECK_EQ_U32(board_ram(&board, i), ref.ram[i], label);
    }
}

static void pipe_raw(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 5),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 3),
        encode_r_type(REG_T0, REG_T1, REG_T2, 0, FUNCT_ADD),
        encode_r_type(REG_T2, REG_T0, REG_T3, 0, FUNCT_ADD),
        encode_r_type(REG_T3, REG_T2, REG_T4, 0, FUNCT_SUB),
        (uint32_t)REF_OP_HALT << 26,
    };
    cosim_pipe("cosim-pipe: RAW/forwarding", p, 6, 10, 20);
}

static void pipe_loaduse(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 42),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 5),
        encode_i_type(OP_LW, REG_ZERO, REG_T1, 5),
        encode_r_type(REG_T1, REG_T1, REG_T2, 0, FUNCT_ADD),
        (uint32_t)REF_OP_HALT << 26,
    };
    cosim_pipe("cosim-pipe: load-use (stall + forward)", p, 6, 10, 22);
}

static void pipe_branch(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 7),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 7),
        encode_i_type(OP_BEQ, REG_T0, REG_T1, 2),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T2, 99),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T3, 88),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T4, 1),
        (uint32_t)REF_OP_HALT << 26,
    };
    cosim_pipe("cosim-pipe: branch flush", p, 7, 10, 30);
}

int main(void) {
    prog_demo();
    prog_arith();
    prog_mem();
    prog_branch_taken();
    prog_branch_not_taken();
    prog_jump();
    prog_subroutine();
    prog_nested_stack();
    mc_prog_arith();
    mc_prog_mem();
    mc_prog_branch();
    mc_prog_nested_stack();
    pipe_raw();
    pipe_loaduse();
    pipe_branch();
    return test_report();
}
