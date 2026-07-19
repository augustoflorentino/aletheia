#include "test.h"

#include "../synth/netlist.h"
#include "../cpu/alu/adder.h"

static bit node_vals[NL_MAX];
static int depth_scratch[NL_MAX];

static void nl_full_adder(int a, int b, int cin, int *sum, int *cout) {
    int s1 = nl_xor(a, b);
    int c1 = nl_and(a, b);
    *sum = nl_xor(s1, cin);
    int c2 = nl_and(s1, cin);
    *cout = nl_or(c1, c2);
}

static void test_full_adder_synth(void) {
    test_suite("synth: full_adder -> NAND netlist (equivalence + area)");

    nl_reset();
    int a = nl_input(), b = nl_input(), cin = nl_input();
    int sum, cout;
    nl_full_adder(a, b, cin, &sum, &cout);

    int va, vb, vc;
    for (va = 0; va <= 1; va++) {
        for (vb = 0; vb <= 1; vb++) {
            for (vc = 0; vc <= 1; vc++) {
                bit in[3] = {(bit)va, (bit)vb, (bit)vc};
                nl_eval(in, node_vals);
                bit rs, rc;
                full_adder((bit)va, (bit)vb, (bit)vc, &rs, &rc);
                CHECK_EQ_BIT(node_vals[sum], rs, "NAND netlist sum == full_adder");
                CHECK_EQ_BIT(node_vals[cout], rc, "NAND netlist cout == full_adder");
            }
        }
    }

    int depth = nl_critical_depth(depth_scratch);
    printf("  full_adder: %d NAND2, critical path %d NAND levels\n", nl_nands, depth);
    CHECK(nl_nands > 0, "netlist mapped to NAND2");
}

static void build_adder32(int *aidx, int *bidx, int *result, int *cout) {
    int i;
    for (i = 0; i < 32; i++) aidx[i] = nl_input();
    for (i = 0; i < 32; i++) bidx[i] = nl_input();
    int carry = nl_const0();
    for (i = 0; i < 32; i++) {
        int s, co;
        nl_full_adder(aidx[i], bidx[i], carry, &s, &co);
        result[i] = s;
        carry = co;
    }
    *cout = carry;
}

static void test_adder32_synth(void) {
    test_suite("synth: adder_32 -> NAND netlist (equivalence + area + timing)");

    nl_reset();
    int aidx[32], bidx[32], result[32], cout;
    build_adder32(aidx, bidx, result, &cout);

    int area = nl_nands;
    int depth = nl_critical_depth(depth_scratch);

    rng_seed(0x51A7u);
    int k;
    for (k = 0; k < 500; k++) {
        uint32_t va = rng_next(), vb = rng_next();
        bit in[64];
        int i;
        for (i = 0; i < 32; i++) in[i] = (va >> i) & 1u;
        for (i = 0; i < 32; i++) in[32 + i] = (vb >> i) & 1u;
        nl_eval(in, node_vals);

        uint32_t got = 0;
        for (i = 0; i < 32; i++) got |= ((uint32_t)node_vals[result[i]]) << i;
        CHECK_EQ_U32(got, va + vb, "NAND netlist adder_32 == a+b");

        bit ra[32], rb[32], rr[32], rc[33];
        to_bits_32(va, ra);
        to_bits_32(vb, rb);
        adder_32(ra, rb, rr, rc);
        CHECK_EQ_BIT(node_vals[cout], rc[32], "NAND netlist cout == adder_32 carry");
    }

    printf("  adder_32: %d NAND2 (area), critical path %d NAND levels\n", area, depth);
    printf("  assuming 1 NAND level = 100 ps -> Fmax ~ %.1f MHz\n",
           10000.0 / (double)depth);
    CHECK(area > 300, "adder_32 mapped (ripple ~ hundreds of NAND2)");
    CHECK(depth > 30, "critical path grows with the carry chain");
}

static void test_logic_synth(void) {
    test_suite("synth: AND 32-bit -> NAND netlist");

    nl_reset();
    int aidx[32], bidx[32], out[32];
    int i;
    for (i = 0; i < 32; i++) aidx[i] = nl_input();
    for (i = 0; i < 32; i++) bidx[i] = nl_input();
    for (i = 0; i < 32; i++) out[i] = nl_and(aidx[i], bidx[i]);

    rng_seed(0xA1D2u);
    int k;
    for (k = 0; k < 300; k++) {
        uint32_t va = rng_next(), vb = rng_next();
        bit in[64];
        for (i = 0; i < 32; i++) in[i] = (va >> i) & 1u;
        for (i = 0; i < 32; i++) in[32 + i] = (vb >> i) & 1u;
        nl_eval(in, node_vals);
        uint32_t got = 0;
        for (i = 0; i < 32; i++) got |= ((uint32_t)node_vals[out[i]]) << i;
        CHECK_EQ_U32(got, va & vb, "NAND netlist AND32 == a&b");
    }
    printf("  and32: %d NAND2 (2 per bit, expected 64)\n", nl_nands);
    CHECK_EQ_U32((uint32_t)nl_nands, 64u, "32-bit AND = 64 NAND2");
}

int main(void) {
    test_full_adder_synth();
    test_adder32_synth();
    test_logic_synth();
    return test_report();
}
