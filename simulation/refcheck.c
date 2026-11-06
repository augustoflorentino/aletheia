#include "refcheck.h"
#include <string.h>

#define REFCHECK_SHOWN 16

static uint32_t word_of(const bit *bits) {
    uint32_t v = 0;
    for (int i = 0; i < 32; i++) v |= (uint32_t)(bits[i] & 1) << i;
    return v;
}

static uint32_t gate_ram(simulator_t *sim, unsigned addr) {
    bit a[32] = {0}, out[32];
    for (int b = 0; b < 32; b++) a[b] = (bit)((addr >> b) & 1u);
    ram_set_addr(&sim->board.ram, a);
    ram_set_cs(&sim->board.ram, 1);
    ram_propagate(&sim->board.ram);
    ram_read(&sim->board.ram, out);
    return word_of(out);
}

static int report(FILE *log, int diffs, const char *what, unsigned idx, uint32_t gate, uint32_t iss) {
    if (log && diffs < REFCHECK_SHOWN) fprintf(log, "  %s[%u]: gates 0x%08x  ref 0x%08x\n", what, idx, gate, iss);
    return diffs + 1;
}

static int compare_output(const iss_t *ref, const char *out, int out_len, FILE *log) {
    int same = out_len == ref->out_len && memcmp(out, ref->out, (size_t)out_len) == 0;
    if (!same && log) fprintf(log, "  uart: gates %d bytes, ref %d bytes, contents differ\n", out_len, ref->out_len);
    return !same;
}

int refcheck_compare(simulator_t *sim, iss_t *ref, const char *out, int out_len, const uint32_t *rom,
                     const char *keys, unsigned long long max_steps, FILE *log) {
    iss_reset(ref);
    ref->mmu_allowed = sim->board.config.strap[STRAP_MMU].q;
    iss_type(ref, keys ? keys : "");
    iss_execute(ref, rom, max_steps);
    if (log) fprintf(log, "\nReference model: %llu instructions, %s\n", ref->steps, ref->halted ? "halted" : "did not halt");
    int diffs = compare_output(ref, out, out_len, log);
    regfile_propagate(&sim->board.regs);
    for (unsigned r = 1; r < 32; r++) {
        uint32_t g = word_of(sim->board.regs.regs[r].data_out);
        if (g != ref->reg[r]) diffs = report(log, diffs, "reg", r, g, ref->reg[r]);
    }
    for (unsigned a = 0; a < ISS_WORDS; a++) {
        uint32_t g = gate_ram(sim, a);
        if (g != ref->ram[a]) diffs = report(log, diffs, "ram", a, g, ref->ram[a]);
    }
    if ((int)sim->board.halt != ref->halted) diffs = report(log, diffs, "halt", 0, sim->board.halt, (uint32_t)ref->halted);
    if (log) fprintf(log, "Reference check: %s (%d difference%s)\n", diffs ? "MISMATCH" : "match", diffs, diffs == 1 ? "" : "s");
    return diffs;
}
