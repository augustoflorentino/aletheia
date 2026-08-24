#ifndef ALETHEIA_OPTABLE_H
#define ALETHEIA_OPTABLE_H

typedef enum {
    FMT_R3,
    FMT_SHIFT,
    FMT_SHIFTV,
    FMT_JR,
    FMT_JALR,
    FMT_IMM_S,
    FMT_IMM_U,
    FMT_LUI,
    FMT_MEM,
    FMT_BRANCH,
    FMT_JUMP,
    FMT_NONE
} op_format_t;

typedef struct {
    const char *name;
    unsigned int opcode;
    unsigned int funct;
    op_format_t format;
} op_info_t;

const op_info_t *optable_find_name(const char *name);
const op_info_t *optable_find_word(unsigned int word);

#endif
