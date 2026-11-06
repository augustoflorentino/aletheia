#ifndef ALETHEIA_REFCHECK_H
#define ALETHEIA_REFCHECK_H

#include "simulator.h"
#include "../ref/iss.h"
#include <stdint.h>
#include <stdio.h>

int refcheck_compare(simulator_t *sim, iss_t *ref, const char *out, int out_len, const uint32_t *rom,
                     const char *keys, unsigned long long max_steps, FILE *log);

#endif
