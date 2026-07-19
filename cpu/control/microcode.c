#include "microcode.h"
#include "../../core/gates/gates.h"
#include "../../core/mux/mux.h"

void microcode_init(microcode_rom_t *rom) {
    rom->opcode[0] = 0; rom->opcode[1] = 0; rom->opcode[2] = 0;
    rom->opcode[3] = 0; rom->opcode[4] = 0; rom->opcode[5] = 0;
    rom->step[0] = 0; rom->step[1] = 0; rom->step[2] = 0;

    rom->output.pc_write = 0;
    rom->output.pc_write_cond = 0;
    rom->output.pc_src[0] = 0;
    rom->output.pc_src[1] = 0;
    rom->output.ir_write = 0;
    rom->output.mem_read = 0;
    rom->output.mem_write = 0;
    rom->output.mem_to_reg = 0;
    rom->output.alu_src_a = 0;
    rom->output.alu_src_b[0] = 0;
    rom->output.alu_src_b[1] = 0;
    rom->output.alu_op[0] = 0;
    rom->output.alu_op[1] = 0;
    rom->output.reg_write = 0;
    rom->output.reg_dst = 0;
    rom->output.mar_write = 0;
    rom->output.mdr_write = 0;
}

void microcode_reset(microcode_rom_t *rom) {
    microcode_init(rom);
}

void microcode_set_opcode(microcode_rom_t *rom, bit *opcode) {
    rom->opcode[0] = opcode[0];
    rom->opcode[1] = opcode[1];
    rom->opcode[2] = opcode[2];
    rom->opcode[3] = opcode[3];
    rom->opcode[4] = opcode[4];
    rom->opcode[5] = opcode[5];
}

void microcode_set_step(microcode_rom_t *rom, bit *step) {
    rom->step[0] = step[0];
    rom->step[1] = step[1];
    rom->step[2] = step[2];
}

void microcode_propagate(microcode_rom_t *rom) {
    bit s0 = rom->step[0];
    bit s1 = rom->step[1];
    bit s2 = rom->step[2];
    bit not_s0 = NOT(s0);
    bit not_s1 = NOT(s1);
    bit not_s2 = NOT(s2);

    bit step0 = AND(not_s2, AND(not_s1, not_s0));
    bit step1 = AND(not_s2, AND(not_s1, s0));
    bit step2 = AND(not_s2, AND(s1, not_s0));
    bit step3 = AND(not_s2, AND(s1, s0));
    bit step4 = AND(s2, AND(not_s1, not_s0));

    bit op_is_zero = NOR(rom->opcode[0], NOR(rom->opcode[1], NOR(rom->opcode[2],
                         NOR(rom->opcode[3], NOR(rom->opcode[4], rom->opcode[5])))));

    bit is_r_type = op_is_zero;

    bit is_lw = AND(AND(rom->opcode[5], rom->opcode[4]),
               AND(NOT(rom->opcode[3]), AND(NOT(rom->opcode[2]),
               AND(rom->opcode[1], rom->opcode[0]))));

    bit is_sw = AND(AND(rom->opcode[5], rom->opcode[4]),
               AND(NOT(rom->opcode[3]), AND(rom->opcode[2],
               AND(rom->opcode[1], rom->opcode[0]))));

    bit is_beq = AND(NOT(rom->opcode[5]), AND(NOT(rom->opcode[4]),
                AND(NOT(rom->opcode[3]), AND(rom->opcode[2],
                AND(NOT(rom->opcode[1]), NOT(rom->opcode[0]))))));

    bit is_j = AND(NOT(rom->opcode[5]), AND(NOT(rom->opcode[4]),
              AND(NOT(rom->opcode[3]), AND(NOT(rom->opcode[2]),
              AND(rom->opcode[1], NOT(rom->opcode[0]))))));

    rom->output.ir_write = step0;
    rom->output.pc_write = OR(step0, AND(step2, is_j));
    rom->output.pc_src[0] = AND(step2, is_beq);
    rom->output.pc_src[1] = AND(step2, is_j);

    rom->output.alu_src_a = OR(step2, OR(step3, step4));
    rom->output.alu_src_b[0] = step0;
    rom->output.alu_src_b[1] = OR(step0, step1);

    rom->output.mem_read = OR(step0, AND(step3, is_lw));
    rom->output.mem_write = AND(step3, is_sw);
    rom->output.mdr_write = AND(step3, is_lw);

    rom->output.mar_write = OR(step1, AND(step2, OR(is_lw, is_sw)));

    rom->output.alu_op[0] = AND(step2, is_beq);
    rom->output.alu_op[1] = AND(step2, is_r_type);

    rom->output.reg_write = OR(AND(step3, is_r_type), AND(step4, is_lw));
    rom->output.reg_dst = AND(step3, is_r_type);
    rom->output.mem_to_reg = AND(step4, is_lw);

    rom->output.pc_write_cond = AND(step2, is_beq);
}

microcode_word_t* microcode_get_output(microcode_rom_t *rom) {
    return &rom->output;
}
