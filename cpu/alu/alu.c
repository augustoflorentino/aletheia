#include "alu.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

void alu_execute(bit *a, bit *b, bit *opcode, bit *result, alu_flags_t *flags) {
    bit add_result[32], sub_result[32];
    bit and_result[32], or_result[32], xor_result[32], not_result[32];
    bit shl_result[32], shr_result[32];
    bit carries[33];
    bit t0[32], t1[32], t2[32], t3[32], t4[32], t5[32];

    adder_32(a, b, add_result, carries);
    sub_32(a, b, sub_result);
    and_word_32(a, b, and_result);
    or_word_32(a, b, or_result);
    xor_word_32(a, b, xor_result);
    not_word_32(a, not_result);
    shl_32(a, b, shl_result);
    shr_32(a, b, shr_result);

    mux2_32(add_result, sub_result, opcode[0], t0);
    mux2_32(and_result, or_result, opcode[0], t1);
    mux2_32(xor_result, not_result, opcode[0], t2);
    mux2_32(shl_result, shr_result, opcode[0], t3);

    mux2_32(t0, t1, opcode[1], t4);
    mux2_32(t2, t3, opcode[1], t5);

    mux2_32(t4, t5, opcode[2], result);

    flags->zero = is_zero_32(result);
    flags->negative = result[31];
    flags->carry = carries[32];

    bit a_sign = a[31];
    bit b_sign = b[31];
    bit r_sign = result[31];
    bit same_sign = bit_eq(a_sign, b_sign);
    bit diff_result = NOT(bit_eq(a_sign, r_sign));
    flags->overflow = AND(same_sign, diff_result);
}
