#include "debugger.h"

void debugger_init(debugger_t *dbg) {
    dbg->breakpoint_count = 0;
    dbg->watchpoint_count = 0;
    dbg->single_step = 0;
    dbg->running = 0;
    dbg->pc_ptr = 0;
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
    dbg->running = 0;
}

void debugger_set_pc_ptr(debugger_t *dbg, unsigned int *pc) {
    dbg->pc_ptr = pc;
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

void debugger_continue(debugger_t *dbg) {
    dbg->running = 1;
    dbg->single_step = 0;
}

void debugger_step(debugger_t *dbg) {
    dbg->running = 1;
    dbg->single_step = 1;
}

int debugger_check(debugger_t *dbg) {
    int should_halt = 0;

    if (dbg->single_step) {
        should_halt = 1;
    }

    if (dbg->pc_ptr != 0) {
        unsigned int pc = *dbg->pc_ptr;
        for (int i = 0; i < dbg->breakpoint_count; i++) {
            if (dbg->breakpoints[i].enabled && dbg->breakpoints[i].address == pc) {
                dbg->breakpoints[i].hit_count++;
                should_halt = 1;
            }
        }
    }

    for (int i = 0; i < dbg->watchpoint_count; i++) {
        if (dbg->watchpoints[i].enabled && dbg->watchpoints[i].signal_ptr != 0) {
            if (*dbg->watchpoints[i].signal_ptr == dbg->watchpoints[i].trigger_value) {
                dbg->watchpoints[i].hit_count++;
                should_halt = 1;
            }
        }
    }

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
