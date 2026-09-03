#include "test.h"
#include "../simulation/simulator.h"
#include "../ref/iss.h"

#include "../system/board.h"
#include "../isa/encoding.h"

#define HALT_WORD       ((uint32_t)OP_HALT << 26)
#define MAX_CYCLES      20000
#define RANDOM_PROGRAMS 100

typedef enum { ARCH_SINGLE,
               ARCH_MULTI,
               ARCH_PIPE } arch_t;

static const char *arch_name[] = {"single", "multi", "pipe"};

static void board_load(board_t *b, const uint32_t *rom) {
    bit w[32];
    for (int i = 0; i < ISS_ROM_WORDS; i++) {
        to_bits_32(rom[i], w);
        rom_load_at(&b->rom, (unsigned)(i), w);
    }
}

static uint32_t board_reg(board_t *b, int i) {
    regfile_propagate(&b->regs);
    return from_bits_32(b->regs.regs[i].data_out);
}

static uint32_t board_ram(board_t *b, int i) {
    bit addr[32], out[32];
    to_bits_32((uint32_t)i, addr);
    ram_set_addr(&b->ram, addr);
    ram_set_write(&b->ram, 0);
    ram_set_cs(&b->ram, 1);
    ram_propagate(&b->ram);
    ram_read(&b->ram, out);
    return from_bits_32(out);
}

static void board_cycle(board_t *b, arch_t arch) {
    if (arch == ARCH_SINGLE) board_step(b);
    if (arch == ARCH_MULTI) board_step_mc(b);
    if (arch == ARCH_PIPE) board_step_pipe(b);
}

static void iss_run(iss_t *ref, const uint32_t *rom) {
    iss_reset(ref);
    for (int s = 0; s < MAX_CYCLES && !ref->halted; s++) iss_step(ref, rom);
}

static int compare_state(board_t *b, const iss_t *ref, const char *tag) {
    int fails = g_fail;
    char label[64];
    for (int i = 0; i < 32; i++) {
        snprintf(label, sizeof label, "%s reg[%d]", tag, i);
        CHECK_EQ_U32(board_reg(b, i), ref->reg[i], label);
    }
    for (int i = 0; i < ISS_WORDS; i++) {
        snprintf(label, sizeof label, "%s ram[%d]", tag, i);
        CHECK_EQ_U32(board_ram(b, i), ref->ram[i], label);
    }
    return g_fail == fails;
}

static board_t g_board;
static bit g_l2;

static int run_arch(const uint32_t *rom, const iss_t *ref, arch_t arch) {
    board_t *b = &g_board;
    board_init(b);
    b->config.pin[STRAP_L2] = g_l2;
    config_reset(&b->config);
    if (arch == ARCH_PIPE) board_pipe_init(b);
    board_load(b, rom);
    int n = 0;
    while (n < MAX_CYCLES && !b->halt) {
        board_cycle(b, arch);
        n++;
    }
    CHECK(b->halt, "%s: did not halt within %d cycles", arch_name[arch], MAX_CYCLES);
    return compare_state(b, ref, arch_name[arch]);
}

static int run_lockstep(const uint32_t *rom) {
    board_t *b = &g_board;
    iss_t ref;
    iss_reset(&ref);
    board_init(b);
    board_load(b, rom);
    for (int s = 0; s < MAX_CYCLES && !ref.halted; s++) {
        pc_propagate(&b->pc);
        uint32_t pc = from_bits_32(b->pc.reg.data_out);
        if (pc != ref.pc) {
            CHECK_EQ_U32(pc, ref.pc, "lockstep pc");
            return 0;
        }
        board_step(b);
        iss_step(&ref, rom);
        for (int i = 0; i < 32; i++) {
            if (board_reg(b, i) == ref.reg[i]) continue;
            CHECK(0, "lockstep step %d pc 0x%x: reg[%d] got 0x%08x expected 0x%08x",
                  s, pc, i, board_reg(b, i), ref.reg[i]);
            return 0;
        }
    }
    CHECK(b->halt == ref.halted, "lockstep halt state");
    return 1;
}

static void cosim(const char *name, const uint32_t *prog, int n, int with_pipe) {
    uint32_t rom[ISS_ROM_WORDS] = {0};
    for (int i = 0; i < n && i < ISS_ROM_WORDS; i++) rom[i] = prog[i];
    test_suite(name);
    iss_t ref;
    iss_run(&ref, rom);
    run_lockstep(rom);
    run_arch(rom, &ref, ARCH_SINGLE);
    run_arch(rom, &ref, ARCH_MULTI);
    if (with_pipe) run_arch(rom, &ref, ARCH_PIPE);
}

static void prog_arith(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 100),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 37),
        encode_r_type(REG_T0, REG_T1, REG_T2, 0, FUNCT_SUB),
        encode_r_type(REG_T0, REG_T1, REG_T3, 0, FUNCT_AND),
        encode_r_type(REG_T0, REG_T1, REG_T4, 0, FUNCT_OR),
        encode_i_type(OP_ADDI, REG_T2, REG_T5, (uint32_t)-5 & 0xFFFF),
        HALT_WORD,
    };
    cosim("cosim: arithmetic", p, 7, 1);
}

