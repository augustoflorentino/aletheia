#include "control_unit.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

void control_init(control_unit_t *cu) {
    dff_reset(&cu->state[0]);
    dff_reset(&cu->state[1]);
    dff_reset(&cu->state[2]);

    decoder_init(&cu->decoder);
    sequencer_init(&cu->sequencer);
    microcode_init(&cu->microcode);

    cu->alu_zero = 0;

    cu->state_out[0] = 0;
    cu->state_out[1] = 0;
    cu->state_out[2] = 0;

    cu->decoded_state[0] = 1;
    cu->decoded_state[1] = 0;
    cu->decoded_state[2] = 0;
    cu->decoded_state[3] = 0;
    cu->decoded_state[4] = 0;
    cu->decoded_state[5] = 0;
    cu->decoded_state[6] = 0;
    cu->decoded_state[7] = 0;

    cu->pc_write = 0;
    cu->pc_write_cond = 0;
    cu->pc_src[0] = 0;
    cu->pc_src[1] = 0;
    cu->ir_write = 0;
    cu->mem_read = 0;
    cu->mem_write = 0;
    cu->mem_to_reg = 0;
    cu->alu_src_a = 0;
    cu->alu_src_b[0] = 0;
    cu->alu_src_b[1] = 0;
    cu->alu_op[0] = 0;
    cu->alu_op[1] = 0;
    cu->alu_op[2] = 0;
    cu->alu_op[3] = 0;
    cu->reg_write = 0;
    cu->reg_dst = 0;
    cu->mar_write = 0;
    cu->mdr_write = 0;

    int i;
    i = 0; cu->instruction[i] = 0;
    i = 1; cu->instruction[i] = 0;
    i = 2; cu->instruction[i] = 0;
    i = 3; cu->instruction[i] = 0;
    i = 4; cu->instruction[i] = 0;
    i = 5; cu->instruction[i] = 0;
    i = 6; cu->instruction[i] = 0;
    i = 7; cu->instruction[i] = 0;
    i = 8; cu->instruction[i] = 0;
    i = 9; cu->instruction[i] = 0;
    i = 10; cu->instruction[i] = 0;
    i = 11; cu->instruction[i] = 0;
    i = 12; cu->instruction[i] = 0;
    i = 13; cu->instruction[i] = 0;
    i = 14; cu->instruction[i] = 0;
    i = 15; cu->instruction[i] = 0;
    i = 16; cu->instruction[i] = 0;
    i = 17; cu->instruction[i] = 0;
    i = 18; cu->instruction[i] = 0;
    i = 19; cu->instruction[i] = 0;
    i = 20; cu->instruction[i] = 0;
    i = 21; cu->instruction[i] = 0;
    i = 22; cu->instruction[i] = 0;
    i = 23; cu->instruction[i] = 0;
    i = 24; cu->instruction[i] = 0;
    i = 25; cu->instruction[i] = 0;
    i = 26; cu->instruction[i] = 0;
    i = 27; cu->instruction[i] = 0;
    i = 28; cu->instruction[i] = 0;
    i = 29; cu->instruction[i] = 0;
    i = 30; cu->instruction[i] = 0;
    i = 31; cu->instruction[i] = 0;
}

void control_reset(control_unit_t *cu) {
    control_init(cu);
}

void control_set_instruction(control_unit_t *cu, bit *instruction) {
    cu->instruction[0] = instruction[0];
    cu->instruction[1] = instruction[1];
    cu->instruction[2] = instruction[2];
    cu->instruction[3] = instruction[3];
    cu->instruction[4] = instruction[4];
    cu->instruction[5] = instruction[5];
    cu->instruction[6] = instruction[6];
    cu->instruction[7] = instruction[7];
    cu->instruction[8] = instruction[8];
    cu->instruction[9] = instruction[9];
    cu->instruction[10] = instruction[10];
    cu->instruction[11] = instruction[11];
    cu->instruction[12] = instruction[12];
    cu->instruction[13] = instruction[13];
    cu->instruction[14] = instruction[14];
    cu->instruction[15] = instruction[15];
    cu->instruction[16] = instruction[16];
    cu->instruction[17] = instruction[17];
    cu->instruction[18] = instruction[18];
    cu->instruction[19] = instruction[19];
    cu->instruction[20] = instruction[20];
    cu->instruction[21] = instruction[21];
    cu->instruction[22] = instruction[22];
    cu->instruction[23] = instruction[23];
    cu->instruction[24] = instruction[24];
    cu->instruction[25] = instruction[25];
    cu->instruction[26] = instruction[26];
    cu->instruction[27] = instruction[27];
    cu->instruction[28] = instruction[28];
    cu->instruction[29] = instruction[29];
    cu->instruction[30] = instruction[30];
    cu->instruction[31] = instruction[31];

    decoder_set_instruction(&cu->decoder, instruction);
}

void control_set_alu_zero(control_unit_t *cu, bit zero) {
    cu->alu_zero = zero;
}

void control_clock_rise(control_unit_t *cu) {
    dff_clock_rise(&cu->state[0]);
    dff_clock_rise(&cu->state[1]);
    dff_clock_rise(&cu->state[2]);

    sequencer_clock_rise(&cu->sequencer);
}

