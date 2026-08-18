#ifndef ALETHEIA_SCHEDULER_H
#define ALETHEIA_SCHEDULER_H

#include "../core/types.h"

#define MAX_EVENTS     256
#define MAX_COMPONENTS 64

typedef unsigned long long sim_time_t;

typedef void (*event_callback_fn)(void *context);

typedef struct {
    sim_time_t time;
    event_callback_fn callback;
    void *context;
    unsigned long long seq;
    int valid;
} event_t;

typedef struct {
    event_t events[MAX_EVENTS];
    int event_count;
    unsigned long long next_seq;
    sim_time_t current_time;
    sim_time_t clock_period;
    int running;
    int halt_requested;
} scheduler_t;

void scheduler_init(scheduler_t *sched);
void scheduler_reset(scheduler_t *sched);
void scheduler_set_clock_period(scheduler_t *sched, sim_time_t period);
sim_time_t scheduler_get_time(scheduler_t *sched);
int scheduler_schedule(scheduler_t *sched, sim_time_t delay, event_callback_fn cb, void *ctx);
void scheduler_step(scheduler_t *sched);
void scheduler_run(scheduler_t *sched, sim_time_t duration);
void scheduler_halt(scheduler_t *sched);
int scheduler_is_running(scheduler_t *sched);

#endif
