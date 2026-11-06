#ifndef ALETHEIA_SNAPSHOT_H
#define ALETHEIA_SNAPSHOT_H

#include "simulator.h"
#include <stddef.h>

size_t snapshot_size(void);
int snapshot_write(const simulator_t *sim, unsigned char *out, size_t out_len);
int snapshot_read(simulator_t *sim, const unsigned char *in, size_t in_len);

#endif
