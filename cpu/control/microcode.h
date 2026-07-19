#ifndef ALETHEIA_MICROCODE_H
#define ALETHEIA_MICROCODE_H

#include "../../core/types.h"

typedef struct {
    bit pc_write;
    bit pc_write_cond;
    bit pc_src[2];
    bit ir_write;
    bit mem_read;
    bit mem_write;
    bit mem_to_reg;
    bit alu_src_a;
    bit alu_src_b[2];
    bit alu_op[2];
    bit reg_write;
    bit reg_dst;
    bit mar_write;
    bit mdr_write;
} microcode_word_t;

typedef struct {
    bit opcode[6];
    bit step[3];
    microcode_word_t output;
} microcode_rom_t;

void microcode_init(microcode_rom_t *rom);
void microcode_reset(microcode_rom_t *rom);
void microcode_set_opcode(microcode_rom_t *rom, bit *opcode);
void microcode_set_step(microcode_rom_t *rom, bit *step);
void microcode_propagate(microcode_rom_t *rom);
microcode_word_t* microcode_get_output(microcode_rom_t *rom);

#endif
