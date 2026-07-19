#include "scheduler.h"

void scheduler_init(scheduler_t *sched) {
    sched->event_count = 0;
    sched->current_time = 0;
    sched->clock_period = 10;
    sched->running = 0;
    sched->halt_requested = 0;
    for (int i = 0; i < MAX_EVENTS; i++) {
        sched->events[i].valid = 0;
    }
}

void scheduler_reset(scheduler_t *sched) {
    sched->event_count = 0;
    sched->current_time = 0;
    sched->running = 0;
    sched->halt_requested = 0;
    for (int i = 0; i < MAX_EVENTS; i++) {
        sched->events[i].valid = 0;
    }
}

void scheduler_set_clock_period(scheduler_t *sched, sim_time_t period) {
    sched->clock_period = period;
}

sim_time_t scheduler_get_time(scheduler_t *sched) {
    return sched->current_time;
}

void scheduler_schedule(scheduler_t *sched, sim_time_t delay, event_callback_fn cb, void *ctx) {
    for (int i = 0; i < MAX_EVENTS; i++) {
        if (!sched->events[i].valid) {
            sched->events[i].time = sched->current_time + delay;
            sched->events[i].callback = cb;
            sched->events[i].context = ctx;
            sched->events[i].valid = 1;
            sched->event_count++;
            return;
        }
    }
}

static int find_next_event(scheduler_t *sched) {
    int min_idx = -1;
    sim_time_t min_time = 0xFFFFFFFFFFFFFFFFULL;

    for (int i = 0; i < MAX_EVENTS; i++) {
        if (sched->events[i].valid && sched->events[i].time < min_time) {
            min_time = sched->events[i].time;
            min_idx = i;
        }
    }
    return min_idx;
}

void scheduler_step(scheduler_t *sched) {
    int idx = find_next_event(sched);
    if (idx >= 0) {
        sched->current_time = sched->events[idx].time;
        sched->events[idx].valid = 0;
        sched->event_count--;

        if (sched->events[idx].callback != 0) {
            sched->events[idx].callback(sched->events[idx].context);
        }
    }
}

void scheduler_run(scheduler_t *sched, sim_time_t duration) {
    sim_time_t end_time = sched->current_time + duration;
    sched->running = 1;
    sched->halt_requested = 0;

    while (sched->running && !sched->halt_requested) {
        int idx = find_next_event(sched);
        if (idx < 0) {
            break;
        }
        if (sched->events[idx].time > end_time) {
            break;
        }
        scheduler_step(sched);
    }

    sched->running = 0;
}

void scheduler_halt(scheduler_t *sched) {
    sched->halt_requested = 1;
}

int scheduler_is_running(scheduler_t *sched) {
    return sched->running;
}
