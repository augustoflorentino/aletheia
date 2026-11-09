#include "stuck.h"

int stuck_arm(long gate, bit value) {
    (void)gate;
    (void)value;
    return -1;
}

void stuck_new_cycle(void) {
}

unsigned long stuck_gates_last_cycle(void) {
    return 0;
}
