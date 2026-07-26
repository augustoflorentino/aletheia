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
    bit is_sub = AND(opcode[0], NOR(opcode[1], opcode[2]));
    bit b_sign = XOR(b[31], is_sub);
    bit r_sign = result[31];
    bit same_sign = bit_eq(a_sign, b_sign);
    bit diff_result = NOT(bit_eq(a_sign, r_sign));
    flags->overflow = AND(same_sign, diff_result);
}

static void sign_extend_16(bit *imm, bit *out) {
    out[0] = imm[0]; out[1] = imm[1]; out[2] = imm[2]; out[3] = imm[3];
    out[4] = imm[4]; out[5] = imm[5]; out[6] = imm[6]; out[7] = imm[7];
    out[8] = imm[8]; out[9] = imm[9]; out[10] = imm[10]; out[11] = imm[11];
    out[12] = imm[12]; out[13] = imm[13]; out[14] = imm[14]; out[15] = imm[15];
    out[16] = imm[15]; out[17] = imm[15]; out[18] = imm[15]; out[19] = imm[15];
    out[20] = imm[15]; out[21] = imm[15]; out[22] = imm[15]; out[23] = imm[15];
    out[24] = imm[15]; out[25] = imm[15]; out[26] = imm[15]; out[27] = imm[15];
    out[28] = imm[15]; out[29] = imm[15]; out[30] = imm[15]; out[31] = imm[15];
}

static void zero_extend_16(bit *imm, bit *out) {
    out[0] = imm[0]; out[1] = imm[1]; out[2] = imm[2]; out[3] = imm[3];
    out[4] = imm[4]; out[5] = imm[5]; out[6] = imm[6]; out[7] = imm[7];
    out[8] = imm[8]; out[9] = imm[9]; out[10] = imm[10]; out[11] = imm[11];
    out[12] = imm[12]; out[13] = imm[13]; out[14] = imm[14]; out[15] = imm[15];
    out[16] = 0; out[17] = 0; out[18] = 0; out[19] = 0;
    out[20] = 0; out[21] = 0; out[22] = 0; out[23] = 0;
    out[24] = 0; out[25] = 0; out[26] = 0; out[27] = 0;
    out[28] = 0; out[29] = 0; out[30] = 0; out[31] = 0;
}

static void shift_amount(bit *five, bit *out) {
    out[0] = five[0]; out[1] = five[1]; out[2] = five[2]; out[3] = five[3]; out[4] = five[4];
    out[5] = 0; out[6] = 0; out[7] = 0; out[8] = 0; out[9] = 0;
    out[10] = 0; out[11] = 0; out[12] = 0; out[13] = 0; out[14] = 0;
    out[15] = 0; out[16] = 0; out[17] = 0; out[18] = 0; out[19] = 0;
    out[20] = 0; out[21] = 0; out[22] = 0; out[23] = 0; out[24] = 0;
    out[25] = 0; out[26] = 0; out[27] = 0; out[28] = 0; out[29] = 0;
    out[30] = 0; out[31] = 0;
}

void alu_select_operands(const alu_operand_sel_t *sel, bit *imm16, bit *shamt5,
                         bit *rs_val, bit *rt_val, bit *a, bit *b) {
    bit imm_s[32], imm_z[32], imm[32], shamt_w[32], b_reg_imm[32], b_shift[32], a_sel[32], b_sel[32];
    bit lui_shift[5] = {0, 0, 0, 0, 1};
    bit sixteen[32];
    shift_amount(lui_shift, sixteen);
    sign_extend_16(imm16, imm_s);
    zero_extend_16(imm16, imm_z);
    mux2_32(imm_s, imm_z, sel->imm_zero_ext, imm);
    mux2_32(rt_val, imm, sel->alu_src, b_reg_imm);
    shift_amount(shamt5, shamt_w);
    mux2_32(rs_val, shamt_w, sel->shift_shamt, b_shift);
    mux2_32(b_reg_imm, b_shift, sel->shift_rt, b_sel);
    mux2_32(rs_val, rt_val, sel->shift_rt, a_sel);
    mux2_32(a_sel, imm_z, sel->lui, a);
    mux2_32(b_sel, sixteen, sel->lui, b);
}

static void flag_word(bit value, bit *out) {
    out[0] = value;
    out[1] = 0; out[2] = 0; out[3] = 0; out[4] = 0; out[5] = 0; out[6] = 0; out[7] = 0;
    out[8] = 0; out[9] = 0; out[10] = 0; out[11] = 0; out[12] = 0; out[13] = 0; out[14] = 0;
    out[15] = 0; out[16] = 0; out[17] = 0; out[18] = 0; out[19] = 0; out[20] = 0; out[21] = 0;
    out[22] = 0; out[23] = 0; out[24] = 0; out[25] = 0; out[26] = 0; out[27] = 0; out[28] = 0;
    out[29] = 0; out[30] = 0; out[31] = 0;
}

void alu_execute_ext(bit *a, bit *b, bit *opcode, bit *result, alu_flags_t *flags) {
    bit base[32], sra_result[32], slt_result[32], sltu_result[32], nor_result[32];
    bit g0[32], g1[32], ext[32];
    bit eq, lt_u, gt_u;

    alu_execute(a, b, opcode, base, flags);
    sra_32(a, b, sra_result);
    cmp_32(a, b, &eq, &lt_u, &gt_u);
    bit signs_differ = XOR(a[31], b[31]);
    flag_word(mux2(lt_u, a[31], signs_differ), slt_result);
    flag_word(lt_u, sltu_result);
    nor_word_32(a, b, nor_result);

    mux2_32(sra_result, slt_result, opcode[0], g0);
    mux2_32(sltu_result, nor_result, opcode[0], g1);
    mux2_32(g0, g1, opcode[1], ext);
    mux2_32(base, ext, opcode[3], result);

    flags->zero = is_zero_32(result);
    flags->negative = result[31];
}
