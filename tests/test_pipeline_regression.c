#include "test.h"
#include "../ref/iss.h"

#include "../system/board.h"
#include "../simulation/simulator.h"
#include "../isa/encoding.h"

#define MAX_CYCLES 400
#define HALT_WORD  0xFC000000u

static board_t b;

typedef struct {
    int fwd_a;
    int fwd_b;
    int stalls;
} pipe_stats_t;

static void run_pipe(const uint32_t *prog, int n, iss_t *ref, pipe_stats_t *st) {
    uint32_t rom[ISS_ROM_WORDS] = {0};
    for (int i = 0; i < n; i++) rom[i] = prog[i];

    iss_reset(ref);
    for (int s = 0; s < MAX_CYCLES && !ref->halted; s++) iss_step(ref, rom);

    board_init(&b);
    board_pipe_init(&b);
    bit w[32];
    for (int i = 0; i < ISS_ROM_WORDS; i++) {
        to_bits_32(rom[i], w);
        rom_load_at(&b.rom, (unsigned)i, w);
    }
    *st = (pipe_stats_t){0, 0, 0};
    for (int c = 0; c < MAX_CYCLES && !b.halt; c++) {
        board_step_pipe(&b);
        bit fa[2], fb[2];
        forwarding_get_forward_a(&b.fwd, fa);
        forwarding_get_forward_b(&b.fwd, fb);
        st->fwd_a += fa[0] | fa[1];
        st->fwd_b += fb[0] | fb[1];
        st->stalls += hazard_get_stall(&b.haz);
    }
}

static void check_regs(const iss_t *ref, const char *tag) {
    regfile_propagate(&b.regs);
    for (int i = 0; i < 32; i++) {
        char label[48];
        snprintf(label, sizeof label, "%s reg[%d]", tag, i);
        CHECK_EQ_U32(from_bits_32(b.regs.regs[i].data_out), ref->reg[i], label);
    }
}

static void test_raw_chain(void) {
    test_suite("pipeline regression: RAW chain at distance 1, 2 and 3");

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 3),
        encode_r_type(REG_T0, REG_T0, REG_T1, 0, FUNCT_ADD),
        encode_r_type(REG_T1, REG_T0, REG_T2, 0, FUNCT_ADD),
        encode_r_type(REG_T2, REG_T1, REG_T3, 0, FUNCT_SUB),
        encode_r_type(REG_T0, REG_T3, REG_T4, 0, FUNCT_ADD),
        encode_r_type(REG_T4, REG_T4, REG_T4, 0, FUNCT_ADD),
        encode_r_type(REG_T4, REG_T2, REG_T5, 0, FUNCT_OR),
        HALT_WORD,
    };
    iss_t ref;
    pipe_stats_t st;
    run_pipe(prog, (int)(sizeof prog / sizeof prog[0]), &ref, &st);
    CHECK_EQ_BIT(b.halt, 1, "halted");
    check_regs(&ref, "raw");
    CHECK(st.fwd_a > 0, "forwarding A fired (%d cycles)", st.fwd_a);
    CHECK(st.fwd_b > 0, "forwarding B fired (%d cycles)", st.fwd_b);
    CHECK_EQ_U32((uint32_t)st.stalls, 0u, "ALU-to-ALU dependencies need no stall");
}

static void test_load_use(void) {
    test_suite("pipeline regression: load-use stalls once and forwards");

    uint32_t prog[] = {
        encode_i_type(OP_ADDI, REG_ZERO, REG_T0, 21),
        encode_i_type(OP_SW, REG_ZERO, REG_T0, 9),
        encode_i_type(OP_LW, REG_ZERO, REG_T1, 9),
        encode_r_type(REG_T1, REG_T1, REG_T2, 0, FUNCT_ADD),
        encode_i_type(OP_LW, REG_ZERO, REG_T3, 9),
        encode_i_type(OP_ADDI, REG_ZERO, REG_T4, 1),
        encode_r_type(REG_T3, REG_T4, REG_T5, 0, FUNCT_ADD),
        HALT_WORD,
    };
    iss_t ref;
    pipe_stats_t st;
    run_pipe(prog, (int)(sizeof prog / sizeof prog[0]), &ref, &st);
    CHECK_EQ_BIT(b.halt, 1, "halted");
    check_regs(&ref, "load-use");
    CHECK_EQ_U32((uint32_t)st.stalls, 1u, "only the adjacent load-use stalls");
}

int main(void) {
    test_raw_chain();
    test_load_use();
    return test_report();
}