static void prog_logic_shift(void) {
    uint32_t p[] = {
        encode_i_type(OP_LUI, 0, REG_T0, 0x8765),
        encode_i_type(OP_ORI, REG_T0, REG_T0, 0x4321),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 7),
        encode_r_type(REG_T0, REG_T1, REG_S0, 0, FUNCT_XOR),
        encode_r_type(REG_T0, REG_T1, REG_S1, 0, FUNCT_NOR),
        encode_r_type(REG_T0, REG_T1, REG_S2, 0, FUNCT_SLT),
        encode_r_type(REG_T0, REG_T1, REG_S3, 0, FUNCT_SLTU),
        encode_r_type(0, REG_T0, REG_S4, 5, FUNCT_SLL),
        encode_r_type(0, REG_T0, REG_S5, 9, FUNCT_SRL),
        encode_r_type(0, REG_T0, REG_S6, 9, FUNCT_SRA),
        encode_r_type(REG_T1, REG_T0, REG_S7, 0, FUNCT_SRAV),
        encode_i_type(OP_ANDI, REG_T0, REG_T2, 0xFF00),
        encode_i_type(OP_XORI, REG_T0, REG_T3, 0xFFFF),
        encode_i_type(OP_SLTI, REG_T0, REG_T4, 1),
        encode_i_type(OP_SLTIU, REG_T1, REG_T5, 8),
        HALT_WORD,
    };
    cosim("cosim: logic, compare and shifts", p, 16, 1);
}

static void prog_mem(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 1234),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 3),
        encode_i_type(OP_LW, REG_ZERO, REG_T1, 3),
        encode_r_type(REG_T1, REG_T1, REG_T2, 0, FUNCT_ADD),
        encode_i_type(OP_SW, REG_ZERO, REG_T2, 200),
        HALT_WORD,
    };
    cosim("cosim: memory sw/lw", p, 6, 1);
}

static void prog_branches(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 4),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 4),
        encode_i_type(OP_BEQ, REG_T0, REG_T1, 1),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T2, 99),
        encode_i_type(OP_BNE, REG_T0, REG_T1, 1),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T3, 7),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T4, 3),
        encode_i_type(OP_ADDI, REG_T4, REG_T4, 0xFFFF),
        encode_i_type(OP_BNE, REG_T4, REG_ZERO, 0xFFFE),
        HALT_WORD,
    };
    cosim("cosim: beq/bne and a counted loop", p, 10, 1);
}

static void prog_jumps(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 1),
        encode_j_type(OP_J, 3),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 99),
        encode_j_type(OP_JAL, 8),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T2, 10),
        encode_r_type(REG_T2, 0, REG_S0, 0, FUNCT_JALR),
        HALT_WORD,
        HALT_WORD,
        encode_i_type(OP_ADDI, REG_ZERO, REG_T3, 5),
        encode_r_type(REG_RA, 0, 0, 0, FUNCT_JR),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T4, 6),
        encode_r_type(REG_S0, 0, 0, 0, FUNCT_JR),
    };
    cosim("cosim: j, jal/jr, jalr", p, 12, 1);
}

static void prog_nested_stack(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_SP, 15),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 5),
        encode_j_type(OP_JAL, 4),
        HALT_WORD,
        encode_i_type(OP_ADDI, REG_SP, REG_SP, 0xFFFF),
        encode_i_type(OP_SW, REG_SP, REG_RA, 0),
        encode_j_type(OP_JAL, 11),
        encode_i_type(OP_ADDI, REG_T1, REG_T1, 1),
        encode_i_type(OP_LW, REG_SP, REG_RA, 0),
        encode_i_type(OP_ADDI, REG_SP, REG_SP, 1),
        encode_r_type(REG_RA, 0, 0, 0, FUNCT_JR),
        encode_r_type(REG_T0, REG_T0, REG_T1, 0, FUNCT_ADD),
        encode_r_type(REG_RA, 0, 0, 0, FUNCT_JR),
    };
    cosim("cosim: nested calls with stack", p, 13, 1);
}

static void prog_hazards(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 42),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 5),
        encode_i_type(OP_LW, REG_ZERO, REG_T1, 5),
        encode_r_type(REG_T1, REG_T1, REG_T2, 0, FUNCT_ADD),
        encode_r_type(REG_T2, REG_T1, REG_T3, 0, FUNCT_ADD),
        encode_r_type(REG_T3, REG_T2, REG_T4, 0, FUNCT_SUB),
        encode_i_type(OP_SW, REG_ZERO, REG_T4, 6),
        HALT_WORD,
    };
    cosim("cosim: RAW, load-use and store before halt", p, 8, 1);
}

static void prog_halt_edges(void) {
    uint32_t p[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 9),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 9),
        encode_i_type(OP_BEQ, REG_T0, REG_T1, 1),
        HALT_WORD,
        encode_i_type(OP_ADDI, REG_ZERO, REG_T2, 1),
        HALT_WORD,
        encode_i_type(OP_ADDI, REG_ZERO, REG_T3, 77),
    };
    cosim("cosim: taken branch over halt, nothing runs after halt", p, 7, 1);
}

