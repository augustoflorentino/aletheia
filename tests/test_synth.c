#include "test.h"
#include "refmodel.h"

#include <stdlib.h>
#include "../synth/netlist.h"
#include "../cpu/alu/adder.h"
#include "../cpu/alu/alu.h"
#include "../cpu/alu/logic_unit.h"

static unsigned char *vals;

static void eval(const unsigned char *in) {
    vals = realloc(vals, (size_t)nl_count());
    nl_eval(in, vals);
}

static uint32_t read_word(const bit *nodes) {
    uint32_t v = 0;
    for (int i = 0; i < 32; i++) v |= (uint32_t)vals[nodes[i]] << i;
    return v;
}

static void inputs(bit *nodes, int n) {
    for (int i = 0; i < n; i++) nodes[i] = nl_input();
}

static void pack(unsigned char *in, int at, uint32_t v, int width) {
    for (int i = 0; i < width; i++) in[at + i] = (v >> i) & 1u;
}

static void test_full_adder_synth(void) {
    test_suite("synth: real full_adder -> NAND netlist (equivalence + area)");

    nl_reset();
    bit a = nl_input(), b = nl_input(), cin = nl_input(), sum, cout;
    full_adder(a, b, cin, &sum, &cout);

    for (int v = 0; v < 8; v++) {
        unsigned char in[3] = {v & 1, (v >> 1) & 1, (v >> 2) & 1};
        eval(in);
        int total = in[0] + in[1] + in[2];
        CHECK_EQ_BIT(vals[sum], (bit)(total & 1), "sum");
        CHECK_EQ_BIT(vals[cout], (bit)(total >> 1), "carry");
    }
    bit outs[2] = {sum, cout};
    printf("  full_adder: %d NAND2, critical path %d NAND levels\n",
           nl_cone_nands(outs, 2), nl_depth_of(outs, 2));
    CHECK(nl_cone_nands(outs, 2) > 0, "mapped to NAND2");
}

static void test_adder32_synth(void) {
    test_suite("synth: real adder_32 -> NAND netlist (equivalence + area + timing)");

    nl_reset();
    bit a[32], b[32], r[32], c[33];
    inputs(a, 32);
    inputs(b, 32);
    adder_32(a, b, r, c);

    rng_seed(0x51A7u);
    for (int k = 0; k < 500; k++) {
        uint32_t va = rng_next(), vb = rng_next();
        unsigned char in[64];
        pack(in, 0, va, 32);
        pack(in, 32, vb, 32);
        eval(in);
        CHECK_EQ_U32(read_word(r), va + vb, "netlist adder_32 == a+b");
        CHECK_EQ_BIT(vals[c[32]], (bit)(((uint64_t)va + vb) >> 32), "carry out");
    }
    int area = nl_cone_nands(r, 32);
    int depth = nl_depth_of(r, 32);
    printf("  adder_32: %d NAND2 (area), critical path %d NAND levels\n", area, depth);
    CHECK(area > 300, "ripple adder is hundreds of NAND2");
    CHECK(depth > 32, "critical path grows with the carry chain");
}

static void test_logic_synth(void) {
    test_suite("synth: real and_word_32 -> NAND netlist");

    nl_reset();
    bit a[32], b[32], out[32];
    inputs(a, 32);
    inputs(b, 32);
    and_word_32(a, b, out);

    rng_seed(0xA1D2u);
    for (int k = 0; k < 300; k++) {
        uint32_t va = rng_next(), vb = rng_next();
        unsigned char in[64];
        pack(in, 0, va, 32);
        pack(in, 32, vb, 32);
        eval(in);
        CHECK_EQ_U32(read_word(out), va & vb, "netlist AND32 == a&b");
    }
    printf("  and32: %d NAND2\n", nl_cone_nands(out, 32));
    CHECK_EQ_U32((uint32_t)nl_cone_nands(out, 32), 64u, "32-bit AND = 64 NAND2");
}

static uint32_t ref_alu_ext(int op, uint32_t a, uint32_t b) {
    switch (op) {
        case ALU_OP_SRA: return (uint32_t)((int32_t)a >> (b & 31u));
        case ALU_OP_SLT: return (int32_t)a < (int32_t)b;
        case ALU_OP_SLTU: return a < b;
        case ALU_OP_NOR: return ~(a | b);
        default: return ref_alu_result(op, a, b);
    }
}

static void test_alu_synth(void) {
    test_suite("synth: real ALU -> NAND netlist (all operations)");

    nl_reset();
    bit a[32], b[32], op[4], r[32];
    alu_flags_t flags;
    inputs(a, 32);
    inputs(b, 32);
    inputs(op, 4);
    alu_execute_ext(a, b, op, r, &flags);

    int ops[] = {ALU_OP_ADD, ALU_OP_SUB, ALU_OP_AND, ALU_OP_OR, ALU_OP_XOR,
                 ALU_OP_SHL, ALU_OP_SHR, ALU_OP_SRA, ALU_OP_SLT, ALU_OP_SLTU, ALU_OP_NOR};
    rng_seed(0xA1Bu);
    for (int k = 0; k < 400; k++) {
        int o = ops[k % (int)(sizeof ops / sizeof ops[0])];
        uint32_t va = rng_next(), vb = rng_next();
        unsigned char in[68];
        pack(in, 0, va, 32);
        pack(in, 32, vb, 32);
        pack(in, 64, (uint32_t)o, 4);
        eval(in);
        CHECK_EQ_U32(read_word(r), ref_alu_ext(o, va, vb), "netlist ALU == reference");
    }
    printf("  alu: %d NAND2 (area), critical path %d NAND levels\n",
           nl_cone_nands(r, 32), nl_depth_of(r, 32));
}

int main(void) {
    test_full_adder_synth();
    test_adder32_synth();
    test_logic_synth();
    test_alu_synth();
    return test_report();
}
