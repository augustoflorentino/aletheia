#ifndef ALETHEIA_DISASSEMBLER_H
#define ALETHEIA_DISASSEMBLER_H

#include "encoding.h"

#define MAX_DISASM_LEN 64

void disassemble_instruction(unsigned int word, char *output, int max_len);
const char *get_register_name(int reg);
const char *get_opcode_name(unsigned int opcode, unsigned int funct);

#endif
