#include "debugger.h"

void debugger_init(debugger_t *dbg) {
    dbg->breakpoint_count = 0;
    dbg->watchpoint_count = 0;
    dbg->single_step = 0;
    dbg->step_budget = 0;
    dbg->running = 1;
    dbg->skip_pending = 0;
    dbg->skip_pc = 0;
    dbg->pc_bits = 0;
    dbg->halt_callback = 0;
    dbg->halt_context = 0;

    for (int i = 0; i < MAX_BREAKPOINTS; i++) {
        dbg->breakpoints[i].address = 0;
        dbg->breakpoints[i].enabled = 0;
        dbg->breakpoints[i].hit_count = 0;
    }
    for (int i = 0; i < MAX_WATCHPOINTS; i++) {
        dbg->watchpoints[i].signal_ptr = 0;
        dbg->watchpoints[i].trigger_value = 0;
        dbg->watchpoints[i].last_value = 0;
        dbg->watchpoints[i].enabled = 0;
        dbg->watchpoints[i].hit_count = 0;
    }
}

void debugger_reset(debugger_t *dbg) {
    for (int i = 0; i < MAX_BREAKPOINTS; i++) {
        dbg->breakpoints[i].hit_count = 0;
    }
    for (int i = 0; i < MAX_WATCHPOINTS; i++) {
        dbg->watchpoints[i].hit_count = 0;
    }
    dbg->running = 1;
    dbg->skip_pending = 0;
}

void debugger_set_pc_bits(debugger_t *dbg, bit *pc) {
    dbg->pc_bits = pc;
}

unsigned int debugger_pc(debugger_t *dbg) {
    unsigned int pc = 0;
    for (int i = 0; dbg->pc_bits != 0 && i < 32; i++) {
        pc |= (unsigned int)(dbg->pc_bits[i] & 1) << i;
    }
    return pc;
}

void debugger_set_halt_callback(debugger_t *dbg, void (*cb)(void *), void *ctx) {
    dbg->halt_callback = cb;
    dbg->halt_context = ctx;
}

int debugger_add_breakpoint(debugger_t *dbg, unsigned int address) {
    if (dbg->breakpoint_count >= MAX_BREAKPOINTS) {
        return -1;
    }
    int idx = dbg->breakpoint_count;
    dbg->breakpoints[idx].address = address;
    dbg->breakpoints[idx].enabled = 1;
    dbg->breakpoints[idx].hit_count = 0;
    dbg->breakpoint_count++;
    return idx;
}

void debugger_remove_breakpoint(debugger_t *dbg, int index) {
    if (index >= 0 && index < dbg->breakpoint_count) {
        for (int i = index; i < dbg->breakpoint_count - 1; i++) {
            dbg->breakpoints[i] = dbg->breakpoints[i + 1];
        }
        dbg->breakpoint_count--;
    }
}

void debugger_enable_breakpoint(debugger_t *dbg, int index, int enable) {
    if (index >= 0 && index < dbg->breakpoint_count) {
        dbg->breakpoints[index].enabled = enable;
    }
}

void debugger_clear_breakpoints(debugger_t *dbg) {
    dbg->breakpoint_count = 0;
}

int debugger_add_watchpoint(debugger_t *dbg, bit *signal, bit trigger_value) {
    if (dbg->watchpoint_count >= MAX_WATCHPOINTS) {
        return -1;
    }
    int idx = dbg->watchpoint_count;
    dbg->watchpoints[idx].signal_ptr = signal;
    dbg->watchpoints[idx].trigger_value = trigger_value;
    dbg->watchpoints[idx].last_value = signal != 0 ? *signal : 0;
    dbg->watchpoints[idx].enabled = 1;
    dbg->watchpoints[idx].hit_count = 0;
    dbg->watchpoint_count++;
    return idx;
}

void debugger_remove_watchpoint(debugger_t *dbg, int index) {
    if (index >= 0 && index < dbg->watchpoint_count) {
        for (int i = index; i < dbg->watchpoint_count - 1; i++) {
            dbg->watchpoints[i] = dbg->watchpoints[i + 1];
        }
        dbg->watchpoint_count--;
    }
}

void debugger_enable_watchpoint(debugger_t *dbg, int index, int enable) {
    if (index >= 0 && index < dbg->watchpoint_count) {
        dbg->watchpoints[index].enabled = enable;
    }
}

void debugger_clear_watchpoints(debugger_t *dbg) {
    dbg->watchpoint_count = 0;
}

void debugger_set_single_step(debugger_t *dbg, int enable) {
    dbg->single_step = enable;
}

static void resume(debugger_t *dbg) {
    dbg->running = 1;
    dbg->skip_pending = 1;
    dbg->skip_pc = debugger_pc(dbg);
}

void debugger_continue(debugger_t *dbg) {
    resume(dbg);
    dbg->single_step = 0;
}

void debugger_step(debugger_t *dbg) {
    resume(dbg);
    dbg->single_step = 1;
    dbg->step_budget = 1;
}

static int check_breakpoints(debugger_t *dbg) {
    if (dbg->pc_bits == 0) {
        return 0;
    }
    unsigned int pc = debugger_pc(dbg);
    if (dbg->skip_pending && pc == dbg->skip_pc) {
        return 0;
    }
    dbg->skip_pending = 0;
    int hit = 0;
    for (int i = 0; i < dbg->breakpoint_count; i++) {
        if (dbg->breakpoints[i].enabled && dbg->breakpoints[i].address == pc) {
            dbg->breakpoints[i].hit_count++;
            hit = 1;
        }
    }
    return hit;
}

static int check_watchpoints(debugger_t *dbg) {
    int hit = 0;
    for (int i = 0; i < dbg->watchpoint_count; i++) {
        watchpoint_t *w = &dbg->watchpoints[i];
        if (!w->enabled || w->signal_ptr == 0) {
            continue;
        }
        bit now = *w->signal_ptr;
        if (now != w->last_value && now == w->trigger_value) {
            w->hit_count++;
            hit = 1;
        }
        w->last_value = now;
    }
    return hit;
}

static int check_step(debugger_t *dbg) {
    if (!dbg->single_step) {
        return 0;
    }
    if (dbg->step_budget > 0) {
        dbg->step_budget--;
        return 0;
    }
    return 1;
}

int debugger_check(debugger_t *dbg) {
    int bp = check_breakpoints(dbg);
    int wp = check_watchpoints(dbg);
    int st = check_step(dbg);
    int should_halt = bp || wp || st;

    if (should_halt) {
        dbg->running = 0;
        if (dbg->halt_callback != 0) {
            dbg->halt_callback(dbg->halt_context);
        }
    }

    return should_halt;
}

int debugger_should_break(debugger_t *dbg) {
    return !dbg->running;
}
