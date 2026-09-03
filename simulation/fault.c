#include "fault.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void flip(d_flip_flop_t *ff) {
    ff->master_q = (bit)!ff->q;
    ff->q = ff->master_q;
    ff->q_bar = (bit)!ff->q;
}

static int parse_target(const char *s, fault_t *f, const char **rest) {
    char *end;
    if (strncmp(s, "pc:", 3) == 0) {
        f->target = FAULT_PC;
        f->index = 0;
        *rest = s + 3;
        return 1;
    }
    if (s[0] == 'r' && s[1] >= '0' && s[1] <= '9') {
        f->target = FAULT_REG;
        f->index = (unsigned)strtoul(s + 1, &end, 10);
        *rest = end + 1;
        return *end == ':' && f->index > 0 && f->index < 32;
    }
    if (strncmp(s, "ram", 3) == 0) {
        f->target = FAULT_RAM;
        f->index = (unsigned)strtoul(s + 3, &end, 16);
        *rest = end + 1;
        return *end == ':' && end != s + 3 && f->index < RAM_WORDS;
    }
    return 0;
}

int fault_parse(const char *spec, fault_t *f) {
    char *end;
    f->cycle = strtoull(spec, &end, 10);
    if (end == spec || *end != ':') return 0;
    const char *rest;
    if (!parse_target(end + 1, f, &rest)) return 0;
    f->bit = (unsigned)strtoul(rest, &end, 10);
    return end != rest && *end == '\0' && f->bit < 32;
}

void fault_apply(board_t *b, const fault_t *f) {
    if (f->target == FAULT_PC) {
        flip(&b->pc.reg.bits[f->bit]);
        reg32_propagate(&b->pc.reg);
    } else if (f->target == FAULT_REG) {
        flip(&b->regs.regs[f->index].bits[f->bit]);
        reg32_propagate(&b->regs.regs[f->index]);
    } else {
        flip(&RAM_CELL(&b->ram, f->index)->bits[f->bit]);
    }
}

void fault_describe(const fault_t *f, char *out, int cap) {
    static const char *names[] = {"pc", "r", "ram"};
    if (f->target == FAULT_PC) snprintf(out, (size_t)cap, "cycle %llu: pc bit %u", f->cycle, f->bit);
    else if (f->target == FAULT_REG) snprintf(out, (size_t)cap, "cycle %llu: %s%u bit %u", f->cycle, names[1], f->index, f->bit);
    else snprintf(out, (size_t)cap, "cycle %llu: ram[0x%02x] bit %u", f->cycle, f->index, f->bit);
}
