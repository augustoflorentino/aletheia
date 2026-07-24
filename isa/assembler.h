#ifndef ALETHEIA_ASSEMBLER_H
#define ALETHEIA_ASSEMBLER_H

#include "encoding.h"

#define MAX_LABELS     64
#define MAX_LABEL_NAME 32
#define MAX_LINE_LEN   256

typedef struct {
    char name[MAX_LABEL_NAME];
    unsigned int address;
} label_t;

typedef struct {
    label_t labels[MAX_LABELS];
    int label_count;
    unsigned int current_address;
    unsigned int origin;
} assembler_t;

void assembler_init(assembler_t *as);
void assembler_reset(assembler_t *as);
void assembler_set_origin(assembler_t *as, unsigned int origin);

int assembler_add_label(assembler_t *as, const char *name, unsigned int address);
int assembler_find_label(assembler_t *as, const char *name, unsigned int *address);

int assembler_parse_register(const char *str);
int assembler_parse_immediate(const char *str);
int assembler_parse_address(const char *str);

unsigned int assembler_assemble_line(assembler_t *as, const char *line);
int assembler_assemble_file(assembler_t *as, const char *filename, unsigned int *output, int max_words);

#endif
