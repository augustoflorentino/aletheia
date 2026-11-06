#include "power_trace.h"
#include <string.h>

int power_trace_open(power_trace_t *pt, const char *path, const board_t *start) {
    pt->out = fopen(path, "w");
    if (!pt->out) return -1;
    memcpy(&pt->prev, start, sizeof pt->prev);
    fprintf(pt->out, "cycle,toggles\n");
    return 0;
}

static unsigned long hamming(const unsigned char *a, const unsigned char *b, size_t n) {
    unsigned long d = 0;
    for (size_t i = 0; i < n; i++) d += (unsigned long)__builtin_popcount((unsigned)(a[i] ^ b[i]));
    return d;
}

void power_trace_sample(power_trace_t *pt, const board_t *now, unsigned long long cycle) {
    unsigned long toggles = hamming((const unsigned char *)&pt->prev, (const unsigned char *)now, sizeof *now);
    fprintf(pt->out, "%llu,%lu\n", cycle, toggles);
    memcpy(&pt->prev, now, sizeof pt->prev);
}

void power_trace_close(power_trace_t *pt) {
    if (!pt || !pt->out) return;
    fclose(pt->out);
    pt->out = NULL;
}
