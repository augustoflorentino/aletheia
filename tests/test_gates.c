#include "test.h"

#include "../core/gates/gates.h"
#include "../cpu/alu/adder.h"

static void test_basic_gates(void) {
    test_suite("gates: truth tables");

    int a, b;
    for (a = 0; a <= 1; a++) {
        for (b = 0; b <= 1; b++) {
            CHECK_EQ_BIT(AND(a, b), a & b, "AND");
            CHECK_EQ_BIT(OR(a, b), a | b, "OR");
            CHECK_EQ_BIT(XOR(a, b), a ^ b, "XOR");
            CHECK_EQ_BIT(NAND(a, b), !(a & b), "NAND");
            CHECK_EQ_BIT(NOR(a, b), !(a | b), "NOR");
        }
    }
    CHECK_EQ_BIT(NOT(0), 1, "NOT(0)");
    CHECK_EQ_BIT(NOT(1), 0, "NOT(1)");
}

static void test_adders(void) {
    test_suite("gates: half/full adder");

    int a, b, cin;
    for (a = 0; a <= 1; a++) {
        for (b = 0; b <= 1; b++) {
            bit sum, carry;
            half_adder(a, b, &sum, &carry);
            CHECK_EQ_BIT(sum, a ^ b, "half_adder sum");
            CHECK_EQ_BIT(carry, a & b, "half_adder carry");

            for (cin = 0; cin <= 1; cin++) {
                bit fs, fc;
                full_adder(a, b, cin, &fs, &fc);
                int total = a + b + cin;
                CHECK_EQ_BIT(fs, total & 1, "full_adder sum");
                CHECK_EQ_BIT(fc, (total >> 1) & 1, "full_adder carry");
            }
        }
    }
}

static bit ref_carry(uint32_t a, uint32_t b) {
    return (bit)((((uint64_t)a + (uint64_t)b) >> 32) & 1u);
}

static void test_adder_32_exhaustive_carry(void) {
    test_suite("gates: adder_32 vs uint");

    uint32_t vectors[] = {
        0u, 1u, 2u, 0xFFFFFFFFu, 0x7FFFFFFFu, 0x80000000u,
        0xDEADBEEFu, 0x12345678u, 0xFFFF0000u, 0x0000FFFFu
    };
    int n = (int)(sizeof(vectors) / sizeof(vectors[0]));
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            bit a[32], b[32], r[32], carries[33];
            to_bits_32(vectors[i], a);
            to_bits_32(vectors[j], b);
            adder_32(a, b, r, carries);
            CHECK_EQ_U32(from_bits_32(r), vectors[i] + vectors[j], "adder_32 sum");
            CHECK_EQ_BIT(carries[32], ref_carry(vectors[i], vectors[j]), "adder_32 carry");
        }
    }
}

int main(void) {
    test_basic_gates();
    test_adders();
    test_adder_32_exhaustive_carry();
    return test_report();
}
