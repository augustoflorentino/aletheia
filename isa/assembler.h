#ifndef ALETHEIA_ASSEMBLER_H
#define ALETHEIA_ASSEMBLER_H

#include "encoding.h"

#define MAX_LABELS     256
#define MAX_LABEL_NAME 32
#define MAX_LINE_LEN   256
#define MAX_ERROR_LEN  384

typedef struct {
    char name[MAX_LABEL_NAME];
    unsigned int address;
} label_t;

typedef enum {
    LINE_EMPTY,
    LINE_ORG,
    LINE_INSTR,
    LINE_WORD
} line_kind_t;

typedef struct {
    line_kind_t kind;
    char label[MAX_LABEL_NAME];
    unsigned int org;
    char text[MAX_LINE_LEN];
} asm_line_t;

typedef struct {
    label_t labels[MAX_LABELS];
    int label_count;
    unsigned int current_address;
    char error[MAX_ERROR_LEN];
} assembler_t;

void assembler_init(assembler_t *as);

int assembler_split_line(assembler_t *as, const char *line, asm_line_t *out);
int assembler_add_label(assembler_t *as, const char *name, unsigned int address);
int assembler_find_label(assembler_t *as, const char *name, unsigned int *address);
int assembler_parse_register(const char *str);

int assembler_assemble_line(assembler_t *as, const char *text, unsigned int *word);
int assembler_assemble_file(assembler_t *as, const char *filename, unsigned int *output, int max_words);

#endif
