#ifndef ALETHEIA_FAULT_H
#define ALETHEIA_FAULT_H

#include "../system/board.h"

#define MAX_FAULTS 16

typedef enum { FAULT_PC,
               FAULT_REG,
               FAULT_RAM } fault_target_t;

typedef struct {
    unsigned long long cycle;
    fault_target_t target;
    unsigned index;
    unsigned bit;
} fault_t;

int fault_parse(const char *spec, fault_t *f);
void fault_apply(board_t *b, const fault_t *f);
void fault_describe(const fault_t *f, char *out, int cap);

#endif