static void prog_undefined(void) {
    uint32_t p[ISS_VEC_UNDEFINED + 2] = {0};
    p[0] = encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 1);
    p[1] = encode_r_type(0, 0, 0, 0, FUNCT_MTHI);
    p[2] = encode_i_type(OP_ADDI, REG_ZERO, REG_T1, 2);
    p[3] = HALT_WORD;
    p[ISS_VEC_UNDEFINED] = encode_i_type(OP_ADDI, REG_ZERO, REG_T2, 3);
    p[ISS_VEC_UNDEFINED + 1] = HALT_WORD;
    uint32_t rom[ISS_ROM_WORDS] = {0};
    for (uint32_t i = 0; i < ISS_VEC_UNDEFINED + 2; i++) rom[i] = p[i];
    test_suite("cosim: undefined instruction traps (all three cores)");
    iss_t ref;
    iss_run(&ref, rom);
    run_lockstep(rom);
    run_arch(rom, &ref, ARCH_SINGLE);
    run_arch(rom, &ref, ARCH_MULTI);
    run_arch(rom, &ref, ARCH_PIPE);
}

static uint32_t rng_state = 0x12345678u;

static uint32_t rnd(uint32_t n) {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state % n;
}

static const uint32_t rand_regs[] = {
    REG_ZERO, REG_T0, REG_T1, REG_T2, REG_T3, REG_S0, REG_S1, REG_S2, REG_A0, REG_V0};

static uint32_t rreg(void) {
    return rand_regs[rnd(sizeof rand_regs / sizeof rand_regs[0])];
}

static const uint32_t r3_functs[] = {
    FUNCT_ADD, FUNCT_SUB, FUNCT_AND, FUNCT_OR, FUNCT_XOR, FUNCT_NOR, FUNCT_SLT, FUNCT_SLTU,
    FUNCT_SLLV, FUNCT_SRLV, FUNCT_SRAV};

static const uint32_t shift_functs[] = {FUNCT_SLL, FUNCT_SRL, FUNCT_SRA};

static const uint32_t imm_ops[] = {OP_ADDI, OP_SLTI, OP_SLTIU, OP_ANDI, OP_ORI, OP_XORI};

static uint32_t random_instr(int pc, int len) {
    uint32_t kind = rnd(100);
    int left = len - pc - 1;
    if (kind < 30) return encode_r_type(rreg(), rreg(), rreg(), 0, r3_functs[rnd(11)]);
    if (kind < 40) return encode_r_type(0, rreg(), rreg(), rnd(32), shift_functs[rnd(3)]);
    if (kind < 60) return encode_i_type(imm_ops[rnd(6)], rreg(), rreg(), rnd(0x10000));
    if (kind < 65) return encode_i_type(OP_LUI, 0, rreg(), rnd(0x10000));
    if (kind < 75) return encode_i_type(OP_SW, REG_ZERO, rreg(), rnd(ISS_WORDS));
    if (kind < 85) return encode_i_type(OP_LW, REG_ZERO, rreg(), rnd(ISS_WORDS));
    if (left < 2) return encode_i_type(OP_ADDI, rreg(), rreg(), rnd(0x10000));
    if (kind < 95) return encode_i_type(rnd(2) ? OP_BEQ : OP_BNE, rreg(), rreg(), rnd((uint32_t)(left < 6 ? left : 6)));
    return encode_j_type(rnd(2) ? OP_J : OP_JAL, (uint32_t)pc + 1 + rnd((uint32_t)(left < 6 ? left : 6)));
}

static void random_program(uint32_t *rom, int len) {
    for (int i = 0; i < ISS_ROM_WORDS; i++) rom[i] = HALT_WORD;
    for (int i = 0; i < len - 1; i++) rom[i] = random_instr(i, len);
}

static void random_cosim(void) {
    uint32_t rom[ISS_ROM_WORDS];
    int bad[3] = {0, 0, 0}, bad_lock = 0;
    test_suite("cosim: random programs, three microarchitectures vs ISS");
    for (int p = 0; p < RANDOM_PROGRAMS; p++) {
        random_program(rom, 20 + (int)rnd(100));
        g_l2 = (bit)(p & 1);
        iss_t ref;
        iss_run(&ref, rom);
        bad_lock += !run_lockstep(rom);
        bad[ARCH_SINGLE] += !run_arch(rom, &ref, ARCH_SINGLE);
        bad[ARCH_MULTI] += !run_arch(rom, &ref, ARCH_MULTI);
        bad[ARCH_PIPE] += !run_arch(rom, &ref, ARCH_PIPE);
    }
    g_l2 = 0;
    printf("  random: %d programs (half with L2), mismatches lockstep=%d single=%d multi=%d pipe=%d\n",
           RANDOM_PROGRAMS, bad_lock, bad[0], bad[1], bad[2]);
}

int main(void) {
    prog_arith();
    prog_logic_shift();
    prog_mem();
    prog_branches();
    prog_jumps();
    prog_nested_stack();
    prog_hazards();
    prog_halt_edges();
    prog_undefined();
    random_cosim();
    return test_report();
}