void control_clock_fall(control_unit_t *cu) {
    bit s0 = cu->state[0].q;
    bit s1 = cu->state[1].q;
    bit s2 = cu->state[2].q;

    bit is_fetch = AND(NOT(s2), AND(NOT(s1), NOT(s0)));
    bit is_decode = AND(NOT(s2), AND(NOT(s1), s0));
    bit is_execute = AND(NOT(s2), AND(s1, NOT(s0)));
    bit is_memory = AND(NOT(s2), AND(s1, s0));
    bit is_writeback = AND(s2, AND(NOT(s1), NOT(s0)));

    decoder_propagate(&cu->decoder);

    bit needs_memory = OR(cu->decoder.mem_read, cu->decoder.mem_write);
    bit needs_writeback = cu->decoder.mem_to_reg;

    bit next_from_fetch = is_fetch;
    bit next_from_decode = is_decode;
    bit next_from_execute = AND(is_execute, NOT(OR(needs_memory, cu->decoder.branch)));
    bit next_from_execute_mem = AND(is_execute, needs_memory);
    bit next_from_memory = AND(is_memory, needs_writeback);
    bit back_to_fetch = OR(AND(is_execute, OR(cu->decoder.branch, cu->decoder.jump)),
                           OR(AND(is_execute, NOT(OR(needs_memory, OR(cu->decoder.branch, cu->decoder.jump)))),
                           OR(AND(is_memory, NOT(needs_writeback)), is_writeback)));

    (void)next_from_execute;

    bit next0 = OR(next_from_fetch, next_from_execute_mem);
    bit next1 = OR(next_from_decode, next_from_execute_mem);
    bit next2 = next_from_memory;

    cu->state[0].d = mux2(next0, 0, back_to_fetch);
    cu->state[1].d = mux2(next1, 0, back_to_fetch);
    cu->state[2].d = mux2(next2, 0, back_to_fetch);

    dff_clock_fall(&cu->state[0]);
    dff_clock_fall(&cu->state[1]);
    dff_clock_fall(&cu->state[2]);

    sequencer_set_advance(&cu->sequencer, 1);
    sequencer_set_reset(&cu->sequencer, back_to_fetch);
    sequencer_clock_fall(&cu->sequencer);
}

void control_propagate(control_unit_t *cu) {
    cu->state_out[0] = cu->state[0].q;
    cu->state_out[1] = cu->state[1].q;
    cu->state_out[2] = cu->state[2].q;

    bit s0 = cu->state_out[0];
    bit s1 = cu->state_out[1];
    bit s2 = cu->state_out[2];
    bit not_s0 = NOT(s0);
    bit not_s1 = NOT(s1);
    bit not_s2 = NOT(s2);

    cu->decoded_state[0] = AND(not_s2, AND(not_s1, not_s0));
    cu->decoded_state[1] = AND(not_s2, AND(not_s1, s0));
    cu->decoded_state[2] = AND(not_s2, AND(s1, not_s0));
    cu->decoded_state[3] = AND(not_s2, AND(s1, s0));
    cu->decoded_state[4] = AND(s2, AND(not_s1, not_s0));

    bit is_fetch = cu->decoded_state[0];
    bit is_decode = cu->decoded_state[1];
    bit is_execute = cu->decoded_state[2];
    bit is_memory = cu->decoded_state[3];
    bit is_writeback = cu->decoded_state[4];
    (void)is_fetch; (void)is_decode; (void)is_execute;
    (void)is_memory; (void)is_writeback;

    decoder_propagate(&cu->decoder);
    sequencer_propagate(&cu->sequencer);

    bit step[3];
    sequencer_get_step(&cu->sequencer, step);
    microcode_set_opcode(&cu->microcode, cu->decoder.opcode);
    microcode_set_step(&cu->microcode, step);
    microcode_propagate(&cu->microcode);

    microcode_word_t *mc = microcode_get_output(&cu->microcode);

    cu->pc_write = OR(mc->pc_write, AND(mc->pc_write_cond, cu->alu_zero));
    cu->pc_write_cond = mc->pc_write_cond;
    cu->pc_src[0] = mc->pc_src[0];
    cu->pc_src[1] = mc->pc_src[1];
    cu->ir_write = mc->ir_write;
    cu->mem_read = mc->mem_read;
    cu->mem_write = mc->mem_write;
    cu->mem_to_reg = mc->mem_to_reg;
    cu->alu_src_a = mc->alu_src_a;
    cu->alu_src_b[0] = mc->alu_src_b[0];
    cu->alu_src_b[1] = mc->alu_src_b[1];
    cu->alu_op[0] = mc->alu_op[0];
    cu->alu_op[1] = mc->alu_op[1];
    cu->alu_op[2] = cu->decoder.alu_op[2];
    cu->alu_op[3] = cu->decoder.alu_op[3];
    cu->reg_write = mc->reg_write;
    cu->reg_dst = mc->reg_dst;
    cu->mar_write = mc->mar_write;
    cu->mdr_write = mc->mdr_write;
}

bit control_get_pc_write(control_unit_t *cu) {
    return cu->pc_write;
}

bit control_get_ir_write(control_unit_t *cu) {
    return cu->ir_write;
}

bit control_get_mem_read(control_unit_t *cu) {
    return cu->mem_read;
}

bit control_get_mem_write(control_unit_t *cu) {
    return cu->mem_write;
}

bit control_get_reg_write(control_unit_t *cu) {
    return cu->reg_write;
}

bit control_get_alu_src_a(control_unit_t *cu) {
    return cu->alu_src_a;
}

void control_get_alu_src_b(control_unit_t *cu, bit *out) {
    out[0] = cu->alu_src_b[0];
    out[1] = cu->alu_src_b[1];
}

void control_get_alu_op(control_unit_t *cu, bit *out) {
    out[0] = cu->alu_op[0];
    out[1] = cu->alu_op[1];
    out[2] = cu->alu_op[2];
    out[3] = cu->alu_op[3];
}
