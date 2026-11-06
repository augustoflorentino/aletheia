#ifndef ALETHEIA_POWER_TRACE_H
#define ALETHEIA_POWER_TRACE_H

#include <stdio.h>
#include "../system/board.h"

typedef struct {
    FILE *out;
    board_t prev;
} power_trace_t;

int power_trace_open(power_trace_t *pt, const char *path, const board_t *start);
void power_trace_sample(power_trace_t *pt, const board_t *now, unsigned long long cycle);
void power_trace_close(power_trace_t *pt);

#endif
