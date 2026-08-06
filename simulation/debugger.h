#ifndef ALETHEIA_DEBUGGER_H
#define ALETHEIA_DEBUGGER_H

#include "../core/types.h"
#include "scheduler.h"

#define MAX_BREAKPOINTS 32
#define MAX_WATCHPOINTS 32

typedef struct {
    unsigned int address;
    int enabled;
    int hit_count;
} breakpoint_t;

typedef struct {
    bit *signal_ptr;
    bit trigger_value;
    bit last_value;
    int enabled;
    int hit_count;
} watchpoint_t;

typedef struct {
    breakpoint_t breakpoints[MAX_BREAKPOINTS];
    int breakpoint_count;
    watchpoint_t watchpoints[MAX_WATCHPOINTS];
    int watchpoint_count;
    int single_step;
    int step_budget;
    int running;
    int skip_pending;
    unsigned int skip_pc;
    bit *pc_bits;
    void (*halt_callback)(void *);
    void *halt_context;
} debugger_t;

void debugger_init(debugger_t *dbg);
void debugger_reset(debugger_t *dbg);
void debugger_set_pc_bits(debugger_t *dbg, bit *pc);
unsigned int debugger_pc(debugger_t *dbg);
void debugger_set_halt_callback(debugger_t *dbg, void (*cb)(void *), void *ctx);

int debugger_add_breakpoint(debugger_t *dbg, unsigned int address);
void debugger_remove_breakpoint(debugger_t *dbg, int index);
void debugger_enable_breakpoint(debugger_t *dbg, int index, int enable);
void debugger_clear_breakpoints(debugger_t *dbg);

int debugger_add_watchpoint(debugger_t *dbg, bit *signal, bit trigger_value);
void debugger_remove_watchpoint(debugger_t *dbg, int index);
void debugger_enable_watchpoint(debugger_t *dbg, int index, int enable);
void debugger_clear_watchpoints(debugger_t *dbg);

void debugger_set_single_step(debugger_t *dbg, int enable);
void debugger_continue(debugger_t *dbg);
void debugger_step(debugger_t *dbg);
int debugger_check(debugger_t *dbg);
int debugger_should_break(debugger_t *dbg);

#endif
