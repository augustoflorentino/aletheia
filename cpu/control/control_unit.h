#ifndef ALETHEIA_CONTROL_UNIT_H
#define ALETHEIA_CONTROL_UNIT_H

#include "../../core/types.h"
#include "../../core/sequential/d_flip_flop.h"
#include "decoder.h"
#include "sequencer.h"
#include "microcode.h"

typedef enum {
    STATE_FETCH = 0,
    STATE_DECODE = 1,
    STATE_EXECUTE = 2,
    STATE_MEMORY = 3,
    STATE_WRITEBACK = 4
} cpu_state_t;

typedef struct {
    d_flip_flop_t state[3];

    instruction_decoder_t decoder;
    sequencer_t sequencer;
    microcode_rom_t microcode;

    bit instruction[32];
    bit alu_zero;

    bit state_out[3];
    bit decoded_state[8];

    bit pc_write;
    bit pc_write_cond;
    bit pc_src[2];
    bit ir_write;
    bit mem_read;
    bit mem_write;
    bit mem_to_reg;
    bit alu_src_a;
    bit alu_src_b[2];
    bit alu_op[4];
    bit reg_write;
    bit reg_dst;
    bit mar_write;
    bit mdr_write;
} control_unit_t;

void control_init(control_unit_t *cu);
void control_reset(control_unit_t *cu);
void control_set_instruction(control_unit_t *cu, bit *instruction);
void control_set_alu_zero(control_unit_t *cu, bit zero);
void control_clock_rise(control_unit_t *cu);
void control_clock_fall(control_unit_t *cu);
void control_propagate(control_unit_t *cu);

bit control_get_pc_write(control_unit_t *cu);
bit control_get_ir_write(control_unit_t *cu);
bit control_get_mem_read(control_unit_t *cu);
bit control_get_mem_write(control_unit_t *cu);
bit control_get_reg_write(control_unit_t *cu);
bit control_get_alu_src_a(control_unit_t *cu);
void control_get_alu_src_b(control_unit_t *cu, bit *out);
void control_get_alu_op(control_unit_t *cu, bit *out);

#endif
