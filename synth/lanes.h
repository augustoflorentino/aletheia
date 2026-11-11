#ifndef ALETHEIA_LANES_H
#define ALETHEIA_LANES_H

#include "../core/types.h"

#define LANE_COUNT    64
#define LANE_MACHINES (~1ull)

bit lanes_of(bit x);
int lanes_arm(int lane, long gate, int value);
void lanes_new_cycle(void);
unsigned long lanes_gates_last_cycle(void);
bit lanes_diverged(void);
void lanes_reset(void);

#endif
