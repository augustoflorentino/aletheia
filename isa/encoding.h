#ifndef ALETHEIA_ENCODING_H
#define ALETHEIA_ENCODING_H

#include "opcodes.h"

unsigned int encode_r_type(unsigned int rs, unsigned int rt, unsigned int rd, unsigned int shamt, unsigned int funct);
unsigned int encode_i_type(unsigned int opcode, unsigned int rs, unsigned int rt, unsigned int immediate);
unsigned int encode_j_type(unsigned int opcode, unsigned int address);

void decode_instruction(unsigned int word, instruction_t *instr);
instr_type_t get_instruction_type(unsigned int opcode);

unsigned int extract_opcode(unsigned int word);
unsigned int extract_rs(unsigned int word);
unsigned int extract_rt(unsigned int word);
unsigned int extract_rd(unsigned int word);
unsigned int extract_shamt(unsigned int word);
unsigned int extract_funct(unsigned int word);
unsigned int extract_immediate(unsigned int word);
unsigned int extract_address(unsigned int word);

int sign_extend_16(unsigned int value);
int sign_extend_26(unsigned int value);

#endif
