#include "test.h"
#include "refmodel.h"

#include "../cpu/alu/alu.h"

static const char *op_name(int op) {
    switch (op) {
        case ALU_OP_ADD: return "ADD";
        case ALU_OP_SUB: return "SUB";
        case ALU_OP_AND: return "AND";
        case ALU_OP_OR: return "OR";
        case ALU_OP_XOR: return "XOR";
        case ALU_OP_NOT: return "NOT";
        case ALU_OP_SHL: return "SHL";
        case ALU_OP_SHR: return "SHR";
        default: return "?";
    }
}

static void run_case(int op, uint32_t a, uint32_t b) {
    bit ba[32], bb[32], bop[8], br[32];
    alu_flags_t flags;

    to_bits_32(a, ba);
    to_bits_32(b, bb);
    to_bits_n((uint32_t)op, bop, 3);

    alu_execute(ba, bb, bop, br, &flags);

    uint32_t got = from_bits_32(br);
    uint32_t exp = ref_alu_result(op, a, b);
    CHECK(got == exp, "%s result a=0x%08x b=0x%08x: got 0x%08x exp 0x%08x",
          op_name(op), a, b, got, exp);

    CHECK_EQ_BIT(flags.zero, exp == 0, "zero flag");
    CHECK_EQ_BIT(flags.negative, (exp >> 31) & 1u, "negative flag");

    if (op == ALU_OP_ADD) {
        CHECK_EQ_BIT(flags.carry, ref_add_carry(a, b), "carry flag");
        CHECK_EQ_BIT(flags.overflow, ref_add_overflow(a, b), "overflow flag");
    }
}

static void test_alu_directed(void) {
    test_suite("alu: directed edge cases");

    uint32_t edges[] = {
        0u, 1u, 2u, 0xFFFFFFFFu, 0x7FFFFFFFu, 0x80000000u,
        0x0000FFFFu, 0xFFFF0000u, 0xAAAAAAAAu, 0x55555555u, 31u};
    int n = (int)(sizeof(edges) / sizeof(edges[0]));
    int op, i, j;
    for (op = 0; op < 8; op++) {
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                run_case(op, edges[i], edges[j]);
            }
        }
    }
}

static void test_alu_fuzz(void) {
    test_suite("alu: randomized fuzz");

    rng_seed(0xA1E7u);
    int op, k;
    for (op = 0; op < 8; op++) {
        for (k = 0; k < 2000; k++) {
            run_case(op, rng_next(), rng_next());
        }
    }
}

int main(void) {
    test_alu_directed();
    test_alu_fuzz();
    return test_report();
}
