#ifndef ALETHEIA_STUCK_H
#define ALETHEIA_STUCK_H

#include "../core/types.h"

int stuck_arm(long gate, bit value);
void stuck_new_cycle(void);
unsigned long stuck_gates_last_cycle(void);

#endif
